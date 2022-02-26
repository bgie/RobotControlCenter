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

CameraController::CameraController(QObject* parent)
    : QObject(parent)
    , _connectPossible(false)
    , _currentVideoFormatIndex(-1)
    , _exposure(0)
    , _gain(0)
    , _canCameraStream(false)
    , _isCameraStreaming(false)
{
}

CameraController::~CameraController()
{
}

QString CameraController::videoDevice() const
{
    return _videoDevice;
}

void CameraController::connect()
{
    if (_connectPossible && (_camera.isNull() || _camera->deviceName() != _videoDevice)) {
        _camera.reset(new Camera(_videoDevice));
        QObject::connect(_camera.data(), &Camera::frameRead, this, &CameraController::frameReadAsync, Qt::DirectConnection);
        QObject::connect(_camera.data(), &Camera::frameRead, this, &CameraController::setImage, Qt::QueuedConnection);
        QObject::connect(_camera.data(), &Camera::framesPerSecondChanged, this, &CameraController::framesPerSecondChanged);

        auto formats = _camera->videoFormats();
        setVideoFormats(formats);
        setCurrentVideoFormatIndex(-1);
        _camera->setExposure(_exposure);
        _camera->setGain(_gain);

        emit framesPerSecondChanged(_camera->framesPerSecond());
    }
}

void CameraController::startCameraStream()
{
    if (_canCameraStream && !_camera.isNull() && !_isCameraStreaming) {
        _camera->startStream();
        setIsCameraStreaming(true);
    }
}

void CameraController::stopCameraStream()
{
    if (_isCameraStreaming) {
        _camera->stopStream();
        setImage(QImage());
        setIsCameraStreaming(false);
    }
}

bool CameraController::connectPossible() const
{
    return _connectPossible;
}

QStringList CameraController::videoFormats() const
{
    return _videoFormats;
}

int CameraController::currentVideoFormatIndex() const
{
    return _currentVideoFormatIndex;
}

int CameraController::exposure() const
{
    return _exposure;
}

int CameraController::gain() const
{
    return _gain;
}

bool CameraController::canCameraStream() const
{
    return _canCameraStream;
}

bool CameraController::isCameraStreaming() const
{
    return _isCameraStreaming;
}

float CameraController::framesPerSecond() const
{
    return _camera.isNull() ? 0.0f : _camera->framesPerSecond();
}

QImage CameraController::image() const
{
    return _image;
}

void CameraController::setVideoDevice(QString videoDevice)
{
    if (_videoDevice == videoDevice)
        return;

    stopCameraStream();

    _videoDevice = videoDevice;

    setConnectPossible(Camera::isValidDevice(_videoDevice));

    emit videoDeviceChanged(_videoDevice);

    if (_connectPossible) {
        connect();
    }
}

void CameraController::setConnectPossible(bool connectPossible)
{
    if (_connectPossible == connectPossible)
        return;

    _connectPossible = connectPossible;
    emit connectPossibleChanged(_connectPossible);
}

void CameraController::setVideoFormats(QStringList videoFormats)
{
    if (_videoFormats == videoFormats)
        return;

    _videoFormats = videoFormats;
    emit videoFormatsChanged(_videoFormats);
}

void CameraController::setCurrentVideoFormatIndex(int currentVideoFormatIndex)
{
    if (_currentVideoFormatIndex != currentVideoFormatIndex) {
        _currentVideoFormatIndex = currentVideoFormatIndex;

        if (!_camera.isNull()) {
            _camera->setVideoFormatIndex(_currentVideoFormatIndex);
        }

        emit currentVideoFormatIndexChanged(_currentVideoFormatIndex);
    }

    setCanCameraStream(_camera.isNull() ? false : _camera->canStream());
}

void CameraController::setCanCameraStream(bool canCameraStream)
{
    if (_canCameraStream == canCameraStream)
        return;

    _canCameraStream = canCameraStream;
    emit canCameraStreamChanged(_canCameraStream);
}

void CameraController::setIsCameraStreaming(bool isCameraStreaming)
{
    if (_isCameraStreaming == isCameraStreaming)
        return;

    _isCameraStreaming = isCameraStreaming;
    emit isCameraStreamingChanged(_isCameraStreaming);
}

void CameraController::setImage(QImage image)
{
    if (_image == image)
        return;

    _image = image;
    emit imageChanged(image);
}

void CameraController::setExposure(int value)
{
    if (_exposure == value)
        return;

    _exposure = value;

    if (_camera) {
        _camera->setExposure(_exposure);
    }

    emit exposureChanged(_exposure);
}

void CameraController::setGain(int value)
{
    if (_gain == value)
        return;

    _gain = value;

    if (_camera) {
        _camera->setGain(_gain);
    }

    emit gainChanged(_gain);
}
