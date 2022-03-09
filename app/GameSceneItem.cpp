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
#include "GameSceneItem.h"
#include "game/GameScene.h"
#include "game/WorldEdge.h"
#include <QPainter>

GameSceneItem::GameSceneItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , _scene(nullptr)
{
}

void GameSceneItem::paint(QPainter* painter)
{
    QRectF screenRect(0, 0, width(), height());
    painter->fillRect(screenRect, QColor(Qt::red));

    if (_scene) {
        QRectF sceneRect = _scene->bounds();

        qreal scale = qMin(screenRect.width() / sceneRect.width(), screenRect.height() / sceneRect.height());
        QPointF offset = sceneRect.topLeft();
        // todo - center align

        foreach (auto scenePoint, _scene->worldEdge().points()) {
            QPointF screenPoint = (scenePoint - offset) * scale;
            painter->fillRect(QRectF(screenPoint - QPointF(5, 5), QSizeF(10, 10)), QColor(Qt::blue));
        }
    }
}

GameScene* GameSceneItem::scene() const
{
    return _scene;
}

void GameSceneItem::setScene(GameScene* scene)
{
    if (_scene == scene)
        return;

    if (_scene) {
        _scene->disconnect(this);
    }

    _scene = scene;

    if (_scene) {
        connect(_scene, &GameScene::boundsChanged, this, &GameSceneItem::onSceneChanged);
    }
    update();

    emit sceneChanged(_scene);
}

void GameSceneItem::onSceneChanged()
{
    update();
}
