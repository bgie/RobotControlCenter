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
#include "NonBlockingReceiverPipe.h"

class Robot;

class RobotCommandPipe : public NonBlockingReceiverPipe {
    Q_OBJECT

public:
    RobotCommandPipe(QString path, Robot* robot, QObject* parent = nullptr);
    virtual ~RobotCommandPipe() override;

    Robot* robot() const;

    static QString generatePath(QString basePath, QByteArray id);

private:
    void onCommandReceived(QByteArray command);

private:
    Robot* const _robot;
};
