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
#include "SettingsController.h"
#include "aruco/MarkerTracker.h"
#include "aruco/Plane3d.h"
#include <QTimer>
#include <math.h>

#define REFRESH_MARKERINFO_INTERVAL_MSEC 500

struct SettingsController::Data {
    Data(MarkerTracker& tracker)
        : tracker(tracker)
        , hasPlane(false)
        , planeAlpha(0.0f)
        , planeBeta(0.0f)
    {
    }

    MarkerTracker& tracker;
    QImage image;
    bool hasPlane;
    float planeAlpha;
    float planeBeta;
    QString markerIds;
};

SettingsController::SettingsController(MarkerTracker& tracker, QObject* parent)
    : QObject(parent)
    , _d(new Data(tracker))
{
    QObject::connect(&tracker, &MarkerTracker::frameProcessed, this, &SettingsController::updateArucoImage);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SettingsController::updateMarkerInfo);
    timer->start(REFRESH_MARKERINFO_INTERVAL_MSEC);
}

SettingsController::~SettingsController()
{
}

QImage SettingsController::arucoImage() const
{
    return _d->image;
}

bool SettingsController::hasPlane() const
{
    return _d->hasPlane;
}

float SettingsController::planeAlpha() const
{
    return _d->planeAlpha;
}

float SettingsController::planeBeta() const
{
    return _d->planeBeta;
}

QString SettingsController::markerIds() const
{
    return _d->markerIds;
}

void SettingsController::setArucoImage(QImage newImage)
{
    if (_d->image == newImage)
        return;

    _d->image = newImage;

    emit arucoImageChanged(_d->image);
}

void SettingsController::updateArucoImage()
{
    setArucoImage(_d->tracker.annotatedImage());
}

void SettingsController::updateMarkerInfo()
{
    auto points = _d->tracker.points();
    auto plane = Plane3d::fitToPoints(points, &_d->hasPlane);
    if (_d->hasPlane) {
        _d->planeAlpha = plane.xAngle() * (180 / M_PI);
        _d->planeBeta = plane.yAngle() * (180 / M_PI);
    }
    emit planeChanged();

    auto ids = _d->tracker.ids();
    qSort(ids);
    QStringList idStrings;
    foreach (int id, ids) {
        idStrings << QString::number(id);
    }
    setMarkerIds(idStrings.empty() ? QStringLiteral("-") : idStrings.join(QStringLiteral(", ")));
}

void SettingsController::setMarkerIds(QString newIds)
{
    if (_d->markerIds == newIds)
        return;

    _d->markerIds = newIds;
    emit markerIdsChanged();
}
