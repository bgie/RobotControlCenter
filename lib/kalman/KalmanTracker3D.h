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
#include <QScopedPointer>
#include <QVector3D>

class KalmanTracker3D {
public:
    struct Params {
        Params(
            double positionXYProcessNoiseCov = 1e-5,
            double positionZProcessNoiseCov = 1e-8,
            double velocityXYProcessNoiseCov = 1e-5,
            double velocityZProcessNoiseCov = 1e-8,
            double measurementXYNoiseCov = 1,
            double measurementZNoiseCov = 1,
            bool useTimeout = true,
            double notUpdatedTimeoutInMsec = 3000);

        double positionXYProcessNoiseCov;
        double positionZProcessNoiseCov;
        double velocityXYProcessNoiseCov;
        double velocityZProcessNoiseCov;
        double measurementXYNoiseCov;
        double measurementZNoiseCov;
        bool useTimeout;
        double notUpdatedTimeoutInMsec;
    };

    KalmanTracker3D(const Params& p = Params());
    ~KalmanTracker3D();

    void update(const QVector3D& position);
    void predict(double elapsedMsec);

    bool hasPosition() const;
    QVector3D position() const;

    static const Params& movingRobotParams();
    static const Params& staticMarkerParams();

private:
    struct Data;
    QScopedPointer<Data> _d;
};
