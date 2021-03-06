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
#include <QObject>
#include <QScopedPointer>

class Robot;
class RobotCommandPipe;
class RobotCameraPipe;

class PipeController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString cameraPipePath READ cameraPipePath WRITE setCameraPipePath NOTIFY cameraPipePathChanged)
    Q_PROPERTY(bool cameraPipeHasError READ cameraPipeHasError NOTIFY cameraPipeErrorChanged)
    Q_PROPERTY(QString cameraPipeErrorString READ cameraPipeErrorString NOTIFY cameraPipeErrorChanged)
    Q_PROPERTY(QString robotPipesPath READ robotPipesPath WRITE setRobotPipesPath NOTIFY robotPipesPathChanged)
    Q_PROPERTY(bool robotsConnected READ robotsConnected NOTIFY robotPipesChanged)
    Q_PROPERTY(QList<QObject*> robotCommandPipes READ robotCommandPipesQObject NOTIFY robotPipesChanged)
    Q_PROPERTY(QList<QObject*> robotCameraPipes READ robotCameraPipesQObject NOTIFY robotPipesChanged)

public:
    explicit PipeController(QObject* parent = nullptr);
    virtual ~PipeController() override;

    QString cameraPipePath() const;
    void setCameraPipePath(QString newPath);
    bool cameraPipeHasError() const;
    QString cameraPipeErrorString() const;

    QString robotPipesPath() const;
    void setRobotPipesPath(QString newPath);

    bool robotsConnected() const;
    QList<RobotCommandPipe*> robotCommandPipes() const;
    QList<QObject*> robotCommandPipesQObject() const;
    QList<RobotCameraPipe*> robotCameraPipes() const;
    QList<QObject*> robotCameraPipesQObject() const;

public slots:
    void sendCameraMessage(QByteArray serializedMarkers);
    void addRobot(Robot* r);
    void removeRobot(Robot* r);

signals:
    void cameraPipePathChanged(QString newPath);
    void cameraPipeErrorChanged();
    void robotPipesPathChanged(QString newPath);
    void robotPipesChanged();

private:
    RobotCommandPipe* createRobotCommandPipe(Robot* robot);
    RobotCameraPipe* createRobotCameraPipe(Robot* robot);
    void receiveCommandsOnRobotPipes();

private:
    struct Data;
    QScopedPointer<Data> _d;
};
