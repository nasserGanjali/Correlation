#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    test();
}

void MainWindow::test()
{

    float aaa;
    uint16_t imax[1],jmax[1];
    //    uint8_t  *mattest = new uint8_t[10000];
    //    uint8_t  *matobj = new uint8_t[1000];
    uint8_t  mattest[10000];
    uint8_t  matobj[1000];

    float *Rcorwindow = new float[10000];

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    for(int i = 0 ; i < 100 ; i++)
        for(int j = 0 ; j < 100 ; j++)
        {
            mattest[i*100+j] = (qrand())%100;
            qsrand(qrand());
        }
    for(int i = 0 ; i < 30 ; i++)
        for(int j = 0 ; j < 30 ; j++)
            matobj[i*30+j] = (qrand())%255;
    for(int i = 0 ; i < 10000 ; i++)
        Rcorwindow[i] = -1;

    FCorr *Corr = new FCorr();

    qDebug()<<Corr->Corr(Rcorwindow ,imax,jmax,mattest ,matobj ,100,100,100,30,30,30);

}

MainWindow::~MainWindow()
{
    delete ui;
}
