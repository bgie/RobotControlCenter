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
#include "aruco/Aruco.h"
#include "aruco/Plane3d.h"
#include "aruco/SceneTracker.h"
#include "game/WorldEdge.h"
#include <QDebug>
#include <QTimer>
#include <QVariantMap>
#include <math.h>

#define REFRESH_MARKERINFO_INTERVAL_MSEC 500

struct SettingsController::Data {
    Data(SceneTracker& tracker, WorldEdge& worldEdge, Aruco& aruco)
        : tracker(tracker)
        , worldEdge(worldEdge)
        , aruco(aruco)
        , hasPlane(false)
        , planeAlpha(0.0f)
        , planeBeta(0.0f)
        , screenRotation(0)
    {
    }

    SceneTracker& tracker;
    WorldEdge& worldEdge;
    Aruco& aruco;
    QImage image;
    bool hasPlane;
    float planeAlpha;
    float planeBeta;
    QString markerIds;
    QVariantList markerPoints;
    QString serializedMarkers;
    int screenRotation;
};

SettingsController::SettingsController(SceneTracker& tracker, WorldEdge& worldEdge, Aruco& aruco, QObject* parent)
    : QObject(parent)
    , _d(new Data(tracker, worldEdge, aruco))
{
    QObject::connect(&tracker, &SceneTracker::frameProcessed, this, &SettingsController::updateArucoImage);

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

QVariantList SettingsController::markerScreenPoints() const
{
    return _d->markerPoints;
}

QString SettingsController::serializedMarkers() const
{
    return _d->serializedMarkers;
}

void SettingsController::addPointToWorldEdge(float x, float y, float z)
{
    _d->worldEdge.addPoint(x, y, z);
}

int SettingsController::screenRotation() const
{
    return _d->screenRotation;
}

void SettingsController::setScreenRotation(int screenRotation)
{
    if (_d->screenRotation == screenRotation)
        return;

    _d->screenRotation = screenRotation;
    emit screenRotationChanged(screenRotation);
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
    QImage img = _d->tracker.image();
    _d->aruco.drawPolygon(img, _d->worldEdge.points(), _d->worldEdge.z());
    _d->tracker.annotateImage(img);
    setArucoImage(img);
}

void SettingsController::updateMarkerInfo()
{
    MarkerList markerList = _d->tracker.markers();
    QList<QVector3D> points;
    _d->markerPoints.clear();
    QList<int> ids;
    foreach (const Marker& m, markerList) {
        if (m.isDetectedFiltered()) {
            ids << m.id();
            points << m.filteredPos();
            _d->markerPoints << QVariantMap({ { QStringLiteral("screenX"), m.screenPos().x() },
                { QStringLiteral("screenY"), m.screenPos().y() },
                { QStringLiteral("x"), m.filteredPos().x() },
                { QStringLiteral("y"), m.filteredPos().y() },
                { QStringLiteral("z"), m.filteredPos().z() } });
        }
    }
    emit markerScreenPointsChanged();

    auto plane = Plane3d::fitToPoints(points, &_d->hasPlane);
    if (_d->hasPlane) {
        _d->planeAlpha = plane.xAngle() * (180 / M_PI);
        _d->planeBeta = plane.yAngle() * (180 / M_PI);
    }
    emit planeChanged();

    qSort(ids);
    QStringList idStrings;
    foreach (int id, ids) {
        idStrings << QString::number(id);
    }
    setMarkerIds(idStrings.empty() ? QStringLiteral("-") : QStringLiteral("ids: %1").arg(idStrings.join(QStringLiteral(", "))));

    setSerializedMarkers(markerList.serialize().replace(';', '\n').trimmed());
}

void SettingsController::setMarkerIds(QString newIds)
{
    if (_d->markerIds == newIds)
        return;

    _d->markerIds = newIds;
    emit markerIdsChanged();
}

void SettingsController::setSerializedMarkers(QString newValue)
{
    if (_d->serializedMarkers == newValue)
        return;

    _d->serializedMarkers = newValue;
    emit serializedMarkersChanged();
}
