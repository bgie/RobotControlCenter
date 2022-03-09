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
#include <QRectF>

class WorldEdge;

class GameScene : public QObject {
    Q_OBJECT
    Q_PROPERTY(QRectF bounds READ bounds NOTIFY boundsChanged)

public:
    explicit GameScene(QObject* parent = nullptr);
    virtual ~GameScene() override;

    QRectF bounds() const;
    WorldEdge& worldEdge() const;

signals:
    void boundsChanged();

private:
    struct Data;
    QScopedPointer<Data> _d;
};
