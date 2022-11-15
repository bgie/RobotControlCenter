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

class IRobotManager;
class Robot;
class RobotSocket;

class RobotSocketManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool robotsConnected READ robotsConnected NOTIFY robotSocketChanged)
    Q_PROPERTY(QList<QObject*> robotSocket READ robotSocketQObject NOTIFY robotSocketChanged)

public:
    explicit RobotSocketManager(IRobotManager& robots, QObject* parent = nullptr);
    virtual ~RobotSocketManager() override;

    bool robotsConnected() const;
    QList<RobotSocket*> robotSocket() const;
    QList<QObject*> robotSocketQObject() const;

signals:
    void robotSocketChanged();
    void robotSocketAdded(RobotSocket* socket);
    void robotSocketRemoved(RobotSocket* socket);

private:
    void onRobotAdded(Robot* robot);
    void onRobotRemoved(Robot* robot);

private:
    struct Data;
    QScopedPointer<Data> _d;
};
