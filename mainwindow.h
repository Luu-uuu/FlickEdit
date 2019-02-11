#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "customscene.h"
#include "levelloader.h"
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void populateScene();
    ~MainWindow();
    void loadTiles();

private:
    Ui::MainWindow *ui;
    customScene *scene;
    QGraphicsPixmapItem *backImg;
    levelLoader *level;

private slots:
    void slot_gridCheck();
    void slot_openFile();
    void slot_saveFile();

};

#endif // MAINWINDOW_H
