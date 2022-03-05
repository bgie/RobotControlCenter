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
#include <QImage>
#include <QObject>
#include <QScopedPointer>
#include <QVariantList>

class SceneTracker;
class WorldEdge;
class Aruco;

class SettingsController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QImage arucoImage READ arucoImage NOTIFY arucoImageChanged)
    Q_PROPERTY(bool hasPlane READ hasPlane NOTIFY planeChanged)
    Q_PROPERTY(float planeAlpha READ planeAlpha NOTIFY planeChanged)
    Q_PROPERTY(float planeBeta READ planeBeta NOTIFY planeChanged)
    Q_PROPERTY(QString markerIds READ markerIds NOTIFY markerIdsChanged)
    Q_PROPERTY(QVariantList markerScreenPoints READ markerScreenPoints NOTIFY markerScreenPointsChanged)
    Q_PROPERTY(QString serializedMarkers READ serializedMarkers NOTIFY serializedMarkersChanged)

public:
    SettingsController(SceneTracker& tracker, WorldEdge& worldEdge, Aruco& aruco, QObject* parent = nullptr);
    virtual ~SettingsController() override;

    QImage arucoImage() const;

    bool hasPlane() const;
    float planeAlpha() const;
    float planeBeta() const;

    QString markerIds() const;
    QVariantList markerScreenPoints() const;
    QString serializedMarkers() const;

    Q_INVOKABLE void addPointToWorldEdge(float x, float y, float z);

signals:
    void arucoImageChanged(QImage arucoImage);
    void planeChanged();
    void markerIdsChanged();
    void markerScreenPointsChanged();
    void serializedMarkersChanged();

private:
    void updateArucoImage();
    void setArucoImage(QImage newImage);
    void updateMarkerInfo();
    void setMarkerIds(QString newIds);
    void setSerializedMarkers(QString newValue);

private:
    struct Data;
    QScopedPointer<Data> _d;
};
