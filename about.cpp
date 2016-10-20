#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
	QDialog(parent),
  ui(new Ui::About)//-----------------------
{
  ui->setupUi(this);
  this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
}
About::~About()
{
	delete ui;
}
