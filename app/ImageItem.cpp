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
#include "ImageItem.h"
#include <QDebug>
#include <QImage>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>

ImageItem::ImageItem(QQuickItem* parent)
    : QQuickItem(parent)
    , _imageChanged(true)
    , _zoom(1.0f)
    , _offset(0.0f, 0.0f)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

ImageItem::~ImageItem()
{
}

QSGNode* ImageItem::updatePaintNode(QSGNode* oldNode, QQuickItem::UpdatePaintNodeData*)
{
    auto node = dynamic_cast<QSGSimpleTextureNode*>(oldNode);
    if (!node || _imageChanged) {
        if (!node) {
            node = new QSGSimpleTextureNode();
        }

        QSGTexture* texture = window()->createTextureFromImage(_image);
        node->setOwnsTexture(true);
        node->setRect(boundingRect());
        node->setTexture(texture);

        QSizeF boundRect = this->size();
        QSizeF fittingRect = QSizeF(_image.size()).scaled(boundRect, Qt::KeepAspectRatio);
        QPointF origin((boundRect.width() - fittingRect.width()) / 2, (boundRect.height() - fittingRect.height()) / 2);
        node->setRect(QRectF(origin, fittingRect));

        node->markDirty(QSGNode::DirtyForceUpdate);
        _imageChanged = false;
        _offset = origin;
    }
    return node;
}

void ImageItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    _imageChanged = true;
    update();
    updateTransform();
}

QImage ImageItem::image() const
{
    return _image;
}

bool ImageItem::hasImage() const
{
    return !_image.isNull();
}

float ImageItem::zoom() const
{
    return _zoom;
}

QPointF ImageItem::offset() const
{
    return _offset;
}

void ImageItem::setImage(QImage image)
{
    _image = image;
    _imageChanged = true;
    update();
    updateTransform();
    emit imageChanged(_image);
}

void ImageItem::updateTransform()
{
    QSizeF boundRect = this->size();
    QSizeF fittingRect = QSizeF(_image.size()).scaled(boundRect, Qt::KeepAspectRatio);
    _zoom = fittingRect.width() / _image.size().width();
    _offset = QPointF((boundRect.width() - fittingRect.width()) / 2, (boundRect.height() - fittingRect.height()) / 2);
    emit transformChanged();
}
