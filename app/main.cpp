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
#include "AppSettings.h"
#include "ImageItem.h"
#include "aruco/Aruco.h"
#include "aruco/CalibrationController.h"
#include "camera/CameraController.h"
#include "camera/CameraManager.h"
#include "joystick/GamePad.h"
#include "joystick/GamePadManager.h"
#include "joystick/JoystickManager.h"
#include "joystick/SDL2EventLoop.h"
#include "network/RobotNetwork.h"
#include "settings/SettingsController.h"
#include "util/FactoryMethod.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
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
    qmlRegisterUncreatableType<SettingsController>("RobotControlCenter", 1, 0, "SettingsController", noCreateQml);
    qmlRegisterUncreatableType<GamePadManager>("RobotControlCenter", 1, 0, "GamePadManager", noCreateQml);

    AppSettings settings;

    SDL2EventLoop loop;
    JoystickManager joystickManager;
    joystickManager.setEventLoop(&loop);
    GamePadManager gamePadManger(joystickManager);

    QTimer idleTimer;
    QObject::connect(&idleTimer, &QTimer::timeout, &loop, &SDL2EventLoop::processEvents);
    idleTimer.start(0);

    CameraManager cameraManager;
    CameraController cameraController;
    cameraController.setVideoDevice(settings.cameraDevice());
    cameraController.setGain(settings.gain());
    cameraController.setExposure(settings.exposure());
    cameraController.setCurrentVideoFormatIndex(settings.videoFormatIndex());
    QObject::connect(&cameraController, &CameraController::videoDeviceChanged, &settings, &AppSettings::setCameraDevice);
    QObject::connect(&cameraController, &CameraController::gainChanged, &settings, &AppSettings::setGain);
    QObject::connect(&cameraController, &CameraController::exposureChanged, &settings, &AppSettings::setExposure);
    QObject::connect(&cameraController, &CameraController::currentVideoFormatIndexChanged, &settings, &AppSettings::setVideoFormatIndex);

    CalibrationController calibrationController;
    Aruco aruco;
    QObject::connect(&calibrationController, &CalibrationController::calibrationChanged, &aruco, &Aruco::setCameraMatrix);
    calibrationController.setCalibrationFile(settings.calibrationFile());
    QObject::connect(&calibrationController, &CalibrationController::calibrationFileChanged, &settings, &AppSettings::setCalibrationFile);

    RobotNetwork robotNetwork;

    FactoryMethod settingsControllerFactory([&]() -> QObject* {
        auto result = new SettingsController(aruco);
        QObject::connect(&cameraController, &CameraController::frameReadAsync, result, &SettingsController::setImage, Qt::DirectConnection);
        return result;
    });

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("settingsControllerFactory"), &settingsControllerFactory);
    engine.rootContext()->setContextProperty(QStringLiteral("gamePadManager"), &gamePadManger);
    engine.rootContext()->setContextProperty(QStringLiteral("cameraManager"), &cameraManager);
    engine.rootContext()->setContextProperty(QStringLiteral("cameraController"), &cameraController);
    engine.rootContext()->setContextProperty(QStringLiteral("robotNetwork"), &robotNetwork);
    engine.rootContext()->setContextProperty(QStringLiteral("calibrationController"), &calibrationController);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    auto result = app.exec();

    cameraController.stopCameraStream();
    return result;
}
