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
#include "PythonAgent.h"
#include "aruco/MarkerList.h"
#include "game/WorldEdge.h"
#include "robot/Robot.h"
#include <QRegularExpression>

PythonAgent::PythonAgent(Robot& robot, WorldEdge& worldEdge, QObject* parent)
    : IAgent(robot, parent)
    , _worldEdge(worldEdge)
    , _insideWorld(false)
    , _thread()
{
    _thread.start();
}

PythonAgent::~PythonAgent()
{
    _thread.wait();
}

void PythonAgent::processMarkers(const MarkerList& markers)
{
    bool insideWorld = false;
    if (_robot.hasMarkerId()) {
        int i = markers.indexOf(_robot.markerId());
        if (i >= 0) {
            insideWorld = _worldEdge.isInside(markers.at(i).filteredPos().toPointF());
        }
    }
    if (_insideWorld != insideWorld) {
        _insideWorld = insideWorld;
        if (!_insideWorld) {
            _robot.sendCommand(Robot::FULLSTOP_COMMAND);
        }
    }
}

void PythonAgent::processUserCommand(QByteArray commandLine)
{
    qDebug() << commandLine;
    if (_insideWorld) {
        // _robot.sendCommand(command);

        //        auto words = commandLine.simplified().toLower().split(' ');
        //        if (words.size() >= 1) {
        //            QByteArray cmd = words.at(0);
        //            if (cmd == "driveTime") {
        //            }
        //        }
    }
}

bool PythonAgent::insideWorld() const
{
    return _insideWorld;
}
