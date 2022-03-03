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
#include "Aruco.h"
#include "Marker.h"
#include "MarkerList.h"
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QVector>

class MarkerTracker;

class SceneTracker : public QObject {
    Q_OBJECT
    Q_PROPERTY(float framesPerSecond READ framesPerSecond WRITE setFramesPerSecond NOTIFY framesPerSecondChanged)
    Q_PROPERTY(QImage annotatedImage READ annotatedImage NOTIFY frameProcessed)

public:
    explicit SceneTracker(Aruco& aruco, QObject* parent = nullptr);
    virtual ~SceneTracker() override;

    float framesPerSecond() const;
    void setFramesPerSecond(float newValue);

    void processFrame(QImage image);

    QImage annotatedImage() const;
    QVector<Marker> markers() const;

    QList<QVector3D> points() const;
    QList<int> ids() const;
    QByteArray serializedMarkers() const;
    QVector<QPair<QPointF, QPointF>> markerScreenPoints() const;

signals:
    void framesPerSecondChanged(float framesPerSecond);
    void frameProcessed();
    void markersChanged(QByteArray serializedMarkers);

private:
    QByteArray serializedMarkersImpl() const;

private:
    struct Data;
    QScopedPointer<Data> _d;
};
