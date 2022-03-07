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
#include "ImageItem.h"
#include "aruco/Aruco.h"
#include "aruco/CalibrationController.h"
#include "aruco/SceneTracker.h"
#include "camera/Camera.h"
#include "camera/CameraController.h"
#include "camera/CameraManager.h"
#include "camera/MultiCameraManager.h"
#include "camera/ReplayCamManager.h"
#include "game/PythonGameMode.h"
#include "game/WorldEdge.h"
#include "joystick/GamePad.h"
#include "joystick/GamePadManager.h"
#include "joystick/JoystickManager.h"
#include "joystick/SDL2EventLoop.h"
#include "network/Robot.h"
#include "network/RobotNetwork.h"
#include "pipe/PipeController.h"
#include "pipe/RobotCameraPipe.h"
#include "pipe/RobotCommandPipe.h"
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
    qmlRegisterUncreatableType<ICamera>("RobotControlCenter", 1, 0, "ICamera", noCreateQml);
    qmlRegisterUncreatableType<SettingsController>("RobotControlCenter", 1, 0, "SettingsController", noCreateQml);
    qmlRegisterUncreatableType<GamePadManager>("RobotControlCenter", 1, 0, "GamePadManager", noCreateQml);
    qmlRegisterUncreatableType<GamePad>("RobotControlCenter", 1, 0, "GamePad", noCreateQml);
    qmlRegisterUncreatableType<RobotNetwork>("RobotControlCenter", 1, 0, "RobotNetwork", noCreateQml);
    qmlRegisterUncreatableType<Robot>("RobotControlCenter", 1, 0, "Robot", noCreateQml);
    qmlRegisterUncreatableType<PipeController>("RobotControlCenter", 1, 0, "PipeController", noCreateQml);
    qmlRegisterUncreatableType<RobotCommandPipe>("RobotControlCenter", 1, 0, "RobotCommandPipe", noCreateQml);
    qmlRegisterUncreatableType<RobotCameraPipe>("RobotControlCenter", 1, 0, "RobotCameraPipe", noCreateQml);
    qmlRegisterUncreatableType<PythonGameMode>("RobotControlCenter", 1, 0, "PythonGameMode", noCreateQml);
    qmlRegisterUncreatableType<WorldEdge>("RobotControlCenter", 1, 0, "WorldEdge", noCreateQml);

    AppSettings settings;

    SDL2EventLoop loop;
    JoystickManager joystickManager;
    joystickManager.setEventLoop(&loop);
    GamePadManager gamePadManger(joystickManager);

    QTimer idleTimer;
    QObject::connect(&idleTimer, &QTimer::timeout, &loop, &SDL2EventLoop::processEvents);
    idleTimer.start(0);

    CameraManager cameraManager(settings);
    ReplayCamManager replayCamManager(settings);
    MultiCameraManager multiCameraManager;
    multiCameraManager.addManager(cameraManager);
    multiCameraManager.addManager(replayCamManager);
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
    robotNetwork.setRobot2Marker(settings.robot2Marker());
    QObject::connect(&robotNetwork, &RobotNetwork::robot2MarkerChanged, &settings, &AppSettings::setRobot2Marker);

    PipeController pipeController;
    pipeController.setCameraPipePath(settings.cameraPipePath());
    pipeController.setRobotPipesPath(settings.robotPipesPath());
    QObject::connect(&pipeController, &PipeController::cameraPipePathChanged, &settings, &AppSettings::setCameraPipePath);
    QObject::connect(&pipeController, &PipeController::robotPipesPathChanged, &settings, &AppSettings::setRobotPipesPath);
    QObject::connect(
        &tracker, &SceneTracker::frameProcessed, &pipeController, [&]() {
            pipeController.sendCameraMessage(tracker.markers().serialize());
        },
        Qt::QueuedConnection);
    foreach (Robot* r, robotNetwork.robots()) {
        pipeController.addRobot(r);
    }
    QObject::connect(&robotNetwork, &RobotNetwork::robotAdded, &pipeController, &PipeController::addRobot);
    QObject::connect(&robotNetwork, &RobotNetwork::robotRemoved, &pipeController, &PipeController::removeRobot);

    WorldEdge worldEdge;

    FactoryMethod settingsControllerFactory([&]() -> QObject* {
        return new SettingsController(tracker, worldEdge, aruco);
    });
    FactoryMethod pythonGameModeFactory([&]() -> QObject* {
        return new PythonGameMode(pipeController, cameraController, tracker);
    });

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("settingsControllerFactory"), &settingsControllerFactory);
    engine.rootContext()->setContextProperty(QStringLiteral("pythonGameModeFactory"), &pythonGameModeFactory);
    engine.rootContext()->setContextProperty(QStringLiteral("gamePadManager"), &gamePadManger);
    engine.rootContext()->setContextProperty(QStringLiteral("cameraManager"), &multiCameraManager);
    engine.rootContext()->setContextProperty(QStringLiteral("cameraController"), &cameraController);
    engine.rootContext()->setContextProperty(QStringLiteral("robotNetwork"), &robotNetwork);
    engine.rootContext()->setContextProperty(QStringLiteral("calibrationController"), &calibrationController);
    engine.rootContext()->setContextProperty(QStringLiteral("pipeController"), &pipeController);
    engine.rootContext()->setContextProperty(QStringLiteral("worldEdge"), &worldEdge);
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
