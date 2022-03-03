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
#include "SceneTracker.h"
#include "MarkerTracker.h"
#include <QBuffer>
#include <QDebug>
#include <QImage>
#include <QMutexLocker>

struct SceneTracker::Data {
    Data(Aruco& aruco)
        : aruco(aruco)
        , framesPerSecond(1.0f)
    {
    }

    Aruco& aruco;
    QMutex mutex;
    float framesPerSecond;

    QImage image;
    Aruco::Markers markers;
    QMap<int, MarkerTracker*> markerTrackers;
    QVector<Marker> markerList;
};

SceneTracker::SceneTracker(Aruco& aruco, QObject* parent)
    : QObject(parent)
    , _d(new Data(aruco))
{
}

SceneTracker::~SceneTracker()
{
    qDeleteAll(_d->markerTrackers.values());
}

float SceneTracker::framesPerSecond() const
{
    QMutexLocker lock(&_d->mutex);
    return _d->framesPerSecond;
}

void SceneTracker::setFramesPerSecond(float newValue)
{
    QMutexLocker lock(&_d->mutex);
    if (qFuzzyCompare(_d->framesPerSecond, newValue))
        return;

    _d->framesPerSecond = newValue;
    lock.unlock();
    emit framesPerSecondChanged(newValue);
}

void SceneTracker::processFrame(QImage image)
{
    auto markers = _d->aruco.detectMarkers(image);

    QMutexLocker lock(&_d->mutex);
    const float msecsPerFrame = 1000.0f / _d->framesPerSecond;
    QSet<int> foundIds;
    for (size_t i = 0; i < markers.ids.size(); ++i) {
        const int id = markers.ids.at(i);
        foundIds << id;

        if (!_d->markerTrackers.contains(id)) {
            _d->markerTrackers[id] = new MarkerTracker();
        }
        QPointF screenPos(0, 0);
        for (const cv::Point2f& corner : markers.corners.at(i)) {
            screenPos += QPointF(corner.x, corner.y);
        }
        screenPos /= markers.corners.at(i).size();
        const auto& tvec = markers.tvecs.at(i);
        const float angle = _d->aruco.calc2dAngle(markers.rvecs.at(i), tvec);
        _d->markerTrackers[id]->setPositionRotation(screenPos, QVector3D(tvec[0], tvec[1], tvec[2]), angle, msecsPerFrame);
    }
    QVector<Marker> markerList;
    markerList.reserve(_d->markerTrackers.size());

    for (auto it = _d->markerTrackers.cbegin(); it != _d->markerTrackers.cend(); ++it) {
        const int id = it.key();
        MarkerTracker* tracker = it.value();
        if (!foundIds.contains(id)) {
            tracker->setNotDetected(msecsPerFrame);
        }

        markerList << Marker(
            id,
            tracker->isDetected(),
            tracker->screenPos(),
            tracker->pos(),
            tracker->angle(),
            tracker->isDetectedFiltered(),
            tracker->filteredPos(),
            tracker->filteredAngle());
    }

    _d->image = image;
    _d->markers = markers;
    _d->markerList = markerList;

    auto serialized = serializedMarkersImpl();

    lock.unlock();
    emit markersChanged(serialized);
    emit frameProcessed();
}

QImage SceneTracker::annotatedImage() const
{
    QMutexLocker lock(&_d->mutex);
    QImage img = _d->image.copy();
    Aruco::Markers markers = _d->markers;
    lock.unlock();

    _d->aruco.drawMarkers(img, markers);

    return img;
}

QVector<Marker> SceneTracker::markers() const
{
    QMutexLocker lock(&_d->mutex);
    return _d->markerList;
}

QList<QVector3D> SceneTracker::points() const
{
    QList<QVector3D> result;
    QMutexLocker lock(&_d->mutex);
    for (auto it = _d->markerTrackers.cbegin(); it != _d->markerTrackers.cend(); ++it) {
        if (it.value()->isDetectedFiltered()) {
            result << it.value()->filteredPos();
        }
    }
    return result;
}

QList<int> SceneTracker::ids() const
{
    QList<int> result;
    QMutexLocker lock(&_d->mutex);
    for (auto it = _d->markerTrackers.cbegin(); it != _d->markerTrackers.cend(); ++it) {
        if (it.value()->isDetectedFiltered()) {
            result << it.key();
        }
    }
    return result;
}

QByteArray SceneTracker::serializedMarkers() const
{
    QMutexLocker lock(&_d->mutex);
    return serializedMarkersImpl();
}

QVector<QPair<QPointF, QPointF>> SceneTracker::markerScreenPoints() const
{
    QVector<QPair<QPointF, QPointF>> result;
    QMutexLocker lock(&_d->mutex);
    for (auto it = _d->markerTrackers.cbegin(); it != _d->markerTrackers.cend(); ++it) {
        if (it.value()->isDetectedFiltered()) {
            //result << it.key();
        }
    }
    return result;
}

QByteArray SceneTracker::serializedMarkersImpl() const
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QTextStream out(&buffer);
    bool firstItem = true;

    for (auto it = _d->markerTrackers.cbegin(); it != _d->markerTrackers.cend(); ++it) {
        if (it.value()->isDetectedFiltered()) {
            if (firstItem) {
                firstItem = false;
            } else {
                out << ";";
            }
            out << "id:" << it.key()
                << " x:" << static_cast<int>(qRound(it.value()->filteredPos().x()))
                << " y:" << static_cast<int>(qRound(it.value()->filteredPos().y()))
                << " a:" << static_cast<int>(qRound(it.value()->filteredAngle() * 180 / M_PI));
        }
    }
    out << "\n";
    out.flush();
    return buffer.buffer();
}
