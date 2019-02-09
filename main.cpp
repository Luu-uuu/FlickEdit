#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow window;
    window.show();

    return app.exec();
}



//char MSB 7 bit determines if solid
//2nd MSB 6 bit if vertical or horiz
//5-0 bit determine amount of blocks 

