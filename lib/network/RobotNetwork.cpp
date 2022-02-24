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
    {
    }

    bool connected;
    QTimer discoveryTimer;
    QUdpSocket listener;
    QHash<QByteArray, Robot*> robots;
};

RobotNetwork::RobotNetwork(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
    _d->listener.bind(listenAddress, CONTROL_CENTER_UDP_PORT, QAbstractSocket::DontShareAddress);
    connect(&_d->listener, &QUdpSocket::readyRead, this, &RobotNetwork::readListenerSocket);

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

QList<QObject*> RobotNetwork::robotQObjects() const
{
    QList<QObject*> result;
    for (auto it = _d->robots.cbegin(); it != _d->robots.cend(); ++it) {
        result << it.value();
    }
    return result;
}

int RobotNetwork::count() const
{
    return _d->robots.size();
}

void RobotNetwork::setConnected(bool connected)
{
    if (_d->connected == connected)
        return;

    _d->connected = connected;
    emit connectedChanged(_d->connected);
}

void RobotNetwork::readListenerSocket()
{
    QList<Robot*> newRobots;

    while (_d->listener.hasPendingDatagrams()) {
        QNetworkDatagram datagram = _d->listener.receiveDatagram();
        auto parts = datagram.data().split(' ');
        if (parts.size() >= 2) {
            QByteArray id = parts.at(0);
            float voltage = parts.at(1).toFloat();

            Robot* r = _d->robots[id];
            if (!r) {
                newRobots << (_d->robots[id] = r = new Robot(id, this));
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
    QUdpSocket socket;
    setConnected(-1 != socket.writeDatagram(discoverMessage, QHostAddress(QHostAddress::Broadcast), ROBOT_UDP_PORT));

    QList<Robot*> removedRobots;
    QMutableHashIterator<QByteArray, Robot*> it(_d->robots);
    while (it.hasNext()) {
        Robot* r = it.next().value();
        if (r->connectionTimedOut()) {
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
