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
#include "joystick/GamePad.h"
#include "joystick/GamePadManager.h"
#include "joystick/JoystickManager.h"
#include "joystick/SDL2EventLoop.h"
#include "settings/SettingsController.h"
#include "util/FactoryMethod.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName(QStringLiteral("RobotControlCenter"));
    QCoreApplication::setOrganizationName(QStringLiteral("Maken!"));
    QGuiApplication app(argc, argv);

    qmlRegisterSingletonType(QUrl("qrc:/Style.qml"), "RobotControlCenter", 1, 0, "Style");
    qmlRegisterType<SettingsController>("RobotControlCenter", 1, 0, "SettingsController");
    qmlRegisterUncreatableType<GamePadManager>("RobotControlCenter", 1, 0, "GamePadManager", "Has constructor args");

    SDL2EventLoop loop;
    JoystickManager joystickManager;
    joystickManager.setEventLoop(&loop);
    GamePadManager gamePadManger(joystickManager);

    QTimer idleTimer;
    QObject::connect(&idleTimer, &QTimer::timeout, &loop, &SDL2EventLoop::processEvents);
    idleTimer.start(0);

    QQmlApplicationEngine engine;

    FactoryMethod settingsControllerFactory([&]() -> QObject* {
        return new SettingsController();
    });
    engine.rootContext()->setContextProperty(QStringLiteral("settingsControllerFactory"), &settingsControllerFactory);
    engine.rootContext()->setContextProperty(QStringLiteral("gamePadManager"), &gamePadManger);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    return app.exec();
}
