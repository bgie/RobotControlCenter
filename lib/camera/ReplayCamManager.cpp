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
#include "ReplayCamManager.h"
#include "ReplayCam.h"
#include "settings/AppSettings.h"

const QString REPLAY_CAM_DEVICE_NAME(QStringLiteral("Replay"));

ReplayCamManager::ReplayCamManager(AppSettings& settings, QObject* parent)
    : ICameraManager(parent)
    , _settings(settings)
{
}

QStringList ReplayCamManager::availableDevices() const
{
    static QStringList list({ REPLAY_CAM_DEVICE_NAME });
    return list;
}

ICamera* ReplayCamManager::createCamera(QString deviceName) const
{
    if (isValidDevice(deviceName)) {
        auto result = new ReplayCam();

        result->setVideosPath(_settings.replayVideosPath());
        result->setVideo(_settings.replayVideo());
        result->setFramesPerSecond(_settings.replayFramesPerSecond());
        connect(result, &ReplayCam::videosPathChanged, &_settings, &AppSettings::setReplayVideosPath);
        connect(result, &ReplayCam::videoChanged, &_settings, &AppSettings::setReplayVideo);
        connect(result, &ICamera::framesPerSecondChanged, &_settings, &AppSettings::setReplayFramesPerSecond);

        return result;
    }
    return nullptr;
}

bool ReplayCamManager::isValidDevice(QString deviceName) const
{
    return deviceName == REPLAY_CAM_DEVICE_NAME;
}
