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
#include "CameraController.h"
#include "Camera.h"
#include "CameraManager.h"

CameraController::CameraController(CameraManager& manager, QObject* parent)
    : QObject(parent)
    , _cameraManager(manager)
    , _connectPossible(false)
{
}

CameraController::~CameraController()
{
}

QString CameraController::videoDevice() const
{
    return _videoDevice;
}

bool CameraController::connectPossible() const
{
    return _connectPossible;
}

Camera* CameraController::camera() const
{
    return _camera.data();
}

float CameraController::framesPerSecond() const
{
    return _camera ? _camera->framesPerSecond() : 0;
}

QImage CameraController::image() const
{
    return _image;
}

bool CameraController::canStart() const
{
    return _camera && !_camera->isStreaming();
}

bool CameraController::isStreaming() const
{
    return _camera && _camera->isStreaming();
}

void CameraController::startCameraStream() const
{
    if (_camera) {
        _camera->startStream();
    }
}

void CameraController::stopCameraStream() const
{
    if (_camera) {
        _camera->stopStream();
    }
}

void CameraController::setVideoDevice(QString videoDevice)
{
    if (_videoDevice == videoDevice)
        return;

    _videoDevice = videoDevice;

    setConnectPossible(_cameraManager.isValidDevice(_videoDevice));

    if (_connectPossible) {
        _camera.reset(_cameraManager.createCamera(_videoDevice));
        connect(_camera.data(), &Camera::frameRead, this, &CameraController::setImage, Qt::QueuedConnection);
        connect(_camera.data(), &Camera::frameRead, this, &CameraController::frameReadAsync, Qt::DirectConnection);
        connect(_camera.data(), &Camera::framesPerSecondChanged, this, &CameraController::framesPerSecondChanged);
        connect(_camera.data(), &Camera::isStreamingChanged, this, &CameraController::canStartChanged);
        connect(_camera.data(), &Camera::isStreamingChanged, this, &CameraController::isStreamingChanged);
    } else {
        _camera.reset();
    }
    emit cameraChanged(_camera.data());
    emit videoDeviceChanged(_videoDevice);
    emit framesPerSecondChanged(framesPerSecond());
    emit canStartChanged();
    emit isStreamingChanged();
}

void CameraController::setConnectPossible(bool connectPossible)
{
    if (_connectPossible == connectPossible)
        return;

    _connectPossible = connectPossible;
    emit connectPossibleChanged(_connectPossible);
}

void CameraController::setImage(QImage image)
{
    if (_image == image)
        return;

    _image = image;
    emit imageChanged(image);
}
