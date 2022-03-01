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
#include "Robot.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QNetworkDatagram>
#include <QTimer>
#include <QUdpSocket>

#define TIMEOUT_INTERVAL_MSEC 2000

struct Robot::Data {
    Data(QByteArray id, QHostAddress address, int port)
        : id(id)
        , address(address)
        , port(port)
        , batteryVoltage(0.0f)
        , markerId(-1)
    {
    }

    const QByteArray id;
    const QHostAddress address;
    const int port;
    float batteryVoltage;
    int markerId;
    QElapsedTimer lastDiscoveryTime;
    QUdpSocket sendSocket;
};

Robot::Robot(QByteArray id, QHostAddress address, int port, QObject* parent)
    : QObject(parent)
    , _d(new Data(id, address, port))
{
}

Robot::~Robot()
{
}

QByteArray Robot::id() const
{
    return _d->id;
}

QString Robot::idString() const
{
    return QString::fromUtf8(_d->id);
}

QString Robot::url() const
{
    return QStringLiteral("%1:%2").arg(_d->address.toString()).arg(_d->port);
}

float Robot::batteryVoltage() const
{
    return _d->batteryVoltage;
}

bool Robot::hasMarkerId() const
{
    return _d->markerId >= 0;
}

int Robot::markerId() const
{
    return _d->markerId;
}

void Robot::setMarkerId(int newId)
{
    if (_d->markerId == newId)
        return;

    _d->markerId = newId;
    emit markerIdChanged(_d->markerId);
}

void Robot::discoveryMessageReceived(float batteryVoltage)
{
    _d->lastDiscoveryTime.start();
    setBatteryVoltage(batteryVoltage);
}

bool Robot::hasConnectionTimedOut() const
{
    return _d->lastDiscoveryTime.hasExpired(TIMEOUT_INTERVAL_MSEC);
}

void Robot::sendCommand(QString command)
{
    sendCommand(command.toLatin1());
}

void Robot::setBatteryVoltage(float newVoltage)
{
    if (qFuzzyIsNull(newVoltage - _d->batteryVoltage))
        return;

    _d->batteryVoltage = newVoltage;
    emit batteryVoltageChanged();
}

bool Robot::sendCommand(QByteArray command)
{
    const qint64 bytesSent = _d->sendSocket.writeDatagram(command, _d->address, _d->port);
    const bool sendOk = (bytesSent == command.size());
    return sendOk;
}
