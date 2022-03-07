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
#include <QObject>

class ICamera : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString className READ className CONSTANT)
    Q_PROPERTY(float framesPerSecond READ framesPerSecond NOTIFY framesPerSecondChanged)
    Q_PROPERTY(bool canStream READ canStream NOTIFY canStreamChanged)
    Q_PROPERTY(bool isStreaming READ isStreaming NOTIFY isStreamingChanged)

public:
    explicit ICamera(QObject* parent = nullptr);

    virtual QString className() const = 0;
    virtual float framesPerSecond() const = 0;
    virtual bool canStream() const = 0;
    virtual void startStream() = 0;
    virtual void stopStream() = 0;
    virtual bool isStreaming() const = 0;

signals:
    void framesPerSecondChanged(float value);
    void canStreamChanged(bool val);
    void isStreamingChanged(bool val);
    void frameRead(const QImage image);
};
