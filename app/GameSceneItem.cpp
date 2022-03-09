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
#include "aruco/MarkerList.h"
#include "game/GameScene.h"
#include "game/WorldEdge.h"
#include <QPainter>
#include <math.h>

const int WORLD_EDGE_POINT_SIZE = 10;
const int WORLD_EDGE_LINE_WIDTH = 5;
const QColor WORLD_EDGE_COLOR = QColor(Qt::yellow);
const int MARKER_CIRCLE_SIZE = 26;
const QColor MARKER_CIRCLE_LINE_COLOR(Qt::darkGreen);
const int MARKER_CIRCLE_LINE_WIDTH = 3;
const QColor MARKER_CIRCLE_FILL_COLOR(Qt::white);
const int MARKER_ANGLE_SIZE = 80;
const int MARKER_ANGLE_DEGREES = 60;
const QColor MARKER_ANGLE_FILL_COLOR("#70707070");
const int MARKER_UNFILTERED_CIRCLE_SIZE = 5;
const QColor MARKER_UNFILTERED_POS_COLOR("#7000FF00");
const int MARKER_ID_FONT_SIZE = 24;
const QColor MARKER_ID_COLOR(Qt::black);

GameSceneItem::GameSceneItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , _scene(nullptr)
{
}

void GameSceneItem::paint(QPainter* painter)
{
    if (_scene) {
        QRectF sceneRect = _scene->worldEdge().boundsWithMargin();

        qreal scale = qMin(width() / sceneRect.width(), height() / sceneRect.height());
        QSize screenSize(sceneRect.width() * scale, sceneRect.height() * scale);
        QPointF screenOrigin((width() - screenSize.width()) * 0.5, (height() - screenSize.height()) * 0.5);
        QPointF offset = screenOrigin - (sceneRect.topLeft() * scale);

        QPolygonF edgePoints;
        foreach (auto scenePoint, _scene->worldEdge().points()) {
            QPointF screenPoint = (scenePoint * scale) + offset;
            edgePoints << screenPoint;
            painter->fillRect(QRectF(screenPoint - QPointF(WORLD_EDGE_POINT_SIZE / 2, WORLD_EDGE_POINT_SIZE / 2), QSizeF(WORLD_EDGE_POINT_SIZE, WORLD_EDGE_POINT_SIZE)), WORLD_EDGE_COLOR);
        }
        if (edgePoints.size() > 2) {
            edgePoints.append(edgePoints.first());
        }
        if (edgePoints.size() > 1) {
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->setPen(QPen(WORLD_EDGE_COLOR, WORLD_EDGE_LINE_WIDTH));
            painter->drawPolyline(edgePoints);
        }

        QFont font;
        font.setPixelSize(MARKER_ID_FONT_SIZE);
        painter->setFont(font);

        foreach (const auto& m, _scene->markers()) {
            if (m.isDetectedFiltered()) {
                QPointF screenPoint = (m.filteredPos().toPointF() * scale) + offset;
                int angle = static_cast<int>(qRound(-m.filteredAngle() * 180 * 16 / M_PI));
                painter->setPen(Qt::NoPen);
                painter->setBrush(QBrush(MARKER_ANGLE_FILL_COLOR));
                QRectF pieRect(screenPoint.x() - MARKER_ANGLE_SIZE, screenPoint.y() - MARKER_ANGLE_SIZE, 2 * MARKER_ANGLE_SIZE, 2 * MARKER_ANGLE_SIZE);
                painter->drawPie(pieRect, angle - (MARKER_ANGLE_DEGREES * 8), MARKER_ANGLE_DEGREES * 16);

                painter->setPen(QPen(MARKER_CIRCLE_LINE_COLOR, MARKER_CIRCLE_LINE_WIDTH));
                painter->setBrush(QBrush(MARKER_CIRCLE_FILL_COLOR));
                painter->drawEllipse(screenPoint, MARKER_CIRCLE_SIZE, MARKER_CIRCLE_SIZE);

                painter->setPen(MARKER_ID_COLOR);
                painter->drawText(pieRect, Qt::AlignCenter, QString::number(m.id()));
            }
            if (m.isDetected()) {
                QPointF screenPoint = (m.pos().toPointF() * scale) + offset;
                painter->setPen(Qt::NoPen);
                painter->setBrush(QBrush(MARKER_UNFILTERED_POS_COLOR));
                painter->drawEllipse(screenPoint, MARKER_UNFILTERED_CIRCLE_SIZE, MARKER_UNFILTERED_CIRCLE_SIZE);
            }
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
        connect(_scene, &GameScene::markersChanged, this, &GameSceneItem::onSceneChanged);
    }
    update();

    emit sceneChanged(_scene);
}

void GameSceneItem::onSceneChanged()
{
    update();
}
