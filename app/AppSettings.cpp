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
#include <QSettings>

namespace {
const QString VIDEODEVICE_KEY(QStringLiteral("VideoDevice"));
const QString EXPOSURE_KEY(QStringLiteral("Exposure"));
const QString GAIN_KEY(QStringLiteral("Gain"));
const QString VIDEOFORMATINDEX_KEY(QStringLiteral("VideoFormatIndex"));
const QString CALIBRATIONFILE_KEY(QStringLiteral("CalibrationFile"));
const QString CAMERA_PIPE_PATH_KEY(QStringLiteral("CameraPipePath"));
const QString ROBOT_PIPES_PATH_KEY(QStringLiteral("RobotPipesPath"));
const QString ROBOT_2_MARKER_KEY(QStringLiteral("Robot2Marker"));
}

AppSettings::AppSettings(QObject *parent) : QObject(parent)
{
    QSettings settings;
    _cameraDevice = settings.value(VIDEODEVICE_KEY, QStringLiteral("/dev/video0")).toString();
    _exposure = settings.value(EXPOSURE_KEY, 100).toInt();
    _gain = settings.value(GAIN_KEY, 255).toInt();
    _videoFormatIndex = settings.value(VIDEOFORMATINDEX_KEY, -1).toInt();
    _calibrationFile = settings.value(CALIBRATIONFILE_KEY, QStringLiteral("CameraCalibration.json")).toString();
    _cameraPipePath = settings.value(CAMERA_PIPE_PATH_KEY, QStringLiteral("/robots/camera")).toString();
    _robotPipesPath = settings.value(ROBOT_PIPES_PATH_KEY, QStringLiteral("/robots")).toString();
    auto r2m = settings.value(ROBOT_2_MARKER_KEY).toMap();
    for (auto it = r2m.cbegin(); it != r2m.cend(); ++it) {
        _robot2Marker.insert(it.key().toLatin1(), it.value().toInt());
    }
}

QString AppSettings::cameraDevice() const
{
    return _cameraDevice;
}

int AppSettings::exposure() const
{
    return _exposure;
}

int AppSettings::gain() const
{
    return _gain;
}

int AppSettings::videoFormatIndex() const
{
    return _videoFormatIndex;
}

QString AppSettings::calibrationFile() const
{
    return _calibrationFile;
}

QString AppSettings::cameraPipePath() const
{
    return _cameraPipePath;
}

QString AppSettings::robotPipesPath() const
{
    return _robotPipesPath;
}

QMap<QByteArray, int> AppSettings::robot2Marker() const
{
    return _robot2Marker;
}

void AppSettings::setCameraDevice(QString cameraDevice)
{
    if (_cameraDevice == cameraDevice)
        return;

    _cameraDevice = cameraDevice;

    QSettings settings;
    settings.setValue(VIDEODEVICE_KEY, _cameraDevice);
}

void AppSettings::setExposure(int exposure)
{
    if (_exposure == exposure)
        return;

    _exposure = exposure;

    QSettings settings;
    settings.setValue(EXPOSURE_KEY, _exposure);
}

void AppSettings::setGain(int gain)
{
    if (_gain == gain)
        return;

    _gain = gain;

    QSettings settings;
    settings.setValue(GAIN_KEY, _gain);
}

void AppSettings::setVideoFormatIndex(int videoFormatIndex)
{
    if (_videoFormatIndex == videoFormatIndex)
        return;

    _videoFormatIndex = videoFormatIndex;

    QSettings settings;
    settings.setValue(VIDEOFORMATINDEX_KEY, _videoFormatIndex);
}

void AppSettings::setCalibrationFile(QString calibrationFile)
{
    if (_calibrationFile == calibrationFile)
        return;

    _calibrationFile = calibrationFile;

    QSettings settings;
    settings.setValue(CALIBRATIONFILE_KEY, _calibrationFile);
}

void AppSettings::setCameraPipePath(QString newPath)
{
    if (_cameraPipePath == newPath)
        return;

    _cameraPipePath = newPath;

    QSettings settings;
    settings.setValue(CAMERA_PIPE_PATH_KEY, newPath);
}

void AppSettings::setRobotPipesPath(QString newPath)
{
    if (_robotPipesPath == newPath)
        return;

    _robotPipesPath = newPath;

    QSettings settings;
    settings.setValue(ROBOT_PIPES_PATH_KEY, newPath);
}

void AppSettings::setRobot2Marker(QMap<QByteArray, int> values)
{
    if (_robot2Marker == values)
        return;

    _robot2Marker = values;

    QSettings settings;
    QMap<QString, QVariant> r2m;
    for (auto it = _robot2Marker.cbegin(); it != _robot2Marker.cend(); ++it) {
        r2m.insert(QString::fromLatin1(it.key()), QVariant(it.value()));
    }
    settings.setValue(ROBOT_2_MARKER_KEY, r2m);
}
