#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    level(new levelLoader)
{
    ui->setupUi(this);
    populateScene();

    connect(ui->action_Grid_Toggle, SIGNAL(triggered()), this, SLOT(slot_gridCheck()));
    connect(ui->action_Open,SIGNAL(triggered()), this, SLOT(slot_openFile()));
    connect(ui->action_Save,SIGNAL(triggered()), this, SLOT(slot_saveFile()));
}

void MainWindow::slot_gridCheck(){
    scene->showGrid = !scene->showGrid;
    scene->invalidate(scene->backItemRect,QGraphicsScene::ForegroundLayer);
    //Debug() << scene->showGrid;
}

void MainWindow::slot_openFile(){

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("Binary (*.bin)"));

    if(!fileName.isNull()){

        if(level->floorQItems[0][0])
        {
            scene->clear();
            for(int i=0; i<level->floorQItems.size(); i++){
                for (int k=0; k<level->floorQItems[0].size();k++)
                {
                    level->floorQItems[i][k]=nullptr;
                }
            }

            //since level editor does not support background editing yet we just load a default background png from level 1
            //this is also a bit ugly and will need to be refactored when background/pallete editing is added
            QImage levelBackgroundImg(":/res/blankLevel.png");
            levelBackgroundImg = levelBackgroundImg.scaled(levelBackgroundImg.width()*2,levelBackgroundImg.height()*2,Qt::KeepAspectRatio);
            backImg = new QGraphicsPixmapItem(QPixmap::fromImage(levelBackgroundImg));
            scene->addItem(backImg);

        }

        level->loadLevel(fileName.toLatin1().data());
        level->toQItems();

        //this should probably be done in a seperate function, perhaps in toQItems
        for (int i=0; i<24; i++){
            for(int k=0;k<32; k++){
                if(level->floorQItems[k][i]){
                    scene->addItem(level->floorQItems[k][i]);
                    level->floorQItems[k][i]->setPos(QPointF(k*16, i*16 + 32));
                }else{
                    //air tiles added
                    level->floorQItems[k][i] = new levelTile(EMPTY);
                    scene->addItem(level->floorQItems[k][i]);
                    level->floorQItems[k][i]->setPos(QPointF(k*16, i*16 + 32));

                }
            }
        }

    }

}

void MainWindow::slot_saveFile(){
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("Binary (*.bin)"));
    if(!fileName.isNull())
        level->saveLevel(fileName.toLatin1().data());
}

void MainWindow::populateScene(){
    scene = new customScene(this);
    QImage levelBackgroundImg(":/res/blankLevel.png");
    levelBackgroundImg= levelBackgroundImg.scaled(levelBackgroundImg.width()*2,levelBackgroundImg.height()*2,Qt::KeepAspectRatio);
    backImg = new QGraphicsPixmapItem(QPixmap::fromImage(levelBackgroundImg));
    scene->addItem(backImg);
    scene->backItemRect = backImg->boundingRect();

    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete level;
}
