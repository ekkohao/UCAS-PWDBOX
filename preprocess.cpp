#include "preprocess.h"
#include "ui_preprocess.h"
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <QDebug>

PreProcess::PreProcess(QWidget *parent):
	QDialog(parent),
	ui(new Ui::PreProcess)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
}

PreProcess::~PreProcess()
{
	delete ui;
}

void PreProcess::on_buttonOpenFile_clicked()
{
	//获取将要打开的文件名

	QString strSrcName = QFileDialog::getOpenFileName(this,"打开文件", ".","Text files(*.txt);;All files(*)");

	if( strSrcName.isEmpty() )
	{
		ui->lineEditSrcFile->setText("");
		return;
	}
	else
	{
		//设置源文件名
		ui->lineEditSrcFile->setText(strSrcName);

	}
	if(strSrcName.indexOf("163")>-1)
		ui->comboBoxMode->setCurrentIndex(0);
	else if(strSrcName.indexOf("linkedin")>-1)
		ui->comboBoxMode->setCurrentIndex(1);
	else if(strSrcName.indexOf("yahoo")>-1)
		ui->comboBoxMode->setCurrentIndex(2);
}

void PreProcess::on_buttonProcess_clicked()
{
	QString strFileIn=ui->lineEditSrcFile->text();
	QString strFileOut=strFileIn.left(strFileIn.lastIndexOf(".txt"))+"_out.txt";
	QFile fileIn(strFileIn);
	QFile fileOut(strFileOut);

	//判断是否正确打开

	if( ! fileIn.open( QIODevice::ReadOnly ) )
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + fileIn.errorString());
		return; //不处理文件

	}
	if(! fileOut.open( QIODevice::WriteOnly ) )
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + fileOut.errorString());
		return; //不处理文件
	}
	int i=0;
	int err=0;
	int index;
	int index2;

	ui->buttonProcess->setEnabled(false);
	repaint();	//重绘使按钮变灰

	//读取并解析文件
	while( ! fileIn.atEnd() )

	{
		//读取一行

		QString baLine = fileIn.readLine();

		baLine = baLine.trimmed();  //剔除字符串两端空白

		//判断是否为注释行
		QString sep=ui->lineEditSep->text();
		index=baLine.indexOf(sep);
		index2=baLine.indexOf(sep,index+sep.size());

		//如果是yahoo模式，特殊处理
		if(ui->comboBoxMode->currentIndex()==2)
		{
			index=index2;
			index2=baLine.indexOf(sep,index+sep.size());
		}

		if( index < 0 || index2 >= 0 )
		{
			ui->textResult->append(baLine);
			err++;
			continue;   //不处理没有分隔符或有两个和两个以上分割符的行
		}
		fileOut.write(baLine.remove(0,index+sep.size()).toLocal8Bit()+"\r\n");
		//ui->textResult->append(baLine);

		i++;


	}
	fileIn.close();
	fileOut.close();

	ui->buttonProcess->setEnabled(true);
	ui->textResult->append(QString("成功完成预处理总结果数：%1,失败数：%2\r\n").arg(QString::number(i),QString::number(err)));

	//提示加载完毕
	QMessageBox::information(this, "处理结果", "数据预处理完毕\r\n处理后文件保存在："+strFileOut);
}

void PreProcess::on_comboBoxMode_currentIndexChanged(int index)
{
	switch(index)
	{
		case 0:
			ui->lineEditSep->setText("----");
			ui->lineEditSep->setReadOnly(true);
			break;
		case 1:
			ui->lineEditSep->setText(":");
			ui->lineEditSep->setReadOnly(true);
			break;
		case 2:
			ui->lineEditSep->setText(":");
			ui->lineEditSep->setReadOnly(true);
			break;
		default:
			ui->lineEditSep->setReadOnly(false);
			break;
	}
}

void PreProcess::on_buttonClear_clicked()
{
	ui->textResult->clear();
}
