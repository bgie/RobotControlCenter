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
#include "MultiCameraManager.h"

MultiCameraManager::MultiCameraManager(QObject* parent)
    : ICameraManager(parent)
{
}

void MultiCameraManager::add(ICameraManager& manager)
{
    _managers.append(&manager);
    connect(&manager, &ICameraManager::availableDevicesChanged, this, &ICameraManager::availableDevicesChanged);
    connect(&manager, &ICameraManager::availableDeviceAdded, this, &ICameraManager::availableDeviceAdded);
    connect(&manager, &ICameraManager::availableDeviceRemoved, this, &ICameraManager::availableDeviceRemoved);

    foreach (auto dev, manager.availableDevices()) {
        emit availableDeviceAdded(dev);
    }
    emit availableDevicesChanged();
}

QStringList MultiCameraManager::availableDevices() const
{
    QStringList result;
    foreach (auto manager, _managers) {
        result += manager->availableDevices();
    }
    return result;
}

ICamera* MultiCameraManager::createCamera(QString deviceName) const
{
    foreach (auto manager, _managers) {
        if (manager->isValidDevice(deviceName)) {
            auto result = manager->createCamera(deviceName);
            if (result) {
                return result;
            }
        }
    }
    return nullptr;
}

bool MultiCameraManager::isValidDevice(QString deviceName) const
{
    foreach (auto manager, _managers) {
        if (manager->isValidDevice(deviceName)) {
            return true;
        }
    }
    return false;
}
