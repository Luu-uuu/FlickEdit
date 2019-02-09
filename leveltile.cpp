#include "leveltile.h"

levelTile::levelTile()
    :QGraphicsPixmapItem(){

}

levelTile::levelTile(dataType typeOfTile, int x, int y)
    :QGraphicsPixmapItem(){
    this->typeOfTile = typeOfTile;
    this->x = x;
    this->y = y;

}

levelTile::levelTile(dataType typeOfTile)
    :QGraphicsPixmapItem(){
    this->typeOfTile = typeOfTile;
    this->x = 0;
    this->y = 0;
    setPos(x, y);
    setFlags(ItemIsSelectable);
    //setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
    QImage tileImage;
    switch (typeOfTile){
        case PLATFORMS:
            tileImage=QImage(":/res/floor.png");
            tileImage=tileImage.scaled(tileImage.width()*2,tileImage.height()*2,Qt::KeepAspectRatio);
            boundRect = tileImage.rect();
            setPixmap(QPixmap::fromImage(tileImage));
            break;
        case EXIT_DOOR:
            break;
        case CATFLAP:
            break;
        case SPECIAL_BG_DECOR:
            break;
        case BG_DECOR:
            break;
        case LARGE_BG_DECOR:
            break;
        case WEAPONS:
            break;
        case CHIRPS:
            break;
        case COOL_CHIRPS:
            break;
        case EMPTY:
            //no image but still has a rect to click on
            //tileImage=QImage(":/res/empty.png");
            //tileImage=tileImage.scaled(tileImage.width()*2,tileImage.height()*2,Qt::KeepAspectRatio);
            boundRect=QRectF(0,0,16,16);//tileImage.rect();
            //setPixmap(QPixmap::fromImage(tileImage));
        break;
    }


}

QRectF levelTile::boundingRect() const{
    return boundRect;
}
void levelTile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    this->QGraphicsPixmapItem::paint(painter,option,widget);
}

QVariant levelTile::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange	)
    {
        if (value == true)
        {
            if(typeOfTile == EMPTY){
                typeOfTile = PLATFORMS;
                QImage tileImage=QImage(":/res/floor.png");
                tileImage=tileImage.scaled(tileImage.width()*2,tileImage.height()*2,Qt::KeepAspectRatio);
                boundRect=tileImage.rect();
                setPixmap(QPixmap::fromImage(tileImage));
            }else if(typeOfTile == PLATFORMS){
                typeOfTile = EMPTY;
                //this could be potentially be a memoryleak need to go through docs more
                setPixmap(QPixmap());

            }
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

dataType levelTile::getTileType(){
    return typeOfTile;
}

QPainterPath levelTile::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}
