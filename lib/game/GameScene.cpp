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
#include "GameScene.h"
#include "MarkerSceneItem.h"
#include "WorldEdge.h"

struct GameScene::Data {
    Data()
        : screenRotation(0)
    {
    }

    WorldEdge edge;
    QVector<MarkerSceneItem> markers;
    int screenRotation;
};

GameScene::GameScene(QObject* parent)
    : QObject(parent)
    , _d(new Data())
{
    connect(&_d->edge, &WorldEdge::pointsChanged, this, &GameScene::boundsChanged);
}

GameScene::~GameScene()
{
}

QRectF GameScene::bounds() const
{
    return _d->edge.bounds();
}

WorldEdge& GameScene::worldEdge() const
{
    return _d->edge;
}

const QVector<MarkerSceneItem>& GameScene::markers() const
{
    return _d->markers;
}

void GameScene::setMarkers(const QVector<MarkerSceneItem>& markers)
{
    _d->markers = markers;
    emit markersChanged();
}

int GameScene::screenRotation() const
{
    return _d->screenRotation;
}

void GameScene::setScreenRotation(int screenRotation)
{
    if (_d->screenRotation == screenRotation)
        return;

    _d->screenRotation = screenRotation;
    emit screenRotationChanged();
}
