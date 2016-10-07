#include "dataanalysis.h"
#include "ui_dataanalysis.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QHash>

#include <QDebug>

DataAnalysis::DataAnalysis(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DataAnalysis)
{
	ui->setupUi(this);
}

DataAnalysis::~DataAnalysis()
{
	delete ui;
}

void DataAnalysis::on_buttonOpenFile_clicked()
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
}

void DataAnalysis::on_buttonAnalysis_clicked()
{
	int i=ui->comboBoxAnalysisType->currentIndex();

	ui->buttonAnalysis->setEnabled(false);
	repaint();	//重绘使按钮变灰

	switch ( i )
	{
		case 0:
			composition_ana();
			break;
		case 1:
			keyboard_ana();
			break;
		case 2:
			date_ana();
			break;
		case 3:
			phonetic_top10_anay();
			break;
		case 4:
			word_top10_ana();
			break;
		default:
			break;
	}

	ui->buttonAnalysis->setEnabled(true);
}

//成分分析
void DataAnalysis::composition_ana()
{
	QString strFile=ui->lineEditSrcFile->text();
	QFile fileAna(strFile);
	if(!fileAna.open(QIODevice::ReadOnly ))
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + fileAna.errorString());
		return; //不处理文件
	}

	QString strLine;	//文件路径
	QHash<QString,int> hash;	//类型-个数 哈希表
	QString hash_key;	//哈希表的key
	QChar key_type;	//当前字符类型
	int key_type_count=0;	//当前字符类型计数
	int i,sum=0;

	while(!fileAna.atEnd())
	{


		strLine=fileAna.readLine().trimmed();

		hash_key="";
		key_type_count=0;
		key_type=QChar('E');
		for(i=0;i<strLine.size();i++)
		{
			if(strLine.at(i).isDigit())
			{
				if(key_type==QChar('N'))
					key_type_count++;
				else
				{
					if(key_type_count!=0)
						hash_key+=(key_type+QString::number(key_type_count));
					key_type=QChar('N');
					key_type_count=1;
				}
			}
			else if( ( strLine.at(i).isLetter() ))
			{
				if(key_type==QChar('A'))
					key_type_count++;
				else
				{
					if(key_type_count!=0)
						hash_key+=(key_type+QString::number(key_type_count));
					key_type=QChar('A');
					key_type_count=1;
				}
			}
			else
			{
				if(key_type==QChar('S'))
					key_type_count++;
				else
				{
					if(key_type_count!=0)
						hash_key+=(key_type+QString::number(key_type_count));
					key_type=QChar('S');
					key_type_count=1;
				}
			}
		}

		hash_key+=(key_type+QString::number(key_type_count));

		if(hash.contains(hash_key))
			hash[hash_key]++;
		else
			hash[hash_key]=1;
		//qDebug()<<strLine <<" -> "<<hash_key;
		sum++;

	}
	fileAna.close();

	QString text;

	text=QString("总数：%1\r\n").arg(sum);

	QHash<int,QString> sort_hash;

	hash_sort_insert(sort_hash,hash);

	for(i=0;i<sort_hash.size();i++)
	{
		text+=(sort_hash[i]+" -> "+QString::number(hash[sort_hash[i]])+"\r\n");
	}

	ui->textResult->append(text);
}

//键盘密码构成分析
void DataAnalysis::keyboard_ana()
{
	QString strFile=ui->lineEditSrcFile->text();
	QFile fileAna(strFile);

	if(!fileAna.open(QIODevice::ReadOnly ))
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + fileAna.errorString());
		return; //不处理文件
	}

	QString strLine;

	int i,ii,jj,ipre,jpre,sum;
	bool is_k=true;
	QChar c;
	QHash<QString,int> hash;
	QString temple;

	while(!fileAna.atEnd())
	{
		strLine=fileAna.readLine().trimmed();
		for(i=0,ii=-1,jj=0,ipre=-1,jpre=0,sum=0,is_k=true,temple="";i<strLine.size();i++)
		{
			c=strLine.at(i);
			if(c.isLetter()){
				temple+=c;
				ipre=ii;
				jpre=jj;
				sum++;
				find_keyboard_loc(c,ii,jj);
				if(ipre==-1)
					continue;
				if((qAbs<int>(ii-ipre)>1)||(qAbs<int>(jj-jpre)>1)){	//键盘位置不相邻
					is_k=false;
					break;
				}
			}
		}
		if(is_k&&sum>2)	//必须含有三个字母才判别为键盘模式
		{
			//strLine=temple; //不输出模式则注销此行
			if(hash.contains(strLine))
				hash[strLine]++;
			else
				hash.insert(strLine,1);
		}
	}
	fileAna.close();

	QHash<int,QString> sort_hash;

	QString text;

	hash_sort_insert(sort_hash,hash);

	for(i=0;i<sort_hash.size();i++)
	{
		text+=(sort_hash[i]+" -> "+QString::number(hash[sort_hash[i]])+"\r\n");
	}

	ui->textResult->append(text);
}

//日期密码构成分析
void DataAnalysis::date_ana()
{
	QString strFile=ui->lineEditSrcFile->text();
	QFile fileAna(strFile);
	if(!fileAna.open(QIODevice::ReadOnly ))
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + fileAna.errorString());
		return; //不处理文件
	}
	QString strLine;
	while(!fileAna.atEnd())
	{
		strLine=fileAna.readLine().trimmed();

	}
	fileAna.close();
}

//拼音top10
void DataAnalysis::phonetic_top10_anay()
{
	QString strFile=ui->lineEditSrcFile->text();
	QFile fileAna(strFile);
	if(!fileAna.open(QIODevice::ReadOnly ))
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + fileAna.errorString());
		return; //不处理文件
	}
	QString strLine;
	while(!fileAna.atEnd())
	{
		strLine=fileAna.readLine().trimmed();

	}
	fileAna.close();
}

//英文单词top10
void DataAnalysis::word_top10_ana()
{
	QString strFile=ui->lineEditSrcFile->text();
	QFile fileAna(strFile);
	if(!fileAna.open(QIODevice::ReadOnly ))
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + fileAna.errorString());
		return; //不处理文件
	}
	QString strLine;
	while(!fileAna.atEnd())
	{
		strLine=fileAna.readLine().trimmed();

	}
}

void DataAnalysis::hash_sort_insert(QHash<int,QString> &sort_hash,QHash<QString,int> &source_hash)
{
	int i=0,j=0;
	QString t;

	QHashIterator<QString, int> ind(source_hash);

	while (ind.hasNext()) {
		 ind.next();
		 sort_hash[i]=ind.key();
		 for(j=i;j>0;j--)
		 {
			 if(source_hash[sort_hash[j]]>source_hash[sort_hash[j-1]])
			 {
				 t=sort_hash[j];
				 sort_hash[j]=sort_hash[j-1];
				 sort_hash[j-1]=t;
			 }
		 }
		 i++;
	 }
}

void DataAnalysis::find_keyboard_loc(QChar &c,int &i,int &j)
{
	c=c.toLower();
	switch(c.toLatin1())
	{
		case 'q':i=0;j=0;break;
		case 'w':i=0;j=1;break;
		case 'e':i=0;j=2;break;
		case 'r':i=0;j=3;break;
		case 't':i=0;j=4;break;
		case 'y':i=0;j=5;break;
		case 'u':i=0;j=6;break;
		case 'i':i=0;j=7;break;
		case 'o':i=0;j=8;break;
		case 'p':i=0;j=9;break;
		case 'a':i=1;j=0;break;
		case 's':i=1;j=1;break;
		case 'd':i=1;j=2;break;
		case 'f':i=1;j=3;break;
		case 'g':i=1;j=4;break;
		case 'h':i=1;j=5;break;
		case 'j':i=1;j=6;break;
		case 'k':i=1;j=7;break;
		case 'l':i=1;j=8;break;
		case 'z':i=2;j=0;break;
		case 'x':i=2;j=1;break;
		case 'c':i=2;j=2;break;
		case 'v':i=2;j=3;break;
		case 'b':i=2;j=4;break;
		case 'n':i=2;j=5;break;
		case 'm':i=2;j=6;break;
		default:i=10;j=0;break;
	}
}

void DataAnalysis::on_buttonClear_clicked()
{
	ui->textResult->clear();
}
