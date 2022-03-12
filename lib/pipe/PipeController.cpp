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
#include "RobotCameraPipe.h"
#include "RobotCommandPipe.h"
#include "robot/Robot.h"
#include <QList>
#include <QTimer>

struct PipeController::Data {
    QScopedPointer<NonBlockingSenderPipe> cameraPipe;
    QString robotPipesPath;
    QList<RobotCommandPipe*> robotCommandPipes;
    QList<RobotCameraPipe*> robotCameraPipes;
};

PipeController::PipeController(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PipeController::receiveCommandsOnRobotPipes);
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

    QMutableListIterator it(_d->robotCommandPipes);
    while (it.hasNext()) {
        Robot* robot = it.value()->robot();
        it.value()->deleteLater();
        it.setValue(createRobotCommandPipe(robot));
    }
    QMutableListIterator it2(_d->robotCameraPipes);
    while (it2.hasNext()) {
        Robot* robot = it2.value()->robot();
        it2.value()->deleteLater();
        it2.setValue(createRobotCameraPipe(robot));
    }

    emit robotPipesPathChanged(newPath);
    emit robotPipesChanged();
}

bool PipeController::robotsConnected() const
{
    return !_d->robotCommandPipes.empty();
}

QList<RobotCommandPipe*> PipeController::robotCommandPipes() const
{
    return _d->robotCommandPipes;
}

QList<QObject*> PipeController::robotCommandPipesQObject() const
{
    QList<QObject*> result;
    foreach (auto robot, _d->robotCommandPipes) {
        result << robot;
    }
    return result;
}

QList<RobotCameraPipe*> PipeController::robotCameraPipes() const
{
    return _d->robotCameraPipes;
}

QList<QObject*> PipeController::robotCameraPipesQObject() const
{
    QList<QObject*> result;
    foreach (auto robot, _d->robotCameraPipes) {
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
    _d->robotCommandPipes.append(createRobotCommandPipe(robot));
    _d->robotCameraPipes.append(createRobotCameraPipe(robot));
    emit robotPipesChanged();
}

void PipeController::removeRobot(Robot* r)
{
    bool changed = false;
    QMutableListIterator it(_d->robotCommandPipes);
    while (it.hasNext()) {
        if (it.next()->robot() == r) {
            it.value()->deleteLater();
            it.remove();
            changed = true;
            break;
        }
    }
    QMutableListIterator it2(_d->robotCameraPipes);
    while (it2.hasNext()) {
        if (it2.next()->robot() == r) {
            it2.value()->deleteLater();
            it2.remove();
            changed = true;
            break;
        }
    }
    if (changed) {
        emit robotPipesChanged();
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

RobotCameraPipe* PipeController::createRobotCameraPipe(Robot* robot)
{
    QString path = RobotCameraPipe::generatePath(_d->robotPipesPath, robot->id());
    return new RobotCameraPipe(path, robot, this);
}

void PipeController::receiveCommandsOnRobotPipes()
{
    foreach (auto robot, _d->robotCommandPipes) {
        robot->receive();
    }
}
