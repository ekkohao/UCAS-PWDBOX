#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preprocess.h"
#include "dataanalysis.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->connect(ui->actionPreProcess,SIGNAL(triggered()),this,SLOT(slot_open_preprocess_ui()));
	this->connect(ui->actionAnalysis,SIGNAL(triggered()),this,SLOT(slot_open_dataanalysis_ui()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slot_open_preprocess_ui(){
	PreProcess pp;
	pp.exec();

}

void MainWindow::slot_open_dataanalysis_ui(){
	DataAnalysis da;
	da.exec();
}

