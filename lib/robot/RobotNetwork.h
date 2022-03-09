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
#include "IRobotManager.h"

class RobotNetwork : public IRobotManager {
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString connectionError READ connectionError NOTIFY connectedChanged)
public:
    explicit RobotNetwork(QObject* parent = nullptr);
    virtual ~RobotNetwork();

    bool connected() const;
    QString connectionError() const;

    virtual Robot* robot(QByteArray id) const override;
    virtual QList<Robot*> robots() const override;
    virtual int count() const override;

signals:
    void connectedChanged(bool connected);

private:
    void setConnected(bool connected, QString errorString);

    void readListenerSocket();
    void discoverRobots();

private:
    struct Data;
    QScopedPointer<Data> _d;
};
