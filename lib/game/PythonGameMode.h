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
#pragma once
#include <QObject>

class IRobotManager;
class RobotSocketManager;
class CameraController;
class SceneTracker;
class GamePadManager;
class GameScene;
class RobotSocket;

class PythonGameMode : public QObject
{
     Q_OBJECT
public:
    PythonGameMode(
        RobotSocketManager& sockets,
        CameraController& camera,
        SceneTracker& tracker,
        GamePadManager& gamePadManger,
        GameScene& gameScene,
        QObject* parent = nullptr);
    virtual ~PythonGameMode() override;

private:
    void onRobotSocketAdded(RobotSocket* socket);
    void onRobotSocketRemoved(RobotSocket* socket);
    void onTrackerCameraFrameProcessed();

private:
    struct Data;
    QScopedPointer<Data> _d;
};

