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
#include "Marker.h"
#include "kalman/KalmanTracker1D.h"
#include "kalman/KalmanTracker3D.h"
#include "kalman/RotationCounter.h"
#include <math.h>

struct Marker::Data {
    Data(int id)
        : id(id)
        , posFilter(KalmanTracker3D::movingRobotParams())
        , angleFilter(KalmanTracker1D::movingRobotParams())
    {
    }

    const int id;
    bool isDetected;
    QVector3D pos;
    float angle;
    KalmanTracker3D posFilter;
    RotationCounter rotationCounter;
    KalmanTracker1D angleFilter;
};

Marker::Marker(int id)
    : _d(new Data(id))

{
}

Marker::~Marker()
{
}

void Marker::setPositionRotation(const QVector3D& newPos, float newAngle, float elapsedMsecs)
{
    _d->pos = newPos;
    _d->posFilter.predict(elapsedMsecs);
    _d->posFilter.update(newPos);

    _d->angleFilter.predict(elapsedMsecs);
    _d->rotationCounter.updateAngle(newAngle);
    _d->angle = _d->rotationCounter.angleWithRotations();
    _d->angleFilter.update(_d->rotationCounter.angleWithRotations());

    _d->isDetected = true;
}

void Marker::setNotDetected(float elapsedMsecs)
{
    _d->posFilter.predict(elapsedMsecs);
    _d->angleFilter.predict(elapsedMsecs);

    _d->isDetected = false;
}

bool Marker::isDetected() const
{
    return _d->isDetected;
}

QVector3D Marker::pos() const
{
    return _d->pos;
}

float Marker::angle() const
{
    return _d->angle;
}

bool Marker::isDetectedFiltered() const
{
    return _d->posFilter.hasPosition() && _d->angleFilter.hasPosition();
}

QVector3D Marker::filteredPos() const
{
    return _d->posFilter.position();
}

float Marker::filteredAngle() const
{
    double a = _d->angleFilter.position();
    return a - (2 * M_PI * floor(a / (2 * M_PI)));
}
