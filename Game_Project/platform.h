#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <QGraphicsItem>

class Platform : public QGraphicsRectItem 
{
public:
    enum class PlatformType {
        Solid,      // Cannot pass through from any direction
        Passthrough // Can pass through from below or when pressing down
    };
    
    Platform(qreal x, qreal y, qreal width, qreal height, 
             PlatformType type = PlatformType::Passthrough,
             QColor color = Qt::green,
             QGraphicsItem* parent = nullptr);
             
    // Getters
    PlatformType getType() const { return type; }
    bool isSolid() const { return type == PlatformType::Solid; }
    bool isPassthrough() const { return type == PlatformType::Passthrough; }
    
private:
    PlatformType type;
};

#endif // PLATFORM_H
