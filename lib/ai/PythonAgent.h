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
#include "IAgent.h"
#include <QThread>

class WorldEdge;

class PythonAgent : public IAgent
{
public:
    PythonAgent(Robot& robot, WorldEdge& worldEdge, QObject* parent = nullptr);
    virtual ~PythonAgent() override;

    virtual void processMarkers(const MarkerList& markers) override;
    virtual void processUserCommand(QByteArray commandLine) override;

    bool insideWorld() const;

private:
    WorldEdge& _worldEdge;
    bool _insideWorld;
    QThread _thread;
};

