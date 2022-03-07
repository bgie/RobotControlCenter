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
#include "ReplayCam.h"
#include <QDir>

struct ReplayCam::Data {
    Data()
        : framesPerSecond(30.0f)
        , videoIndex(-1)
    {
    }

    float framesPerSecond;
    QString videosPath;
    QStringList videos;
    int videoIndex;
};

ReplayCam::ReplayCam(QObject* parent)
    : ICamera(parent)
    , _d(new Data())
{
    connect(this, &ICamera::framesPerSecondChanged, this, &ReplayCam::framesPerSecondChangedNOPARAM);
}

ReplayCam::~ReplayCam()
{
}

QString ReplayCam::className() const
{
    return QStringLiteral("ReplayCam");
}

float ReplayCam::framesPerSecond() const
{
    return _d->framesPerSecond;
}

bool ReplayCam::canStream() const
{
    return false;
}

void ReplayCam::startStream()
{
}

void ReplayCam::stopStream()
{
}

bool ReplayCam::isStreaming() const
{
    return false;
}

void ReplayCam::setFramesPerSecond(float newValue)
{
    if (qFuzzyIsNull(_d->framesPerSecond - newValue))
        return;

    _d->framesPerSecond = newValue;
    emit framesPerSecondChanged(_d->framesPerSecond);
}

QString ReplayCam::videosPath() const
{
    return _d->videosPath;
}

void ReplayCam::setVideosPath(QString videosPath)
{
    if (_d->videosPath == videosPath)
        return;

    _d->videosPath = videosPath;

    updateVideos();
    setVideoIndex(-1);

    emit videosPathChanged(_d->videosPath);
    emit videosChanged();
}

bool ReplayCam::videosPathValid() const
{
    return QDir(_d->videosPath).exists();
}

QStringList ReplayCam::videos() const
{
    return _d->videos;
}

QString ReplayCam::video() const
{
    return _d->videos.value(_d->videoIndex, QString());
}

int ReplayCam::videoIndex() const
{
    return _d->videoIndex;
}

void ReplayCam::setVideo(QString newVideo)
{
    setVideoIndex(_d->videos.indexOf(newVideo));
}

void ReplayCam::setVideoIndex(int newIndex)
{
    if (_d->videoIndex == newIndex)
        return;

    _d->videoIndex = newIndex;
    emit videoIndexChanged(_d->videoIndex);
    emit videoChanged(video());
}

void ReplayCam::updateVideos()
{
    _d->videos = QDir(_d->videosPath).entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);
}
