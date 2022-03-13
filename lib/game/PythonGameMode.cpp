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
#include "aruco/SceneTracker.h"
#include "camera/CameraController.h"
#include "pipe/PipeController.h"
#include "pipe/RobotCameraPipe.h"
#include "robot/Robot.h"
#include <QBuffer>
#include <QTextStream>
#include <QTransform>
#include <QVector2D>
#include <math.h>

struct PythonGameMode::Data {
    Data(PipeController& pipes, CameraController& camera, SceneTracker& tracker, GameScene& gameScene)
        : pipes(pipes)
        , camera(camera)
        , tracker(tracker)
        , gameScene(gameScene)
    {
    }

    PipeController& pipes;
    CameraController& camera;
    SceneTracker& tracker;
    GameScene& gameScene;
};

PythonGameMode::PythonGameMode(PipeController& pipes, CameraController& camera, SceneTracker& tracker, GameScene& gameScene, QObject* parent)
    : QObject(parent)
    , _d(new Data(pipes, camera, tracker, gameScene))
{
    connect(&tracker, &SceneTracker::frameProcessed, this, &PythonGameMode::onTrackerCameraFrameProcessed, Qt::QueuedConnection);

    _d->camera.startCameraStream();
}

PythonGameMode::~PythonGameMode()
{
    _d->camera.stopCameraStream();
}

void PythonGameMode::onTrackerCameraFrameProcessed()
{
    const MarkerList markers = _d->tracker.markers();
    const QList<RobotCameraPipe*> cameraPipes = _d->pipes.robotCameraPipes();
    QVector<MarkerSceneItem> sceneItems;

    foreach (const auto& marker, markers) {
        if (marker.isDetectedFiltered()) {
            bool isRobot = false;
            foreach (auto pipe, cameraPipes) {
                if (pipe->robot()->markerId() == marker.id()) {
                    isRobot = true;
                }
            }
            bool isOutsideWorld = !_d->gameScene.worldEdge().isInside(marker.filteredPos().toPointF());
            sceneItems << MarkerSceneItem(
                QString::number(marker.id()),
                marker.isDetected(),
                marker.pos().toPointF(),
                marker.filteredPos().toPointF(),
                marker.filteredAngle(),
                isRobot,
                isOutsideWorld);
        }
    }
    _d->gameScene.setMarkers(sceneItems);

    foreach (auto pipe, cameraPipes) {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        QTextStream out(&buffer);

        auto robot = pipe->robot();
        out << "bat:" << robot->batteryPercentage();

        if (robot->hasMarkerId()) {
            const int robotMarkerId = robot->markerId();
            const int markerIndex = markers.find(robotMarkerId);
            if (markerIndex >= 0) {

                auto robotMarker = markers.at(markerIndex);
                if (robotMarker.isDetectedFiltered()) {
                    out << " x:" << static_cast<int>(qRound(robotMarker.filteredPos().x()))
                        << " y:" << static_cast<int>(qRound(robotMarker.filteredPos().y()));

                    QTransform povTransform;
                    povTransform.rotateRadians(-robotMarker.filteredAngle());
                    povTransform.translate(-robotMarker.filteredPos().x(), -robotMarker.filteredPos().y());

                    for (auto it = markers.cbegin(); it != markers.cend(); ++it) {
                        if (it->id() != robotMarkerId && it->isDetectedFiltered()) {
                            QVector2D pos(povTransform.map(it->filteredPos().toPointF()));
                            float angle = atan2(pos.y(), pos.x()) * 180 / M_PI;

                            out << ";id:" << it->id()
                                << " d:" << static_cast<int>(qRound(pos.length()))
                                << " a:" << static_cast<int>(qRound(angle));
                        }
                    }
                } else {
                    out << " x:? y:?";
                }
            }
        }
        out << "\n";
        out.flush();
        pipe->send(buffer.buffer());
    }
}
