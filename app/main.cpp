/*  RobotControlCenter
    Copyright (C) 2021-2022 Kuppens Brecht

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "GameSceneItem.h"
#include "ImageItem.h"
#include "aruco/Aruco.h"
#include "aruco/CalibrationController.h"
#include "aruco/SceneTracker.h"
#include "camera/Camera.h"
#include "camera/CameraController.h"
#include "camera/CameraManager.h"
#include "camera/MultiCameraManager.h"
#include "camera/ReplayCamManager.h"
#include "game/GameScene.h"
#include "game/PythonGameMode.h"
#include "game/WorldEdge.h"
#include "joystick/GamePad.h"
#include "joystick/GamePadManager.h"
#include "joystick/JoystickManager.h"
#include "joystick/SDL2EventLoop.h"
#include "performance/LogPerformance.h"
#include "performance/PerformanceMonitor.h"
#include "socket/RobotSocketManager.h"
#include "socket/RobotSocket.h"
#include "robot/FakeRobotManager.h"
#include "robot/MultiRobotManager.h"
#include "robot/Robot.h"
#include "robot/RobotNetwork.h"
#include "robot/RobotSettings.h"
#include "settings/AppSettings.h"
#include "settings/SettingsController.h"
#include "util/FactoryMethod.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QTimer>

const char* const noCreateQml = "Cannot create from QML, has constructor args";

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName(QStringLiteral("RobotControlCenter"));
    QCoreApplication::setOrganizationName(QStringLiteral("Maken!"));
    QGuiApplication app(argc, argv);

    qmlRegisterSingletonType(QUrl("qrc:/Style.qml"), "RobotControlCenter", 1, 0, "Style");
    qmlRegisterType<ImageItem>("RobotControlCenter", 1, 0, "ImageItem");
    qmlRegisterType<GameSceneItem>("RobotControlCenter", 1, 0, "GameSceneItem");
    qmlRegisterUncreatableType<GameScene>("RobotControlCenter", 1, 0, "GameScene", noCreateQml);
    qmlRegisterUncreatableType<ICamera>("RobotControlCenter", 1, 0, "ICamera", noCreateQml);
    qmlRegisterUncreatableType<SettingsController>("RobotControlCenter", 1, 0, "SettingsController", noCreateQml);
    qmlRegisterUncreatableType<GamePadManager>("RobotControlCenter", 1, 0, "GamePadManager", noCreateQml);
    qmlRegisterUncreatableType<GamePad>("RobotControlCenter", 1, 0, "GamePad", noCreateQml);
    qmlRegisterUncreatableType<RobotNetwork>("RobotControlCenter", 1, 0, "RobotNetwork", noCreateQml);
    qmlRegisterUncreatableType<Robot>("RobotControlCenter", 1, 0, "Robot", noCreateQml);
    qmlRegisterUncreatableType<RobotSocketManager>("RobotControlCenter", 1, 0, "RobotSocketManager", noCreateQml);
    qmlRegisterUncreatableType<RobotSocket>("RobotControlCenter", 1, 0, "RobotSocket", noCreateQml);
    qmlRegisterUncreatableType<PythonGameMode>("RobotControlCenter", 1, 0, "PythonGameMode", noCreateQml);
    qmlRegisterUncreatableType<WorldEdge>("RobotControlCenter", 1, 0, "WorldEdge", noCreateQml);
    qmlRegisterUncreatableType<PerformanceMonitor>("RobotControlCenter", 1, 0, "PerformanceMonitor", noCreateQml);

    AppSettings settings;
    PerformanceMonitor monitor;
    GlobalPerformanceMonitor gmon(&monitor);

    SDL2EventLoop loop;
    JoystickManager joystickManager;
    joystickManager.setEventLoop(&loop);
    GamePadManager gamePadManager(joystickManager);

    QTimer idleTimer;
    QObject::connect(&idleTimer, &QTimer::timeout, &loop, &SDL2EventLoop::processEvents);
    idleTimer.start(1);

    CameraManager cameraManager(settings);
    ReplayCamManager replayCamManager(settings);
    MultiCameraManager multiCameraManager;
    multiCameraManager.add(cameraManager);
    multiCameraManager.add(replayCamManager);
    CameraController cameraController(multiCameraManager);
    cameraController.setVideoDevice(settings.cameraDevice());
    QObject::connect(&cameraController, &CameraController::videoDeviceChanged, &settings, &AppSettings::setCameraDevice);

    CalibrationController calibrationController;
    Aruco aruco;
    QObject::connect(&calibrationController, &CalibrationController::calibrationChanged, &aruco, &Aruco::setCameraMatrix);
    calibrationController.setCalibrationFile(settings.calibrationFile());
    QObject::connect(&calibrationController, &CalibrationController::calibrationFileChanged, &settings, &AppSettings::setCalibrationFile);

    QThread trackingThread;
    SceneTracker tracker(aruco);
    tracker.moveToThread(&trackingThread);
    trackingThread.start(QThread::TimeCriticalPriority);
    QObject::connect(&cameraController, &CameraController::frameReadAsync, &tracker, &SceneTracker::processFrame, Qt::QueuedConnection);
    QObject::connect(&cameraController, &CameraController::framesPerSecondChanged, &tracker, &SceneTracker::setFramesPerSecond, Qt::QueuedConnection);
    tracker.setFramesPerSecond(cameraController.framesPerSecond());

    RobotNetwork robotNetwork;
    MultiRobotManager multiRobotManager;
    multiRobotManager.add(robotNetwork);
    FakeRobotManager fakeRobotManager;
    multiRobotManager.add(fakeRobotManager);
    RobotSettings robotSettings(multiRobotManager);
    robotSettings.setRobot2Marker(settings.robot2Marker());
    QObject::connect(&robotSettings, &RobotSettings::robot2MarkerChanged, &settings, &AppSettings::setRobot2Marker);

    RobotSocketManager robotSocketManager(multiRobotManager);

    GameScene gameScene;
    gameScene.worldEdge().setPoints(settings.worldEdge());
    gameScene.worldEdge().setZ(settings.worldZ());
    QObject::connect(&gameScene.worldEdge(), &WorldEdge::pointsChanged, &settings, &AppSettings::setWorldEdge);
    QObject::connect(&gameScene.worldEdge(), &WorldEdge::zChanged, &settings, &AppSettings::setWorldZ);

    FactoryMethod settingsControllerFactory([&]() -> QObject* {
        auto result = new SettingsController(tracker, gameScene.worldEdge(), aruco);
        result->setScreenRotation(gameScene.screenRotation());
        QObject::connect(result, &SettingsController::screenRotationChanged, &gameScene, &GameScene::setScreenRotation);
        return result;
    });
    FactoryMethod pythonGameModeFactory([&]() -> QObject* {
        return new PythonGameMode(robotSocketManager, cameraController, tracker, gamePadManager, gameScene);
    });

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("settingsControllerFactory"), &settingsControllerFactory);
    engine.rootContext()->setContextProperty(QStringLiteral("pythonGameModeFactory"), &pythonGameModeFactory);
    engine.rootContext()->setContextProperty(QStringLiteral("gamePadManager"), &gamePadManager);
    engine.rootContext()->setContextProperty(QStringLiteral("cameraManager"), &multiCameraManager);
    engine.rootContext()->setContextProperty(QStringLiteral("cameraController"), &cameraController);
    engine.rootContext()->setContextProperty(QStringLiteral("robotNetwork"), &robotNetwork);
    engine.rootContext()->setContextProperty(QStringLiteral("robotManager"), &multiRobotManager);
    engine.rootContext()->setContextProperty(QStringLiteral("fakeRobotManager"), &fakeRobotManager);
    engine.rootContext()->setContextProperty(QStringLiteral("calibrationController"), &calibrationController);
    engine.rootContext()->setContextProperty(QStringLiteral("robotSocketManager"), &robotSocketManager);
    engine.rootContext()->setContextProperty(QStringLiteral("gameScene"), &gameScene);
    engine.rootContext()->setContextProperty(QStringLiteral("worldEdge"), &gameScene.worldEdge());
    engine.rootContext()->setContextProperty(QStringLiteral("performanceMonitor"), &monitor);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    auto result = app.exec();

    cameraController.stopCameraStream();
    trackingThread.quit();
    trackingThread.wait();
    return result;
}
