#ifndef LEVELTILE_H
#define LEVELTILE_H
#include <QGraphicsPixmapItem>

enum dataType {
    PLATFORMS,
    EXIT_DOOR,
    CATFLAP,
    SPECIAL_BG_DECOR,
    BG_DECOR,
    LARGE_BG_DECOR,
    WEAPONS,
    CHIRPS,
    COOL_CHIRPS,
    EMPTY
};


class levelTile: public QGraphicsPixmapItem{
public:
    levelTile();
    levelTile(dataType);
    levelTile(dataType,int x, int y);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    dataType getTileType();
private:
    int x;
    int y;
    dataType typeOfTile;

    QRectF boundRect;
    QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;
    QPainterPath shape() const override;
};

#endif // LEVELTILE_H
