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
#include "PipeController.h"
#include "NonBlockingSenderPipe.h"
#include "RobotCommandPipe.h"
#include "network/Robot.h"
#include <QList>
#include <QTimer>

struct PipeController::Data {
    QScopedPointer<NonBlockingSenderPipe> cameraPipe;
    QString robotPipesPath;
    QList<RobotCommandPipe*> robots;
};

PipeController::PipeController(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PipeController::receiveRobotPipes);
    timer->start(1);
}

PipeController::~PipeController()
{
}

QString PipeController::cameraPipePath() const
{
    return _d->cameraPipe ? _d->cameraPipe->path() : QString();
}

bool PipeController::cameraPipeHasError() const
{
    return _d->cameraPipe ? _d->cameraPipe->hasError() : false;
}

QString PipeController::cameraPipeErrorString() const
{
    return _d->cameraPipe ? _d->cameraPipe->errorString() : QString();
}

QString PipeController::robotPipesPath() const
{
    return _d->robotPipesPath;
}

void PipeController::setRobotPipesPath(QString newPath)
{
    if (_d->robotPipesPath == newPath)
        return;

    _d->robotPipesPath = newPath;

    QMutableListIterator it(_d->robots);
    while (it.hasNext()) {
        Robot* robot = it.value()->robot();
        it.value()->deleteLater();
        it.setValue(createRobotCommandPipe(robot));
    }

    emit robotPipesPathChanged(newPath);
    emit robotCommandPipesChanged();
}

QList<QObject*> PipeController::robotCommandPipes() const
{
    QList<QObject*> result;
    foreach (auto robot, _d->robots) {
        result << robot;
    }
    return result;
}

void PipeController::sendCameraMessage(QByteArray serializedMarkers)
{
    if (_d->cameraPipe) {
        _d->cameraPipe->send(serializedMarkers);
    }
}

void PipeController::addRobot(Robot* robot)
{
    _d->robots.append(createRobotCommandPipe(robot));
    emit robotCommandPipesChanged();
}

void PipeController::removeRobot(Robot* r)
{
    QMutableListIterator it(_d->robots);
    while (it.hasNext()) {
        if (it.next()->robot() == r) {
            it.value()->deleteLater();
            it.remove();
            emit robotCommandPipesChanged();
            break;
        }
    }
}

void PipeController::setCameraPipePath(QString newPath)
{
    if (cameraPipePath() == newPath)
        return;

    _d->cameraPipe.reset(new NonBlockingSenderPipe(newPath));
    connect(_d->cameraPipe.data(), &NonBlockingSenderPipe::errorChanged, this, &PipeController::cameraPipeErrorChanged);

    emit cameraPipePathChanged(newPath);
    emit cameraPipeErrorChanged();
}

RobotCommandPipe* PipeController::createRobotCommandPipe(Robot* robot)
{
    QString path = RobotCommandPipe::generatePath(_d->robotPipesPath, robot->id());
    return new RobotCommandPipe(path, robot, this);
}

void PipeController::receiveRobotPipes()
{
    foreach (auto robot, _d->robots) {
        robot->receive();
    }
}
