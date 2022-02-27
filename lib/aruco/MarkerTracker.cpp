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
#include "MarkerTracker.h"
#include "Marker.h"
#include <QBuffer>
#include <QDebug>
#include <QImage>
#include <QMutexLocker>

struct MarkerTracker::Data {
    Data(Aruco& aruco)
        : aruco(aruco)
        , framesPerSecond(1.0f)
    {
    }

    Aruco& aruco;
    QMutex mutex;
    QImage image;
    Aruco::Markers markers;
    QMap<int, Marker*> idToMarker;
    float framesPerSecond;
};

MarkerTracker::MarkerTracker(Aruco& aruco, QObject* parent)
    : QObject(parent)
    , _d(new Data(aruco))
{
}

MarkerTracker::~MarkerTracker()
{
    qDeleteAll(_d->idToMarker.values());
}

float MarkerTracker::framesPerSecond() const
{
    QMutexLocker lock(&_d->mutex);
    return _d->framesPerSecond;
}

void MarkerTracker::setFramesPerSecond(float newValue)
{
    QMutexLocker lock(&_d->mutex);
    if (qFuzzyCompare(_d->framesPerSecond, newValue))
        return;

    _d->framesPerSecond = newValue;
    lock.unlock();
    emit framesPerSecondChanged(newValue);
}

void MarkerTracker::processFrame(QImage image)
{
    auto markers = _d->aruco.detectMarkers(image);
    auto angles = _d->aruco.calc2dAngles(markers);

    QMutexLocker lock(&_d->mutex);
    const float msecsPerFrame = 1000.0f / _d->framesPerSecond;
    QSet<int> foundIds;
    bool newIdAdded = false;
    for (size_t i = 0; i < markers.ids.size(); ++i) {
        const int id = markers.ids.at(i);
        foundIds << id;

        if (!_d->idToMarker.contains(id)) {
            newIdAdded = true;
            _d->idToMarker[id] = new Marker(id);
        }
        const auto& tvec = markers.tvecs.at(i);
        const float angle = angles.at(i);
        _d->idToMarker[id]->setPositionRotation(QVector3D(tvec[0], tvec[1], tvec[2]), angle, msecsPerFrame);
    }
    auto missingIds = _d->idToMarker.keys().toSet() - foundIds;
    for (auto id : missingIds) {
        _d->idToMarker[id]->setNotDetected(msecsPerFrame);
    }

    _d->image = image;
    _d->markers = markers;

    auto serialized = serializedMarkersImpl();

    lock.unlock();
    emit markersChanged(serialized);
    emit frameProcessed();
}

QImage MarkerTracker::image() const
{
    return _d->image;
}

QImage MarkerTracker::annotatedImage() const
{
    QMutexLocker lock(&_d->mutex);
    QImage img = _d->image.copy();
    Aruco::Markers markers = _d->markers;
    lock.unlock();

    _d->aruco.drawMarkers(img, markers);

    return img;
}

QList<QVector3D> MarkerTracker::points() const
{
    QList<QVector3D> result;
    QMutexLocker lock(&_d->mutex);
    for (auto it = _d->idToMarker.cbegin(); it != _d->idToMarker.cend(); ++it) {
        if (it.value()->isDetectedFiltered()) {
            result << it.value()->filteredPos();
        }
    }
    return result;
}

QList<int> MarkerTracker::ids() const
{
    QList<int> result;
    QMutexLocker lock(&_d->mutex);
    for (auto it = _d->idToMarker.cbegin(); it != _d->idToMarker.cend(); ++it) {
        if (it.value()->isDetectedFiltered()) {
            result << it.key();
        }
    }
    return result;
}

QByteArray MarkerTracker::serializedMarkers() const
{
    QMutexLocker lock(&_d->mutex);
    return serializedMarkersImpl();
}

QByteArray MarkerTracker::serializedMarkersImpl() const
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QTextStream out(&buffer);
    bool firstItem = true;

    for (auto it = _d->idToMarker.cbegin(); it != _d->idToMarker.cend(); ++it) {
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
