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

class Robot;
class QLocalServer;
class QLocalSocket;

class RobotSocket : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(bool hasError READ hasError NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorChanged)

public:
    explicit RobotSocket(Robot& robot, QObject* parent = nullptr);
    virtual ~RobotSocket() override;

    Robot* robot() const;

    QString path() const;
    bool hasError() const;
    QString errorString() const;

public slots:
    bool send(QByteArray data);

signals:
    void received(QByteArray data);
    void errorChanged();

private:
    void setErrorString(QString s);
    void onNewConnection();
    void onSocketDisconnected(QLocalSocket* s);

private:
    Robot& _robot;
    QLocalServer& _server;
    QList<QLocalSocket*> _connections;
    QString _errorString;
};
