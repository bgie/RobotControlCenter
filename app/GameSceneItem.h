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
#include <QQuickPaintedItem>

class GameScene;

class GameSceneItem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(GameScene* scene READ scene WRITE setScene NOTIFY sceneChanged)

public:
    explicit GameSceneItem(QQuickItem* parent = nullptr);

    virtual void paint(QPainter* painter) override;

    GameScene* scene() const;
    void setScene(GameScene* scene);

signals:
    void sceneChanged(GameScene* scene);

private:
    void onSceneChanged();

private:
    GameScene* _scene;
};
