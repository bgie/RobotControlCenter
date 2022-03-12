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
#include <QHostAddress>
#include <QObject>

class Robot : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ idString CONSTANT)
    Q_PROPERTY(QString url READ url CONSTANT)
    Q_PROPERTY(float batteryVoltage READ batteryVoltage NOTIFY batteryVoltageChanged)
    Q_PROPERTY(int markerId READ markerId WRITE setMarkerId NOTIFY markerIdChanged)

public:
    Robot(QByteArray id, QHostAddress address, int port, QObject* parent = nullptr);
    virtual ~Robot();

    QByteArray id() const;
    QString idString() const;
    QString url() const;

    float batteryVoltage() const;
    int batteryPercentage() const;

    bool hasMarkerId() const;
    int markerId() const;
    void setMarkerId(int newId = -1);

    void discoveryMessageReceived(float batteryVoltage);
    bool hasConnectionTimedOut() const;

    Q_INVOKABLE void sendCommand(QString command);
    bool sendCommand(QByteArray command);

signals:
    void batteryVoltageChanged();
    void markerIdChanged(int newId);

private:
    void setBatteryVoltage(float newVoltage);    

private:
    struct Data;
    QScopedPointer<Data> _d;
};
