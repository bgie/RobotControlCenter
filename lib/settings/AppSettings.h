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
#include <QMap>
#include <QObject>
#include <QPolygonF>

class AppSettings : public QObject {
    Q_OBJECT

public:
    explicit AppSettings(QObject* parent = nullptr);

    QString cameraDevice() const;
    int exposure(QString cameraDevice) const;
    int gain(QString cameraDevice) const;
    int videoFormatIndex(QString cameraDevice) const;
    QString calibrationFile() const;
    QString cameraPipePath() const;
    QString robotPipesPath() const;
    QMap<QByteArray, int> robot2Marker() const;
    QPolygonF worldEdge() const;
    float worldZ() const;
    QString replayVideosPath() const;
    QString replayVideo() const;
    float replayFramesPerSecond() const;

public slots:
    void setCameraDevice(QString cameraDevice);
    void setExposure(QString cameraDevice, int exposure);
    void setGain(QString cameraDevice, int gain);
    void setVideoFormatIndex(QString cameraDevice, int videoFormatIndex);
    void setCalibrationFile(QString calibrationFile);
    void setCameraPipePath(QString newPath);
    void setRobotPipesPath(QString newPath);
    void setRobot2Marker(QMap<QByteArray, int> values);
    void setWorldEdge(QPolygonF newEdge);
    void setWorldZ(float newZ);
    void setReplayVideosPath(QString newPath);
    void setReplayVideo(QString newVideo);
    void setReplayFramesPerSecond(float newFps);

private:
    QString _cameraDevice;
    QMap<QString, QVariant> _exposure;
    QMap<QString, QVariant> _gain;
    QMap<QString, QVariant> _videoFormatIndex;
    QString _calibrationFile;
    QString _cameraPipePath;
    QString _robotPipesPath;
    QMap<QByteArray, int> _robot2Marker;
    QPolygonF _worldEdge;
    float _worldZ;
    QString _replayVideosPath;
    QString _replayVideo;
    float _replayFps;
};
