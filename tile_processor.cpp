#include "qpainter.h"
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

Tile::Tile(const QPixmap& pixmap, int x, int y, int size)
    : m_pixmap(pixmap), m_size(size) {
    setPos(x * size, y * size);
}

QRectF Tile::boundingRect() const {
    return QRectF(0, 0, m_size, m_size);
}

void Tile::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->drawPixmap(0, 0, m_pixmap);
}

class TileProcessor {
public:
    TileProcessor(const QString& tilesetPath, int tileSize);
    Tile* createTile(int tileIndex, int x, int y);

private:
    QPixmap m_tileset;
    int m_tileSize;
    QMap<int, QPixmap> m_tileCache;
};

TileProcessor::TileProcessor(const QString& tilesetPath, int tileSize)
    : m_tileSize(tileSize) {
    m_tileset.load(tilesetPath);
}

Tile* TileProcessor::createTile(int tileIndex, int x, int y) {
    if (!m_tileCache.contains(tileIndex)) {
        int columns = m_tileset.width() / m_tileSize;
        int row = tileIndex / columns;
        int col = tileIndex % columns;
        QPixmap tilePixmap = m_tileset.copy(col * m_tileSize, row * m_tileSize, m_tileSize, m_tileSize);
        m_tileCache.insert(tileIndex, tilePixmap);
    }
    return new Tile(m_tileCache[tileIndex], x, y, m_tileSize);
}
