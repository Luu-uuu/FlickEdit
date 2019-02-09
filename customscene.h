#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H
#include <QGraphicsScene>
//#include <QVector>

class customScene : public QGraphicsScene
{
public:
    customScene(QObject * parent = nullptr);
    QRectF backItemRect;

//private:
    bool showGrid;
protected:
    void drawForeground(QPainter * painter, const QRectF & rect) override;
};

#endif // CUSTOMSCENE_H
