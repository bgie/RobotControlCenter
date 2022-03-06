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

class QFileSystemWatcher;
class Camera;
class AppSettings;

class CameraManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList availableDevices READ availableDevices NOTIFY availableDevicesChanged)

public:
    explicit CameraManager(AppSettings& settings, QObject* parent = nullptr);

    QStringList availableDevices() const;

    Camera* createCamera(QString deviceName) const;
    bool isValidDevice(QString deviceName) const;

signals:
    void availableDevicesChanged(QStringList list);
    void availableDeviceAdded(QString deviceName);
    void availableDeviceRemoved(QString deviceName);

private:
    void updateAvailableDevices();

private:
    AppSettings& _settings;
    QStringList _availableDevices;
    QFileSystemWatcher& _watcher;
};
