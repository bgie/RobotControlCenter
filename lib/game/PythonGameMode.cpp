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
#include "PythonGameMode.h"
#include "GameScene.h"
#include "MarkerSceneItem.h"
#include "WorldEdge.h"
#include "ai/PythonAgent.h"
#include "aruco/SceneTracker.h"
#include "camera/CameraController.h"
#include "joystick/GamePad.h"
#include "joystick/GamePadManager.h"
#include "socket/RobotSocketManager.h"
#include "socket/RobotSocket.h"
#include "robot/Robot.h"
#include <QBuffer>
#include <QHash>
#include <QTextStream>
#include <QTransform>
#include <QVector2D>
#include <math.h>

struct PythonGameMode::Data {
    Data(RobotSocketManager& sockets,
        CameraController& camera,
        SceneTracker& tracker,
        GamePadManager& gamePadManger,
        GameScene& gameScene)
        : sockets(sockets)
        , camera(camera)
        , tracker(tracker)
        , gamePadManger(gamePadManger)
        , gameScene(gameScene)
    {
    }

    RobotSocketManager& sockets;
    CameraController& camera;
    SceneTracker& tracker;
    GamePadManager& gamePadManger;
    GameScene& gameScene;
    QHash<RobotSocket*, PythonAgent*> agents;
};

PythonGameMode::PythonGameMode(
    RobotSocketManager& sockets,
    CameraController& camera,
    SceneTracker& tracker,
    GamePadManager& gamePadManger,
    GameScene& gameScene,
    QObject* parent)
    : QObject(parent)
    , _d(new Data(sockets, camera, tracker, gamePadManger, gameScene))
{
    connect(&tracker, &SceneTracker::frameProcessed, this, &PythonGameMode::onTrackerCameraFrameProcessed, Qt::QueuedConnection);
    connect(&sockets, &RobotSocketManager::robotSocketAdded, this, &PythonGameMode::onRobotSocketAdded);
    connect(&sockets, &RobotSocketManager::robotSocketRemoved, this, &PythonGameMode::onRobotSocketRemoved);
    foreach (auto p, sockets.robotSocket()) {
        onRobotSocketAdded(p);
    }

    _d->camera.startCameraStream();
}

PythonGameMode::~PythonGameMode()
{
    _d->camera.stopCameraStream();
    for (auto it = _d->agents.cbegin(); it != _d->agents.cend(); ++it) {
        delete it.value();
    }
}

void PythonGameMode::onRobotSocketAdded(RobotSocket* socket)
{
    auto agent = new PythonAgent(*socket->robot(), _d->gameScene.worldEdge(), this);
    connect(socket, &RobotSocket::received, agent, &IAgent::processUserCommand);
    connect(agent, &IAgent::sendStatusMessage, socket, &RobotSocket::send);
    _d->agents.insert(socket, agent);
}

void PythonGameMode::onRobotSocketRemoved(RobotSocket* socket)
{
    delete _d->agents.take(socket);
}

void PythonGameMode::onTrackerCameraFrameProcessed()
{
    const MarkerList markers = _d->tracker.markers();

    // run robot agents
    for (auto it = _d->agents.cbegin(); it != _d->agents.cend(); ++it) {
        it.value()->processMarkers(markers);
    }

    // draw scene
    QVector<MarkerSceneItem> sceneItems;
    foreach (const auto& marker, markers) {
        if (marker.isDetectedFiltered()) {
            Robot* robot = nullptr;
            for (auto it = _d->agents.cbegin(); it != _d->agents.cend(); ++it) {
                if (it.key()->robot()->markerId() == marker.id()) {
                    robot = it.key()->robot();
                }
            }
            bool isOutsideWorld = !_d->gameScene.worldEdge().isInside(marker.filteredPos().toPointF());
            sceneItems << MarkerSceneItem(
                QString::number(marker.id()),
                marker.isDetected(),
                marker.pos().toPointF(),
                marker.filteredPos().toPointF(),
                marker.filteredAngle(),
                robot,
                isOutsideWorld);
        }
    }
    _d->gameScene.setMarkers(sceneItems);
}
