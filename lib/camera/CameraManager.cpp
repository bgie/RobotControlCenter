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
#include "CameraManager.h"
#include "Camera.h"
#include "settings/AppSettings.h"
#include <QDir>
#include <QFileSystemWatcher>
#include <QSet>

const QDir devPath = QDir(QStringLiteral("/dev/"));
const QStringList videoDevices = QStringList() << QStringLiteral("video*");

CameraManager::CameraManager(AppSettings& settings, QObject* parent)
    : ICameraManager(parent)
    , _settings(settings)
    , _watcher(*new QFileSystemWatcher(this))
{
    _watcher.addPath(devPath.path());
    connect(&_watcher, &QFileSystemWatcher::directoryChanged, this, &CameraManager::updateAvailableDevices);
    updateAvailableDevices();
}

QStringList CameraManager::availableDevices() const
{
    return _availableDevices;
}

ICamera* CameraManager::createCamera(QString deviceName) const
{
    auto result = new Camera(deviceName);
    result->setVideoFormatIndex(_settings.videoFormatIndex(deviceName));
    result->setExposure(_settings.exposure(deviceName));
    result->setGain(_settings.gain(deviceName));
    connect(result, &Camera::gainChanged, this, [=](int newValue) { _settings.setGain(deviceName, newValue); });
    connect(result, &Camera::exposureChanged, this, [=](int newValue) { _settings.setExposure(deviceName, newValue); });
    connect(result, &Camera::videoFormatIndexChanged, this, [=](int newIndex) { _settings.setVideoFormatIndex(deviceName, newIndex); });
    return result;
}

bool CameraManager::isValidDevice(QString deviceName) const
{
    return _availableDevices.contains(deviceName);
}

void CameraManager::updateAvailableDevices()
{
    QSet<QString> oldDevices = _availableDevices.toSet();
    _availableDevices.clear();
    QSet<QString> currentDevices;

    foreach (QString name, devPath.entryList(videoDevices, QDir::System | QDir::Files | QDir::Readable | QDir::Writable, QDir::Name)) {
        QString fullName = devPath.filePath(name);
        _availableDevices.append(fullName);
        currentDevices.insert(fullName);
    }

    bool hasChange = false;
    foreach (QString name, oldDevices - currentDevices) {
        emit availableDeviceRemoved(name);
        hasChange = true;
    }
    foreach (QString name, currentDevices - oldDevices) {
        emit availableDeviceAdded(name);
        hasChange = true;
    }
    if (hasChange) {
        emit availableDevicesChanged();
    }
}
