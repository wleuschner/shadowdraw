#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include "seamcarving.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //drawArea= new DrawWidget(this);

    //this->setCentralWidget(drawArea);
    this->unifiedTitleAndToolBarOnMac();
    ui->menuBar->setNativeMenuBar(false);
    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openAction()));
    connect(ui->actionRemove_Seam,SIGNAL(triggered()),SLOT(removeSeamAction()));
    connect(ui->actionShow_Energy_Distribution, SIGNAL(triggered()), &ed_view, SLOT(show()));
    connect(this, SIGNAL(sendEnergyDest(QImage&)), &ed_view, SLOT(receiveEnergyDist(QImage&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openAction(){
    QFileDialog fd(this, "Open Image", "", "Image(*.jpg *.bmp *.tif *.png)");
    fd.setFileMode(QFileDialog::ExistingFile);
    if(fd.exec()){
        image = QImage(fd.selectedFiles()[0]);
        qDebug() << "Path: " << fd.selectedFiles()[0];
        this->resize(image.size());
        ui->ImageViewer->setPixmap(QPixmap::fromImage(image));
        emit sendEnergyDest(sc.energyDist);
    }
}

void MainWindow::removeSeamAction()
{
    SeamCarving sc(image);
    sc.removeSeamV();
    emit sendEnergyDest(sc.energyDist);
    ui->ImageViewer->setPixmap(QPixmap::fromImage(image));

}

void MainWindow::resizeEvent(QResizeEvent *event){
    SeamCarving sc(image);
    int delta = event->oldSize().width()- event->size().width();
    if (delta > 0){
        for (int i = 0; i < delta; i++){
            sc.removeSeamV();
        }
    }
    ui->ImageViewer->setPixmap(QPixmap::fromImage(image));
    emit sendEnergyDest(sc.energyDist);
}
