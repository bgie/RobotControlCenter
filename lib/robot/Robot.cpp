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
#include "ai/IAgent.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QNetworkDatagram>
#include <QPointer>
#include <QTimer>
#include <QUdpSocket>

#define TIMEOUT_INTERVAL_MSEC 2000

const QByteArray Robot::FULLSTOP_COMMAND(QByteArrayLiteral("MOTOR 0 0"));

struct Robot::Data {
    Data(QByteArray id, QHostAddress address, int port)
        : id(id)
        , address(address)
        , port(port)
        , batteryCharge(0)
        , markerId(-1)
        , gamepadIndex(0)
    {
    }

    const QByteArray id;
    const QHostAddress address;
    const int port;
    int batteryCharge;
    int markerId;
    int gamepadIndex;
    QByteArray lastCommand;
    QElapsedTimer lastDiscoveryTime;
    QUdpSocket sendSocket;
    QScopedPointer<IAgent> agent;
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
    if (_d->address.isNull())
        return QStringLiteral("-");
    return QStringLiteral("%1:%2").arg(_d->address.toString()).arg(_d->port);
}

float Robot::batteryCharge() const
{
    return _d->batteryCharge;
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

void Robot::discoveryMessageReceived(int batteryCharge)
{
    _d->lastDiscoveryTime.start();
    setBatteryVoltage(batteryCharge);
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
    if (qFuzzyIsNull(newVoltage - _d->batteryCharge))
        return;

    _d->batteryCharge = newVoltage;
    emit batteryChargeChanged();
}

bool Robot::sendCommand(QByteArray command)
{
    bool sendOk = false;
    if (!_d->address.isNull()) {
        const qint64 bytesSent = _d->sendSocket.writeDatagram(command, _d->address, _d->port);
        sendOk = (bytesSent == command.size());
    }
    if (sendOk) {
        _d->lastCommand = command;
    } else {
        _d->lastCommand = QByteArrayLiteral("Failed to send command! - ") + command;
    }
    emit lastCommandChanged();
    return sendOk;
}

void Robot::processUserCommand(QByteArray command)
{
    if (_d->agent) {
        _d->agent->processUserCommand(command);
    }
}

void Robot::setAgent(IAgent* agent)
{
    _d->agent.reset(agent);
}

IAgent* Robot::agent() const
{
    return _d->agent.data();
}

QByteArray Robot::lastCommand() const
{
    return _d->lastCommand;
}

void Robot::setGamepadIndex(int gamepadIndex)
{
    if (_d->gamepadIndex == gamepadIndex)
        return;

    _d->gamepadIndex = gamepadIndex;
    emit gamepadIndexChanged(_d->gamepadIndex);
}

int Robot::gamepadIndex() const
{
    return _d->gamepadIndex;
}
