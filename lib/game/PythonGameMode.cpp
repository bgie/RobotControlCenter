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
#include "aruco/MarkerTracker.h"
#include "camera/CameraController.h"
#include "pipe/PipeController.h"

struct PythonGameMode::Data {
    Data(PipeController& pipes, CameraController& camera, MarkerTracker& tracker)
        : pipes(pipes)
        , camera(camera)
        , tracker(tracker)
    {
    }

    PipeController& pipes;
    CameraController& camera;
    MarkerTracker& tracker;
};

PythonGameMode::PythonGameMode(PipeController& pipes, CameraController& camera, MarkerTracker& tracker, QObject* parent)
    : QObject(parent)
    , _d(new Data(pipes, camera, tracker))
{
    connect(&tracker, &MarkerTracker::frameProcessed, this, &PythonGameMode::onTrackerCameraFrameProcessed);

    _d->camera.startCameraStream();
}

PythonGameMode::~PythonGameMode()
{
    _d->camera.stopCameraStream();
}

void PythonGameMode::onTrackerCameraFrameProcessed()
{
}
