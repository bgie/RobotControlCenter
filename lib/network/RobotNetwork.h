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

#include <QList>
#include <QMap>
#include <QObject>

class Robot;

class RobotNetwork : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString connectionError READ connectionError NOTIFY connectedChanged)
    Q_PROPERTY(QList<QObject*> robots READ robotQObjects NOTIFY robotsChanged)
    Q_PROPERTY(int count READ count NOTIFY robotsChanged)
public:
    explicit RobotNetwork(QObject* parent = nullptr);
    virtual ~RobotNetwork();

    bool connected() const;
    QString connectionError() const;

    QList<Robot*> robots() const;
    QList<QObject*> robotQObjects() const;
    int count() const;

    QMap<QByteArray, int> robot2Marker() const;
    void setRobot2Marker(QMap<QByteArray, int> values);

signals:
    void connectedChanged(bool connected);
    void robotAdded(Robot* r);
    void robotRemoved(Robot* r);
    void robotsChanged();
    void robot2MarkerChanged(QMap<QByteArray, int> values);

private:
    void setConnected(bool connected, QString errorString);
    void onRobotMarkerIdChanged(int newId);

    void readListenerSocket();
    void discoverRobots();

private:
    struct Data;
    QScopedPointer<Data> _d;
};
