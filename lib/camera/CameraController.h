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
#include <QImage>
#include <QObject>

class CameraManager;
class Camera;

class CameraController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString videoDevice READ videoDevice WRITE setVideoDevice NOTIFY videoDeviceChanged)
    Q_PROPERTY(bool connectPossible READ connectPossible NOTIFY connectPossibleChanged)
    Q_PROPERTY(QImage image READ image NOTIFY imageChanged)
    Q_PROPERTY(Camera* camera READ camera NOTIFY cameraChanged)
    Q_PROPERTY(float framesPerSecond READ framesPerSecond NOTIFY framesPerSecondChanged)
    Q_PROPERTY(bool canStart READ canStart NOTIFY canStartChanged)
    Q_PROPERTY(bool isStreaming READ isStreaming NOTIFY isStreamingChanged)

public:
    explicit CameraController(CameraManager& manager, QObject* parent = nullptr);
    virtual ~CameraController();

    QString videoDevice() const;
    bool connectPossible() const;
    Camera* camera() const;
    float framesPerSecond() const;

    QImage image() const;

    bool canStart() const;
    bool isStreaming() const;
    Q_INVOKABLE void startCameraStream() const;
    Q_INVOKABLE void stopCameraStream() const;

public slots:
    void setVideoDevice(QString videoDevice);

signals:
    void videoDeviceChanged(QString videoDevice);
    void connectPossibleChanged(bool connectPossible);
    void cameraChanged(Camera* camera);
    void imageChanged(QImage image);
    void frameReadAsync(const QImage image);
    void framesPerSecondChanged(float value);
    void canStartChanged();
    void isStreamingChanged();

private slots:
    void setConnectPossible(bool connectPossible);
    void setImage(QImage image);

private:
    CameraManager& _cameraManager;
    QString _videoDevice;
    bool _connectPossible;
    QScopedPointer<Camera> _camera;
    QImage _image;
};
