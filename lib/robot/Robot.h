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

class MarkerList;
class IAgent;

class Robot : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ idString CONSTANT)
    Q_PROPERTY(QString url READ url CONSTANT)
    Q_PROPERTY(int batteryCharge READ batteryCharge NOTIFY batteryChargeChanged)
    Q_PROPERTY(int markerId READ markerId WRITE setMarkerId NOTIFY markerIdChanged)
    Q_PROPERTY(int gamepadIndex READ gamepadIndex WRITE setGamepadIndex NOTIFY gamepadIndexChanged)
    Q_PROPERTY(QByteArray lastCommand READ lastCommand NOTIFY lastCommandChanged)

public:
    Robot(QByteArray id, QHostAddress address, int port, QObject* parent = nullptr);
    virtual ~Robot();

    QByteArray id() const;
    QString idString() const;
    QString url() const;

    float batteryCharge() const;

    bool hasMarkerId() const;
    int markerId() const;
    void setMarkerId(int newId = -1);
    int gamepadIndex() const;
    void setGamepadIndex(int gamepadIndex);

    void discoveryMessageReceived(int batteryCharge);
    bool hasConnectionTimedOut() const;

    Q_INVOKABLE void sendCommand(QString command);
    bool sendCommand(QByteArray command);

    static const QByteArray FULLSTOP_COMMAND;

    void processUserCommand(QByteArray command);
    void setAgent(IAgent* agent); // takes ownership
    IAgent* agent() const;

    QByteArray lastCommand() const;

signals:
    void batteryChargeChanged();
    void markerIdChanged(int newId);
    void gamepadIndexChanged(int gamepadIndex);
    void lastCommandChanged();

private:
    void setBatteryVoltage(float newVoltage);    

private:
    struct Data;
    QScopedPointer<Data> _d;
};
