#include "platform.h"
#include <QPen>

Platform::Platform(qreal x, qreal y, qreal width, qreal height, 
                   PlatformType type, QColor color, QGraphicsItem* parent)
    : QGraphicsRectItem(x, y, width, height, parent), type(type)
{
    // Set the platform's appearance
    setBrush(QBrush(color));
    setPen(QPen(Qt::NoPen)); // No border
}
