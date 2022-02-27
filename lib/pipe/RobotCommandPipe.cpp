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
#include "RobotCommandPipe.h"
#include "NonBlockingReceiverPipe.h"
#include "network/Robot.h"
#include <QDir>
#include <QFile>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

RobotCommandPipe::RobotCommandPipe(QString path, Robot* robot, QObject* parent)
    : NonBlockingReceiverPipe(path, parent)
    , _robot(robot)
{
    connect(this, &NonBlockingReceiverPipe::received, this, &RobotCommandPipe::sendCommand);
}

RobotCommandPipe::~RobotCommandPipe()
{
}

Robot* RobotCommandPipe::robot() const
{
    return _robot;
}

QString RobotCommandPipe::generatePath(QString basePath, QByteArray id)
{
    return QDir(basePath).absoluteFilePath(QString::fromLatin1(id));
}

void RobotCommandPipe::sendCommand(QByteArray command)
{
    _robot->sendCommand(command.trimmed().toUpper());
}
