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
#include "RobotNetwork.h"
#include "Robot.h"
#include <QElapsedTimer>
#include <QMap>
#include <QNetworkDatagram>
#include <QTimer>
#include <QUdpSocket>

#define ROBOT_UDP_PORT 8001
#define CONTROL_CENTER_UDP_PORT 8002
#define DISCOVER_INTERVAL_MSEC 500

const QHostAddress listenAddress(QStringLiteral("0.0.0.0"));
const QByteArray discoverMessage("HI");

struct DiscoveredRobot {
    float voltage;
    QElapsedTimer lastDiscoveryTime;
};

struct RobotNetwork::Data {
    Data()
        : connected(false)
        , connectionError(QStringLiteral("Starting up"))
    {
    }

    bool connected;
    QString connectionError;
    QTimer discoveryTimer;
    QUdpSocket listenSocket;
    QUdpSocket sendSocket;
    QMap<QByteArray, Robot*> robots;
};

RobotNetwork::RobotNetwork(QObject* parent)
    : IRobotManager(parent)
    , _d(new Data())
{
    _d->listenSocket.bind(listenAddress, CONTROL_CENTER_UDP_PORT, QAbstractSocket::DontShareAddress);
    connect(&_d->listenSocket, &QUdpSocket::readyRead, this, &RobotNetwork::readListenerSocket);

    connect(&_d->discoveryTimer, &QTimer::timeout, this, &RobotNetwork::discoverRobots);
    _d->discoveryTimer.start(DISCOVER_INTERVAL_MSEC);
    discoverRobots();
}

RobotNetwork::~RobotNetwork()
{
}

bool RobotNetwork::connected() const
{
    return _d->connected;
}

QString RobotNetwork::connectionError() const
{
    return _d->connectionError;
}

Robot* RobotNetwork::robot(QByteArray id) const
{
    return _d->robots.value(id, nullptr);
}

QList<Robot*> RobotNetwork::robots() const
{
    return _d->robots.values();
}

int RobotNetwork::count() const
{
    return _d->robots.size();
}

void RobotNetwork::setConnected(bool connected, QString errorString)
{
    if (_d->connected == connected && _d->connectionError == errorString)
        return;

    _d->connected = connected;
    _d->connectionError = errorString;
    emit connectedChanged(_d->connected);
}

void RobotNetwork::readListenerSocket()
{
    QList<Robot*> newRobots;

    while (_d->listenSocket.hasPendingDatagrams()) {
        QNetworkDatagram datagram = _d->listenSocket.receiveDatagram();
        auto parts = datagram.data().split(' ');
        if (parts.size() >= 2) {
            QByteArray id = parts.at(0);
            float voltage = parts.at(1).toFloat();

            Robot* r = _d->robots[id];
            if (!r) {
                r = new Robot(id, datagram.senderAddress(), ROBOT_UDP_PORT, this);
                _d->robots[id] = r;
                newRobots << r;
            }
            r->discoveryMessageReceived(voltage);
        }
    }

    foreach (auto r, newRobots) {
        emit robotAdded(r);
    }
    if (newRobots.size()) {
        emit robotsChanged();
    }
}

void RobotNetwork::discoverRobots()
{
    const qint64 bytesSent = _d->sendSocket.writeDatagram(discoverMessage, QHostAddress(QHostAddress::Broadcast), ROBOT_UDP_PORT);
    const bool sendOk = (bytesSent == discoverMessage.size());
    setConnected(sendOk, sendOk ? QString() : QStringLiteral("%1 - %2").arg(_d->sendSocket.error()).arg(_d->sendSocket.errorString()));

    QList<Robot*> removedRobots;
    QMutableMapIterator<QByteArray, Robot*> it(_d->robots);
    while (it.hasNext()) {
        Robot* r = it.next().value();
        if (r->hasConnectionTimedOut()) {
            removedRobots << r;
            it.remove();
        }
    }

    foreach (auto r, removedRobots) {
        emit robotRemoved(r);
        r->deleteLater();
    }
    if (removedRobots.size()) {
        emit robotsChanged();
    }
}
