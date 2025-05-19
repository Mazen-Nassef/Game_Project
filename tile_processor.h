#ifndef TILE_PROCESSOR_H
#define TILE_PROCESSOR_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QMap>
#include <QString>

class Tile : public QGraphicsItem {
public:
    Tile(const QPixmap& pixmap, int x, int y, int size);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QPixmap m_pixmap;
    int m_size;
};

class TileProcessor {
public:
    TileProcessor(const QString& tilesetPath, int tileSize);
    Tile* createTile(int tileIndex, int x, int y);

private:
    QPixmap m_tileset;
    int m_tileSize;
    QMap<int, QPixmap> m_tileCache;
};

#endif // TILE_PROCESSOR_H
