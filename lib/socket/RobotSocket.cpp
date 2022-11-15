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
#include "RobotSocket.h"
#include "robot/Robot.h"
#include <QLocalServer>
#include <QLocalSocket>

namespace {
QString generateSocketName(QByteArray id)
{
    return QStringLiteral("/robots/%1").arg(QString::fromLatin1(id));
}
}

RobotSocket::RobotSocket(Robot& robot, QObject* parent)
    : QObject(parent)
    , _robot(robot)
    , _server(*new QLocalServer(this))
{
    connect(&_server, &QLocalServer::newConnection, this, &RobotSocket::onNewConnection);
    _server.listen(generateSocketName(robot.id()));
}

RobotSocket::~RobotSocket()
{
}

Robot* RobotSocket::robot() const
{
    return &_robot;
}

QString RobotSocket::path() const
{
    return _server.serverName();
}

bool RobotSocket::hasError() const
{
    return !_errorString.isEmpty();
}

QString RobotSocket::errorString() const
{
    return _errorString;
}

bool RobotSocket::send(QByteArray data)
{
    if (_connections.empty()) {
        setErrorString(QStringLiteral("No receivers connected to robot pipe"));
        return false;
    }

    bool has_error = false;
    for (QLocalSocket* s : qAsConst(_connections)) {
        if (-1 == s->write(data)) {
            has_error = true;
            setErrorString(s->errorString());
        }
    }
    if (!has_error) {
        setErrorString(QString());
    }
    return has_error;
}

void RobotSocket::setErrorString(QString s)
{
    if (_errorString == s)
        return;
    _errorString = s;
    emit errorChanged();
}

void RobotSocket::onNewConnection()
{
    QLocalSocket* socket = _server.nextPendingConnection();
    connect(socket, &QLocalSocket::disconnected, this, [=]() { onSocketDisconnected(socket); });
    _connections.append(socket);
}

void RobotSocket::onSocketDisconnected(QLocalSocket* s)
{
    _connections.removeAll(s);
}
