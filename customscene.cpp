#include "customscene.h"
#include <QPainter>

customScene::customScene(QObject * parent):
    QGraphicsScene(parent), showGrid(false){
}

void customScene::drawForeground(QPainter *painter, const QRectF & rect){
    Q_UNUSED(rect);
    if(showGrid)
    {
        int gridInterval = 16;
        QRectF rect2=backItemRect;
        qreal left = int(rect2.left()) - (int(rect2.left()) % gridInterval);
        qreal top = int(rect2.top()) - (int(rect2.top()) % gridInterval);

        QVarLengthArray<QLineF, 100> linesX;
        for (qreal x = left; x <= rect2.right(); x += gridInterval )
            linesX.append(QLineF(x, rect2.top()+32, x, rect2.bottom()-32));

        QVarLengthArray<QLineF, 100> linesY;
        for (qreal y = top+32; y <= rect2.bottom()-32; y += gridInterval )
                linesY.append(QLineF(rect2.left(), y, rect2.right(), y));

        painter->drawLines(linesX.data(), linesX.size());
        painter->drawLines(linesY.data(), linesY.size());
    }
}
