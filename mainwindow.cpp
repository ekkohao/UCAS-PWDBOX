#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preprocess.h"
#include "dataanalysis.h"
#include "about.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QTime>


#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->connect(ui->actionPreProcess,SIGNAL(triggered()),this,SLOT(slot_open_preprocess_ui()));
	this->connect(ui->actionAnalysis,SIGNAL(triggered()),this,SLOT(slot_open_dataanalysis_ui()));

	rand_seed=0;

	QDir dir;
	QString path=dir.currentPath();
	ui->lineDic->setText(path+"/savepwd/");

	widget_init();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::widget_init()
{
	ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
	//ui->tableWidget->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Stretch);
	ui->tableWidget->setRowCount(10);
	//ui->tableWidget->setHorizontalHeaderLabels(headers);//设置标题
	//ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString("1")));//行索引和列索引
	//第一列
	ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString("N6-11[56%]")));
	ui->tableWidget->setItem(1, 0, new QTableWidgetItem(QString("A6-10[8%]")));
	ui->tableWidget->setItem(2, 0, new QTableWidgetItem(QString("A1N7[1.1%]")));
	ui->tableWidget->setItem(3, 0, new QTableWidgetItem(QString("A2N6[1.5%]")));
	ui->tableWidget->setItem(4, 0, new QTableWidgetItem(QString("A2N7[1.4%]")));
	ui->tableWidget->setItem(5, 0, new QTableWidgetItem(QString("A3N6[1.9%]")));
	ui->tableWidget->setItem(6, 0, new QTableWidgetItem(QString("A3N7[1.3%]")));
	ui->tableWidget->setItem(7, 0, new QTableWidgetItem(QString("A2N8[0.9%]")));
	ui->tableWidget->setItem(8, 0, new QTableWidgetItem(QString("A4N4[0.9%]")));
	ui->tableWidget->setItem(9, 0, new QTableWidgetItem(QString("A6N3[0.8%]")));
	//第二列
	ui->tableWidget->setItem(0, 1, new QTableWidgetItem(QString("asd[2.3‰]")));
	ui->tableWidget->setItem(1, 1, new QTableWidgetItem(QString("qwe[2.0‰]")));
	ui->tableWidget->setItem(2, 1, new QTableWidgetItem(QString("qaz[0.9‰]")));
	ui->tableWidget->setItem(3, 1, new QTableWidgetItem(QString("zxc[0.9‰]")));
	ui->tableWidget->setItem(4, 1, new QTableWidgetItem(QString("qwer[0.8‰]")));
	ui->tableWidget->setItem(5, 1, new QTableWidgetItem(QString("asdf[0.5‰]")));
	ui->tableWidget->setItem(6, 1, new QTableWidgetItem(QString("zxcvbnm[0.4‰]")));
	ui->tableWidget->setItem(7, 1, new QTableWidgetItem(QString("qwert[0.3‰]")));
	ui->tableWidget->setItem(8, 1, new QTableWidgetItem(QString("qwerty[0.2‰]")));
	ui->tableWidget->setItem(9, 1, new QTableWidgetItem(QString("zxcv[0.2‰]")));
	//第三列
	ui->tableWidget->setItem(0, 2, new QTableWidgetItem(QString("YYYYMMDD[5.07%]")));
	ui->tableWidget->setItem(1, 2, new QTableWidgetItem(QString("MMDDYY[4.56%]")));
	ui->tableWidget->setItem(2, 2, new QTableWidgetItem(QString("AAAYYMMDD[0.66%")));
	ui->tableWidget->setItem(3, 2, new QTableWidgetItem(QString("AAYYMMDD[0.45%]")));
	ui->tableWidget->setItem(4, 2, new QTableWidgetItem(QString("AAYYYYMMDD[0.44%]")));
	ui->tableWidget->setItem(5, 2, new QTableWidgetItem(QString("DDMMYY[0.39%]")));
	ui->tableWidget->setItem(6, 2, new QTableWidgetItem(QString("AAAYYYYMMDD[0.39%]")));
	ui->tableWidget->setItem(7, 2, new QTableWidgetItem(QString("AAAAMMDD[0.23%]")));
	ui->tableWidget->setItem(8, 2, new QTableWidgetItem(QString("AAAAYYMMDD[0.22%]")));
	ui->tableWidget->setItem(9, 2, new QTableWidgetItem(QString("AAAAAAMMDD[0.21%]")));
	//第四列
	ui->tableWidget->setItem(0, 3, new QTableWidgetItem(QString("wang[7.8‰]")));
	ui->tableWidget->setItem(1, 3, new QTableWidgetItem(QString("xiao[7.2‰]")));
	ui->tableWidget->setItem(2, 3, new QTableWidgetItem(QString("zhang[5.9‰]")));
	ui->tableWidget->setItem(3, 3, new QTableWidgetItem(QString("yang[5.3‰]")));
	ui->tableWidget->setItem(4, 3, new QTableWidgetItem(QString("chen[4.2‰]")));
	ui->tableWidget->setItem(5, 3, new QTableWidgetItem(QString("wo[4.0‰]")));
	ui->tableWidget->setItem(6, 3, new QTableWidgetItem(QString("jian[3.2‰]")));
	ui->tableWidget->setItem(7, 3, new QTableWidgetItem(QString("feng[3.1‰]")));
	ui->tableWidget->setItem(8, 3, new QTableWidgetItem(QString("shi[2.7‰]")));
	ui->tableWidget->setItem(9, 3, new QTableWidgetItem(QString("ai[2.6‰]")));
	//第五列
	ui->tableWidget->setItem(0, 4, new QTableWidgetItem(QString("love[4.4‰]")));
	ui->tableWidget->setItem(1, 4, new QTableWidgetItem(QString("bin[2.5‰]")));
	ui->tableWidget->setItem(2, 4, new QTableWidgetItem(QString("she[1.9‰]")));
	ui->tableWidget->setItem(3, 4, new QTableWidgetItem(QString("you[1.9‰]")));
	ui->tableWidget->setItem(4, 4, new QTableWidgetItem(QString("pen[1.1‰]")));
	ui->tableWidget->setItem(5, 4, new QTableWidgetItem(QString("fan[1.1‰]")));
	ui->tableWidget->setItem(6, 4, new QTableWidgetItem(QString("men[1.1‰]")));
	ui->tableWidget->setItem(7, 4, new QTableWidgetItem(QString("song[1.0‰]")));
	ui->tableWidget->setItem(8, 4, new QTableWidgetItem(QString("any[1.0‰]")));
	ui->tableWidget->setItem(9, 4, new QTableWidgetItem(QString("king[0.7‰]")));

	ui->tableWidget->resizeRowToContents(0);
}

void MainWindow::save(QStringList &strList)
{
	QString strDic=ui->lineDic->text();
	QString saveName=strDic+"pwd.txt";
	int ii=1;
	QFile *pfileOut=new QFile(saveName);
	while(pfileOut->exists()){
		pfileOut->close();
		saveName=QString("%1pwd_%2.txt").arg(strDic,QString::number(ii++));
		delete pfileOut;
		pfileOut=new QFile(saveName);
	}
	if(!pfileOut->open(QIODevice::WriteOnly ))
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + pfileOut->errorString());
		return; //不处理文件
	}

	int i;
	QTextStream outStream(pfileOut);

	for(i=0;i<strList.size();i++)
	{
		outStream<<strList.at(i)<<"\r\n";
	}
	pfileOut->close();
	delete pfileOut;
	QMessageBox::information(this, "保存完成", "生成文件已保存在"+saveName);
}

QString MainWindow::random_qchar_in_qstring(QString qstr, int len)
{
	QString str = QString();

	QTime t;
	rand_seed+=3;
	if(rand_seed>2100000000)
		rand_seed=0;
	t= QTime::currentTime();
	qsrand(t.msec()+t.second()*1000+rand_seed);
	int ir;
	for(int i=0;i<len;i++)
	{
		ir = qrand()%qstr.length();
		str[i] = qstr.at(ir);
	}
	return str;
}

void MainWindow::produce_weak(QStringList &strList,int total)
{
	QString str=QString();
	int t;
	QStringList qstrListType=(QStringList()<<"A6"
							  <<"A7"
							  <<"A9"
							  <<"N6"
							  <<"N7"
							  <<"N8"
							  <<"N9"
							  <<"N10"
							  <<"N11"
							  <<"A3N6"
							  <<"A2N6"
							  <<"A2N7"
							  <<"A3N7"
							  <<"A1N7"
							  <<"A6N2"
							  <<"A5N2");
	QString qstrA="QWERTYUIOPLKJHGFDSAZXCVBNMmnbvcxzasdfghjklpoiuytrewq";
	QString qstrN="0123456789";

	int rand;
	QString type;
	QString strNum;
	int fail_count=0;
	int iAll=total;

	int iStep=0;
	ui->progressBar->setValue(0);

	QTime tt;
	tt= QTime::currentTime();
	qsrand(tt.msec()+tt.second()*1000);

	while(total>0&&fail_count<1000)
	{
		str=QString();
		rand=qrand()%qstrListType.size();
		type=qstrListType.at(rand);
		if(type.size()<4)
		{
			strNum=type.mid(1,type.size()-1);
			if(type[0]==QChar('A'))
				str+=random_qchar_in_qstring(qstrA,strNum.toInt());
			else if(type[0]==QChar('N'))
				str+=random_qchar_in_qstring(qstrN,strNum.toInt());
		}
		else
		{
			if(type[0]==QChar('A'))
			{
				t=type.indexOf("N");
				strNum=type.mid(1,t-1);
				str+=random_qchar_in_qstring(qstrA,strNum.toInt());
				strNum=type.mid(t+1);
				str+=random_qchar_in_qstring(qstrN,strNum.toInt());
			}
			else if(type[0]==QChar('N'))
			{
				t=type.indexOf("A");
				strNum=type.mid(1,t-1);
				str+=random_qchar_in_qstring(qstrN,strNum.toInt());
				strNum=type.mid(t+1);
				str+=random_qchar_in_qstring(qstrA,strNum.toInt());
			}

		}
		if(strList.contains(str))
			fail_count++;
		else
		{
			fail_count=0;
			strList.append(str);
			total--;
		}
		iStep++;
		if(iStep>=10){
			ui->progressBar->setValue((iAll-total)*100/iAll);
			wait();
			iStep=0;
		}
	}
	ui->progressBar->setValue(100);
}

void MainWindow::produce_kb(QStringList &strList, int total)
{
	QString str=QString();
	int t;
	QChar kb[][10]={{'q','w','e','r','t','y','u','i','o','p'},
				   {'a','s','d','f','g','h','j','k','l','0'},
				   {'z','x','c','v','b','n','m','0','0','0'}};

	QString qstrSeed;
	int i,j; //字母坐标
	int fail_count=0;
	int iAll=total;
	int iStep=0;

	QTime tt;
	tt= QTime::currentTime();
	qsrand(tt.msec()+tt.second()*1000);

	ui->progressBar->setValue(0);
	while(total>0&&fail_count<1000)//失败一千次直接跳出
	{
		str=QString();
		t=qrand()%13;
		switch(t)
		{
			case 0://"q1w1e1r1"
				qstrSeed=random_qchar_in_qstring(QString("qwertyuhgfdsazxcv"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6%7%8").arg(qstrSeed,QString::number(qrand()%10),
													 kb[i][j+1],QString::number(qrand()%10),
													 kb[i][j+2],QString::number(qrand()%10),
													 kb[i][j+3],QString::number(qrand()%10));
				break;
			case 1://"1q2w3e"
				qstrSeed=random_qchar_in_qstring(QString("qwertyuihgfdsazxcvjb"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6").arg(QString::number(qrand()%10),qstrSeed,
												 QString::number(qrand()%10),kb[i][j+1],
												 QString::number(qrand()%10),kb[i][j+2]);
				break;
			case 2://"123qwe"
				qstrSeed=random_qchar_in_qstring(QString("qwertyuihgfdsazxcvjb"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6").arg(QString::number(qrand()%10),QString::number(qrand()%10),QString::number(qrand()%10),
												 qstrSeed,kb[i][j+1],kb[i][j+2]);
				break;
			case 3://"qwe123"
				qstrSeed=random_qchar_in_qstring(QString("qwertyuihgfdsazxcvjb"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6").arg(qstrSeed,kb[i][j+1],kb[i][j+2],
												 QString::number(qrand()%10),QString::number(qrand()%10),QString::number(qrand()%10));
				break;
			case 4://"qwerty"
				qstrSeed=random_qchar_in_qstring(QString("trewqfdsaxz"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6").arg(qstrSeed,kb[i][j+1],kb[i][j+2],
												 kb[i][j+3],kb[i][j+4],kb[i][j+5]);
				break;
			case 5://"qwe123456"
				qstrSeed=random_qchar_in_qstring(QString("qwertyuihgfdsazxcvjb"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6%7%8%9").arg(qstrSeed,kb[i][j+1],kb[i][j+2],
												 QString::number(qrand()%10),QString::number(qrand()%10),QString::number(qrand()%10),
												 QString::number(qrand()%10),QString::number(qrand()%10),QString::number(qrand()%10));
				break;
			case 6://"zxcvbnm"
				qstrSeed=random_qchar_in_qstring(QString("zdsarewq"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6%7").arg(qstrSeed,kb[i][j+1],kb[i][j+2],
												 kb[i][j+3],kb[i][j+4],kb[i][j+5],kb[i][j+6]);
				break;
			case 7://"zxcvbnm123"
				qstrSeed=random_qchar_in_qstring(QString("zdsarewq"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6%7%8").arg(qstrSeed,kb[i][j+1],kb[i][j+2],
												 kb[i][j+3],kb[i][j+4],kb[i][j+5],kb[i][j+6],
												QString("%1%2%3").arg(QString::number(qrand()%10),QString::number(qrand()%10),QString::number(qrand()%10)));
				break;
			case 8://"1234qwer"
				qstrSeed=random_qchar_in_qstring(QString("qwertyuihgfdsazxc"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6%7%8").arg(QString::number(qrand()%10),QString::number(qrand()%10),QString::number(qrand()%10),QString::number(qrand()%10),
												 qstrSeed,kb[i][j+1],kb[i][j+2],kb[i][j+3]);
				break;
			case 9:	//"qwer1234"
				qstrSeed=random_qchar_in_qstring(QString("qwertyuihgfdsazxc"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6%7%8").arg(qstrSeed,kb[i][j+1],kb[i][j+2],kb[i][j+3],
													 QString::number(qrand()%10),QString::number(qrand()%10),QString::number(qrand()%10),QString::number(qrand()%10));
				break;
			case 10://"12qwaszx"
				qstrSeed=random_qchar_in_qstring(QString("ytrewq"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6%7%8").arg(QString::number(qrand()%10),QString::number(qrand()%10),
													 qstrSeed,kb[i][j+1],
													 kb[i+1][j],kb[i+1][j+1],
													 kb[i+2][j],kb[i+2][j+1]);
				break;
			case 11://"0okmnji9"
				qstrSeed=random_qchar_in_qstring(QString("oiuytr"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6%7%8").arg(QString::number(qrand()%10),
													 qstrSeed,kb[i+1][j],
													 kb[i+2][j],kb[i+2][j-1],
													 kb[i+1][j-1],kb[i][j-1],
													 QString::number(qrand()%10));
				break;
			case 12://"qwedsa"
				qstrSeed=random_qchar_in_qstring(QString("qwertyugfdsa"),1);
				DataAnalysis::find_keyboard_loc(qstrSeed[0],i,j);
				str+=QString("%1%2%3%4%5%6").arg(qstrSeed,kb[i][j+1],
												 kb[i][j+2],kb[i+1][j+2],
												 kb[i+1][j+1],kb[i+1][j]);
				break;
			default:break;
		}
		//qDebug()<<str;
		if(strList.contains(str))
		{
			fail_count++;
		}
		else
		{
			fail_count=0;
			strList.append(str);
			total--;
		}
		iStep++;
		if(iStep>=10){
			ui->progressBar->setValue((iAll-total)*100/iAll);
			wait();
			iStep=0;
		}

	}
	ui->progressBar->setValue(100);

}

void MainWindow::produce_date(QStringList &strList, int total)
{
	QString str=QString();
	int t;
	QStringList qstrListType=(QStringList()<<"YYYYMMDD"
							  <<"YYMMDD"
							  <<"MMDDYY"
							  <<"AAAYYMMDD"
							  <<"AAAAYYYY"
							  <<"AAAYYYYMMDD"
							  <<"AAAYYYY"
							  <<"AAAAAAMMDD"
							  <<"AAAAADDMM"
							  <<"AAAAAYYYY"
							  <<"MMDDYYYY"
							  <<"DDMMYYYY"
							  <<"YYYYAAAAAAA"
							  <<"DDYYYYMM"
							  <<"MMDDAAAAA");
	QString qstrA="QWERTYUIOPLKJHGFDSAZXCVBNMmnbvcxzasdfghjklpoiuytrewq";
	QString qstrType;
	QString qstrTemp;

	QTime tt;
	tt= QTime::currentTime();
	qsrand(tt.msec()+tt.second()*1000);

	int rand,iStep=0,iAll=total;
	int fail_count=0;
	ui->progressBar->setValue(0);
	while(total>0&&fail_count<1000)//失败一千次直接跳出
	{
		str=QString();
		rand=qrand()%qstrListType.size();
		qstrType=qstrListType.at(rand);
		if(qstrType.indexOf("YYYY")>-1)
		{
			qstrTemp=QString::number((qrand()%2+19)*100+qrand()%100);//年
			qstrType.replace("YYYY",qstrTemp);
			qstrTemp=QString::number(101+qrand()%12).right(2);//月
			qstrType.replace("MM",qstrTemp);
			if(qstrTemp=="02")
				qstrTemp=QString::number(101+qrand()%28).right(2);
			else if(qstrTemp=="04"||qstrTemp=="06"||qstrTemp=="09"||qstrTemp=="11")
				qstrTemp=QString::number(101+qrand()%30).right(2);
			else
				qstrTemp=QString::number(101+qrand()%31).right(2);//日
			qstrType.replace("DD",qstrTemp);
			int temp=qstrType.indexOf("A");
			str=qstrType.left(temp);
			if(temp==-1)
				qstrType.clear();
			else
			{
				while(temp>-1){
					str+=random_qchar_in_qstring(qstrA,1);
					qstrType=qstrType.mid(temp+1,-1);
					temp=qstrType.indexOf("A");
				}
				str+=qstrType;
			}
		}
		else
		{
			qstrTemp=QString::number(100+qrand()%100).right(2);//年
			qstrType.replace("YY",qstrTemp);
			qstrTemp=QString::number(101+qrand()%12).right(2);//月
			qstrType.replace("MM",qstrTemp);
			if(qstrTemp=="02")
				qstrTemp=QString::number(101+qrand()%28).right(2);
			else if(qstrTemp=="04"||qstrTemp=="6"||qstrTemp=="9"||qstrTemp=="11")
				qstrTemp=QString::number(101+qrand()%30).right(2);
			else
				qstrTemp=QString::number(101+qrand()%31).right(2);//日
			qstrType.replace("DD",qstrTemp);
			int temp=qstrType.indexOf("A");
			str=qstrType.left(temp);
			if(temp==-1)
				qstrType.clear();
			else
			{
				while(temp>-1){
					str+=random_qchar_in_qstring(qstrA,1);
					qstrType=qstrType.mid(temp+1,-1);
					temp=qstrType.indexOf("A");
				}
				str+=qstrType;
			}
		}
		if(strList.contains(str))
		{
			fail_count++;
		}
		else
		{
			fail_count=0;
			strList.append(str);
			total--;
		}
		iStep++;
		if(iStep>=10){
			ui->progressBar->setValue((iAll-total)*100/iAll);
			wait();
			iStep=0;
		}
	}
	ui->progressBar->setValue(100);
}
void MainWindow::produce_pinyin(QStringList &strList, int total)
{
	QString str=QString();
	int t;
	QStringList qstrListType=(QStringList()<<"P1P2P1P2"
							  <<"P1P2NN"
							  <<"NNP1P2"
							  <<"P1YYMMDD"
							  <<"P1YYYY"
							  <<"P1YYYYMMDD"
							  <<"P1YYYY"
							  <<"P1MMDD"
							  <<"P1P2DDMM"
							  <<"P1P2YYYY"
							  <<"YYYYP1P2"
							  <<"MMDDP1P2");
	//QString qstrA="QWERTYUIOPLKJHGFDSAZXCVBNMmnbvcxzasdfghjklpoiuytrewq";
	QString qstrN="0123456789";
	QString qstrType;
	QString qstrTemp;

	QStringList qstrList=get_pinyin_list();
	QStringList qstrList2=(QStringList()<<"wang"
						   <<"xiao"
						   <<"zhang"
						   <<"yang"
						   <<"chen"
						   <<"fan"
						   <<"men"
						   <<"ming"
						   <<"jian"
						   <<"feng"
						   <<"shi"
						   <<"woaini"
						   <<"long"
						   <<"jing"
						   <<"yuan"
						   <<"zhao"
						   <<"tian"
						   <<"zhou"
						   <<"huang"
				);

	QTime tt;
	tt= QTime::currentTime();
	qsrand(tt.msec()+tt.second()*1000);

	int rand,iStep=0,iAll=total;
	int fail_count=0;
	ui->progressBar->setValue(0);
	while(total>0&&fail_count<1000)//失败一千次直接跳出
	{
		str=QString();
		rand=qrand()%qstrListType.size();
		qstrType=qstrListType.at(rand);
		if(qstrType.indexOf("YYYY")>-1)
		{
			qstrTemp=QString::number((qrand()%2+19)*100+qrand()%100);//年
			qstrType.replace("YYYY",qstrTemp);
		}
		else
		{
			qstrTemp=QString::number(100+qrand()%100).right(2);//年
			qstrType.replace("YY",qstrTemp);
		}

		qstrTemp=QString::number(101+qrand()%12).right(2);//月
		qstrType.replace("MM",qstrTemp);
		if(qstrTemp=="02")
			qstrTemp=QString::number(101+qrand()%28).right(2);
		else if(qstrTemp=="04"||qstrTemp=="6"||qstrTemp=="9"||qstrTemp=="11")
			qstrTemp=QString::number(101+qrand()%30).right(2);
		else
			qstrTemp=QString::number(101+qrand()%31).right(2);//日
		qstrType.replace("DD",qstrTemp);
		int temp=qstrType.indexOf("N");
		str=qstrType.left(temp);
		if(temp==-1)
			qstrType.clear();
		else
		{
			while(temp>-1){
				str+=random_qchar_in_qstring(qstrN,1);
				qstrType=qstrType.mid(temp+1,-1);
				temp=qstrType.indexOf("N");
			}
			str+=qstrType;
		}

		rand=qrand()%2;
		if(rand==0)
		{
			rand=qrand()%qstrList2.size();
			str.replace("P1",qstrList2.at(rand));
			rand=qrand()%qstrList2.size();
			str.replace("P2",qstrList2.at(rand));
		}
		else
		{
			rand=qrand()%qstrList.size();
			str.replace("P1",qstrList.at(rand));
			rand=qrand()%qstrList.size();
			str.replace("P2",qstrList.at(rand));
		}

		if(strList.contains(str))
		{
			fail_count++;
		}
		else
		{
			fail_count=0;
			strList.append(str);
			total--;
		}
		iStep++;
		if(iStep>=10){
			ui->progressBar->setValue((iAll-total)*100/iAll);
			wait();
			iStep=0;
		}
	}
	ui->progressBar->setValue(100);
}

void MainWindow::produce_word(QStringList &strList, int total)
{
	QString str=QString();
	int t;
	QStringList qstrListType=(QStringList()<<"W1NNN"
							  <<"W1NN"
							  <<"W1NNNN"
							  <<"NNW1"
							  <<"W1YYMMDD"
							  <<"W1YYYY"
							  <<"W1YYYYMMDD"
							  <<"W1YYYY"
							  <<"W1MMDD"
							  <<"W1DDMM"
							  <<"W1YYYY"
							  <<"YYYYW1"
							  <<"MMDDW1");
	//QString qstrA="QWERTYUIOPLKJHGFDSAZXCVBNMmnbvcxzasdfghjklpoiuytrewq";
	QString qstrN="0123456789";
	QString qstrType;
	QString qstrTemp;

	QStringList qstrList=get_word_list();
	QStringList qstrList2=(QStringList()<<"love"
						   <<"bin"
						   <<"she"
						   <<"you"
						   <<"loveu"
						   <<"loveyou"
						   <<"pen"
						   <<"fan"
						   <<"men"
						   <<"sang"
						   <<"any"
						   <<"window"
						   <<"password"
						   <<"girl"
						   <<"boy"
						   <<"kiss"
						   <<"happy"
						   <<"good"
				);

	QTime tt;
	tt= QTime::currentTime();
	qsrand(tt.msec()+tt.second()*1000);

	int rand,iStep=0,iAll=total;
	int fail_count=0;
	ui->progressBar->setValue(0);
	while(total>0&&fail_count<1000)//失败一千次直接跳出
	{
		str=QString();
		rand=qrand()%qstrListType.size();
		qstrType=qstrListType.at(rand);
		if(qstrType.indexOf("YYYY")>-1)
		{
			qstrTemp=QString::number((qrand()%2+19)*100+qrand()%100);//年
			qstrType.replace("YYYY",qstrTemp);
		}
		else
		{
			qstrTemp=QString::number(100+qrand()%100).right(2);//年
			qstrType.replace("YY",qstrTemp);
		}

		qstrTemp=QString::number(101+qrand()%12).right(2);//月
		qstrType.replace("MM",qstrTemp);
		if(qstrTemp=="02")
			qstrTemp=QString::number(101+qrand()%28).right(2);
		else if(qstrTemp=="04"||qstrTemp=="6"||qstrTemp=="9"||qstrTemp=="11")
			qstrTemp=QString::number(101+qrand()%30).right(2);
		else
			qstrTemp=QString::number(101+qrand()%31).right(2);//日
		qstrType.replace("DD",qstrTemp);
		int temp=qstrType.indexOf("N");
		str=qstrType.left(temp);
		if(temp==-1)
			qstrType.clear();
		else
		{
			while(temp>-1){
				str+=random_qchar_in_qstring(qstrN,1);
				qstrType=qstrType.mid(temp+1,-1);
				temp=qstrType.indexOf("N");
			}
			str+=qstrType;
		}
		rand=qrand()%2;
		if(rand==0)
		{
			rand=qrand()%qstrList2.size();
			str.replace("W1",qstrList2.at(rand));
		}
		else
		{
			rand=qrand()%qstrList.size();
			str.replace("W1",qstrList.at(rand));
		}

		if(strList.contains(str))
		{
			fail_count++;
		}
		else
		{
			fail_count=0;
			strList.append(str);
			total--;
		}
		iStep++;
		if(iStep>=10){
			ui->progressBar->setValue((iAll-total)*100/iAll);
			wait();
			iStep=0;
		}
	}
	ui->progressBar->setValue(100);
}

void MainWindow::wait()
{
	QElapsedTimer et;
	et.start();
	while(et.elapsed()<50){
		QApplication::processEvents();
	}
}

QStringList MainWindow::get_pinyin_list()
{
	QStringList qstrList;
	qstrList.append("a");
	qstrList.append("ai");
	qstrList.append("an");
	qstrList.append("ang");
	qstrList.append("ao");
	qstrList.append("ba");
	qstrList.append("bai");
	qstrList.append("ban");
	qstrList.append("bang");
	qstrList.append("bao");
	qstrList.append("bei");
	qstrList.append("ben");
	qstrList.append("beng");
	qstrList.append("bi");
	qstrList.append("bian");
	qstrList.append("biao");
	qstrList.append("bie");
	qstrList.append("bin");
	qstrList.append("bing");
	qstrList.append("bo");
	qstrList.append("bu");
	qstrList.append("ca");
	qstrList.append("cai");
	qstrList.append("can");
	qstrList.append("cang");
	qstrList.append("cao");
	qstrList.append("ce");
	qstrList.append("cen");
	qstrList.append("ceng");
	qstrList.append("cha");
	qstrList.append("chai");
	qstrList.append("chan");
	qstrList.append("chang");
	qstrList.append("chao");
	qstrList.append("che");
	qstrList.append("chen");
	qstrList.append("cheng");
	qstrList.append("chi");
	qstrList.append("chong");
	qstrList.append("chou");
	qstrList.append("chu");
	qstrList.append("chua");
	qstrList.append("chuai");
	qstrList.append("chuan");
	qstrList.append("chuang");
	qstrList.append("chui");
	qstrList.append("chun");
	qstrList.append("chuo");
	qstrList.append("ci");
	qstrList.append("cong");
	qstrList.append("cou");
	qstrList.append("cu");
	qstrList.append("cuan");
	qstrList.append("cui");
	qstrList.append("cun");
	qstrList.append("cuo");
	qstrList.append("da");
	qstrList.append("dai");
	qstrList.append("dan");
	qstrList.append("dang");
	qstrList.append("dao");
	qstrList.append("de");
	qstrList.append("dei");
	qstrList.append("den");
	qstrList.append("deng");
	qstrList.append("di");
	qstrList.append("dia");
	qstrList.append("dian");
	qstrList.append("diao");
	qstrList.append("die");
	qstrList.append("ding");
	qstrList.append("diu");
	qstrList.append("dong");
	qstrList.append("dou");
	qstrList.append("du");
	qstrList.append("duan");
	qstrList.append("dui");
	qstrList.append("dun");
	qstrList.append("duo");
	qstrList.append("e");
	qstrList.append("en");
	qstrList.append("eng");
	qstrList.append("er");
	qstrList.append("fa");
	qstrList.append("fan");
	qstrList.append("fang");
	qstrList.append("fei");
	qstrList.append("fen");
	qstrList.append("feng");
	qstrList.append("fiao");
	qstrList.append("fo");
	qstrList.append("fou");
	qstrList.append("fu");
	qstrList.append("ga");
	qstrList.append("gai");
	qstrList.append("gan");
	qstrList.append("gang");
	qstrList.append("gao");
	qstrList.append("ge");
	qstrList.append("gei");
	qstrList.append("gen");
	qstrList.append("geng");
	qstrList.append("gong");
	qstrList.append("gou");
	qstrList.append("gu");
	qstrList.append("gua");
	qstrList.append("guai");
	qstrList.append("guan");
	qstrList.append("guang");
	qstrList.append("gui");
	qstrList.append("gun");
	qstrList.append("guo");
	qstrList.append("ha");
	qstrList.append("hai");
	qstrList.append("han");
	qstrList.append("hang");
	qstrList.append("hao");
	qstrList.append("he");
	qstrList.append("hei");
	qstrList.append("hen");
	qstrList.append("heng");
	qstrList.append("hong");
	qstrList.append("hou");
	qstrList.append("hu");
	qstrList.append("hua");
	qstrList.append("huai");
	qstrList.append("huan");
	qstrList.append("huang");
	qstrList.append("hui");
	qstrList.append("hun");
	qstrList.append("huo");
	qstrList.append("ji");
	qstrList.append("jia");
	qstrList.append("jian");
	qstrList.append("jiang");
	qstrList.append("jiao");
	qstrList.append("jie");
	qstrList.append("jin");
	qstrList.append("jing");
	qstrList.append("jiong");
	qstrList.append("jiu");
	qstrList.append("ju");
	qstrList.append("juan");
	qstrList.append("jue");
	qstrList.append("ka");
	qstrList.append("kai");
	qstrList.append("kan");
	qstrList.append("kang");
	qstrList.append("kao");
	qstrList.append("ke");
	qstrList.append("ken");
	qstrList.append("keng");
	qstrList.append("kong");
	qstrList.append("kou");
	qstrList.append("ku");
	qstrList.append("kua");
	qstrList.append("kuai");
	qstrList.append("kuan");
	qstrList.append("kuang");
	qstrList.append("kui");
	qstrList.append("kun");
	qstrList.append("kuo");
	qstrList.append("la");
	qstrList.append("lai");
	qstrList.append("lan");
	qstrList.append("lang");
	qstrList.append("lao");
	qstrList.append("le");
	qstrList.append("lei");
	qstrList.append("leng");
	qstrList.append("li");
	qstrList.append("lia");
	qstrList.append("lian");
	qstrList.append("liang");
	qstrList.append("liao");
	qstrList.append("lie");
	qstrList.append("lin");
	qstrList.append("ling");
	qstrList.append("liu");
	qstrList.append("lo");
	qstrList.append("long");
	qstrList.append("lou");
	qstrList.append("lu");
	qstrList.append("luan");
	qstrList.append("lun");
	qstrList.append("luo");
	qstrList.append("lv");
	qstrList.append("lve");
	qstrList.append("ma");
	qstrList.append("mai");
	qstrList.append("man");
	qstrList.append("mang");
	qstrList.append("mao");
	qstrList.append("me");
	qstrList.append("mei");
	qstrList.append("men");
	qstrList.append("meng");
	qstrList.append("mi");
	qstrList.append("mian");
	qstrList.append("miao");
	qstrList.append("mie");
	qstrList.append("minv");
	qstrList.append("ming");
	qstrList.append("miu");
	qstrList.append("mo");
	qstrList.append("mou");
	qstrList.append("mu");
	qstrList.append("na");
	qstrList.append("nai");
	qstrList.append("nan");
	qstrList.append("nang");
	qstrList.append("nao");
	qstrList.append("ne");
	qstrList.append("nei");
	qstrList.append("nen");
	qstrList.append("neng");
	qstrList.append("ni");
	qstrList.append("nian");
	qstrList.append("niang");
	qstrList.append("niao");
	qstrList.append("nie");
	qstrList.append("nin");
	qstrList.append("ning");
	qstrList.append("niu");
	qstrList.append("nong");
	qstrList.append("nou");
	qstrList.append("nu");
	qstrList.append("nuan");
	qstrList.append("nun");
	qstrList.append("nuo");
	qstrList.append("nv");
	qstrList.append("nve");
	qstrList.append("o");
	qstrList.append("ou");
	qstrList.append("pa");
	qstrList.append("pai");
	qstrList.append("pan");
	qstrList.append("pang");
	qstrList.append("pao");
	qstrList.append("pei");
	qstrList.append("pen");
	qstrList.append("peng");
	qstrList.append("pi");
	qstrList.append("pian");
	qstrList.append("piao");
	qstrList.append("pie");
	qstrList.append("pin");
	qstrList.append("ping");
	qstrList.append("po");
	qstrList.append("pou");
	qstrList.append("pu");
	qstrList.append("qi");
	qstrList.append("qia");
	qstrList.append("qian");
	qstrList.append("qiang");
	qstrList.append("qiao");
	qstrList.append("qie");
	qstrList.append("qin");
	qstrList.append("qing");
	qstrList.append("qiong");
	qstrList.append("qiu");
	qstrList.append("qu");
	qstrList.append("quan");
	qstrList.append("que");
	qstrList.append("qun");
	qstrList.append("ran");
	qstrList.append("rang");
	qstrList.append("rao");
	qstrList.append("re");
	qstrList.append("ren");
	qstrList.append("reng");
	qstrList.append("ri");
	qstrList.append("rong");
	qstrList.append("rou");
	qstrList.append("ru");
	qstrList.append("rua");
	qstrList.append("ruan");
	qstrList.append("rui");
	qstrList.append("run");
	qstrList.append("ruo");
	qstrList.append("sa");
	qstrList.append("sai");
	qstrList.append("san");
	qstrList.append("sang");
	qstrList.append("sao");
	qstrList.append("se");
	qstrList.append("sen");
	qstrList.append("seng");
	qstrList.append("sha");
	qstrList.append("shai");
	qstrList.append("shan");
	qstrList.append("shang");
	qstrList.append("shao");
	qstrList.append("she");
	qstrList.append("shei");
	qstrList.append("shen");
	qstrList.append("sheng");
	qstrList.append("shi");
	qstrList.append("shou");
	qstrList.append("shu");
	qstrList.append("shua");
	qstrList.append("shuai");
	qstrList.append("shuan");
	qstrList.append("shuang");
	qstrList.append("shui");
	qstrList.append("shun");
	qstrList.append("shuo");
	qstrList.append("si");
	qstrList.append("song");
	qstrList.append("sou");
	qstrList.append("su");
	qstrList.append("suan");
	qstrList.append("sui");
	qstrList.append("sun");
	qstrList.append("suo");
	qstrList.append("ta");
	qstrList.append("tai");
	qstrList.append("tan");
	qstrList.append("tang");
	qstrList.append("tao");
	qstrList.append("te");
	qstrList.append("tei");
	qstrList.append("teng");
	qstrList.append("ti");
	qstrList.append("tian");
	qstrList.append("tiao");
	qstrList.append("tie");
	qstrList.append("ting");
	qstrList.append("tong");
	qstrList.append("tou");
	qstrList.append("tu");
	qstrList.append("tuan");
	qstrList.append("tui");
	qstrList.append("tun");
	qstrList.append("tuo");
	qstrList.append("wa");
	qstrList.append("wai");
	qstrList.append("wan");
	qstrList.append("wang");
	qstrList.append("wei");
	qstrList.append("wen");
	qstrList.append("weng");
	qstrList.append("wo");
	qstrList.append("wu");
	qstrList.append("xi");
	qstrList.append("xia");
	qstrList.append("xian");
	qstrList.append("xiang");
	qstrList.append("xiao");
	qstrList.append("xie");
	qstrList.append("xin");
	qstrList.append("xing");
	qstrList.append("xiong");
	qstrList.append("xiu");
	qstrList.append("xu");
	qstrList.append("xuan");
	qstrList.append("xue");
	qstrList.append("xun");
	qstrList.append("ya");
	qstrList.append("yan");
	qstrList.append("yang");
	qstrList.append("yao");
	qstrList.append("ye");
	qstrList.append("yi");
	qstrList.append("yin");
	qstrList.append("ying");
	qstrList.append("yo");
	qstrList.append("yong");
	qstrList.append("you");
	qstrList.append("yu");
	qstrList.append("yuan");
	qstrList.append("yue");
	qstrList.append("yun");
	qstrList.append("za");
	qstrList.append("zai");
	qstrList.append("zan");
	qstrList.append("zang");
	qstrList.append("zao");
	qstrList.append("ze");
	qstrList.append("zei");
	qstrList.append("zen");
	qstrList.append("zeng");
	qstrList.append("zha");
	qstrList.append("zhai");
	qstrList.append("zhan");
	qstrList.append("zhang");
	qstrList.append("zhao");
	qstrList.append("zhe");
	qstrList.append("zhei");
	qstrList.append("zhen");
	qstrList.append("zheng");
	qstrList.append("zhi");
	qstrList.append("zhong");
	qstrList.append("zhou");
	qstrList.append("zhu");
	qstrList.append("zhua");
	qstrList.append("zhuai");
	qstrList.append("zhuan");
	qstrList.append("zhuang");
	qstrList.append("zhui");
	qstrList.append("zhun");
	qstrList.append("zhuo");
	qstrList.append("zi");
	qstrList.append("zong");
	qstrList.append("zou");
	qstrList.append("zu");
	qstrList.append("zuan");
	qstrList.append("zui");
	qstrList.append("zun");
	qstrList.append("zuo");
	return qstrList;
}

QStringList MainWindow::get_word_list()
{
	QFile file("words.txt");
	QStringList qstrList;

	if(!file.open(QIODevice::ReadOnly ))
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + file.errorString());
		return qstrList; //不处理文件
	}
	QString qstr;
	while(!file.atEnd())
	{
		qstr=file.readLine().trimmed();
		if(!qstr.isEmpty())
			qstrList.append(qstr);
	}
	file.close();
	return qstrList;
}

void MainWindow::print_value_result(int iScore, QStringList &msgList)
{
	QString qstrEstimate("弱");
	if(iScore>13)
		qstrEstimate="强";
	else if(iScore>9)
		qstrEstimate="中";
	ui->textBrowserResult->clear();
	ui->textBrowserResult->insertHtml("<h1>"+qstrEstimate+"</h1><br>");
	ui->textBrowserResult->insertHtml("<p>解析：</p><br>");
	if(msgList.size()<1)
		return;
	for(int i=0;i<msgList.size();i++)
		ui->textBrowserResult->insertHtml("<p>"+msgList.at(i)+"</p><br>");

}

void MainWindow::slot_open_preprocess_ui(){
	PreProcess pp;
	pp.exec();

}

void MainWindow::slot_open_dataanalysis_ui(){
	DataAnalysis da;
	da.exec();
}


void MainWindow::on_buttonOpenDic_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, "选择目录","",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
	if(dir.size()>0)
		ui->lineDic->setText(dir+"/");
}

void MainWindow::on_buttonProduce_clicked()
{
	ui->labelNote->setText("预计时间："+QString::number(ui->spinBoxNum->value()*5/1000)+"秒");
	int count=0,sum1loop;
	//int i;
	if(ui->checkBoxWeak->isChecked()){
		count++;
	}
	if(ui->checkBoxKB->isChecked()){
		count++;
	}
	if(ui->checkBoxDate->isChecked()){
		count++;
	}
	if(ui->checkBoxPinyin->isChecked()){
		count++;
	}
	if(ui->checkBoxWord->isChecked()){
		count++;
	}

	QStringList strListProduce;
	if(count==0)
	{
		QMessageBox::warning(this,"提示","至少选择一个密码类型");
		return;
	}
	sum1loop=ui->spinBoxNum->value()/count;

	if(ui->checkBoxWeak->isChecked())
		produce_weak(strListProduce,sum1loop);
	if(ui->checkBoxKB->isChecked())
		produce_kb(strListProduce,sum1loop);
	if(ui->checkBoxDate->isChecked())
		produce_date(strListProduce,sum1loop);
	if(ui->checkBoxPinyin->isChecked())
		produce_pinyin(strListProduce,sum1loop);
	if(ui->checkBoxWord->isChecked())
		produce_word(strListProduce,sum1loop);

	save(strListProduce);
}

void MainWindow::on_action_close_triggered()
{
	this->close();
}

void MainWindow::on_action_about_triggered()
{
	About a(this);
	a.exec();
}

void MainWindow::on_pushButtonValue_clicked()
{
	QString qstrPasswd=ui->lineEditPasswd->text();
	QStringList reasonList;
	bool flag=true;
	qstrPasswd=qstrPasswd.trimmed();
	if(qstrPasswd.size()<1)
		return;
	int iScore=0;//manfen 15 0-8 9-13 14-15
	int i;
	if(qstrPasswd.size()<7)
	{
		reasonList.append("长度过短");
		print_value_result(0,reasonList);
		return;
	}

	QRegExp re("^[a-zA-Z]+$");
	if(re.exactMatch(qstrPasswd))
	{
		if(qstrPasswd.size()<11)
		{
			reasonList.append("长度过短的纯英文字母密码");
			print_value_result(0,reasonList);
		}
		else
		{
			reasonList.append("字符类型过于单一");
			print_value_result(9,reasonList);
		}
		return;
	}
	re.setPattern("^[0-9]+$");
	if(re.exactMatch(qstrPasswd))
	{

		if(qstrPasswd.size()<13)
		{
			reasonList.append("长度过短的纯数字密码");
			print_value_result(0,reasonList);
		}
		else
		{
			reasonList.append("字符类型过于单一");
			print_value_result(9,reasonList);
		}
		return;
	}
	//1111111111111111111111111111111111
	QStringList patternList;
	patternList<<".*([a-zA-Z]{3}[0-9]{6}).*"<<".*([a-zA-Z]{6}[0-9]{2}).*"<<".*([a-zA-Z]{8}[0-9]{2}).*"
			   <<".*([a-zA-Z]{7}[0-9]{1}).*"<<".*([a-zA-Z]{7}[0-9]{2}).*"<<".*([a-zA-Z]{5}[0-9]{2}).*";
	int index;
	for(i=0;i<patternList.size();i++)
	{
		re.setPattern(patternList.at(i));
		index=re.indexIn(qstrPasswd);
		if(index==0)
		{
			if(qstrPasswd.size()==re.cap(1).size())
			{
				iScore-=3;
				reasonList.append("字符组合属于弱组合");
			}
			else
			{
				iScore-=2;
				reasonList.append("字符组合含有弱组合");
			}
			break;
		}
		else if(index>0)
		{
			iScore-=2;
			reasonList.append("字符组合含有弱组合");
			break;
		}
	}
	if(i==patternList.size())
		reasonList.append("字符组合安全");
	iScore+=3;
	//222222222222222222222222222222222222
	patternList.clear();
	patternList<<"asd"<<"qwe"<<"qaz"<<"zxc"<<"qwert"<<"qwer"<<"qwerty"<<"zxcv"<<"zxcvbm"<<"fred"<<"asdf"
			   <<"asdf"<<"loki"<<"qazxsw"<<"asdfgh"<<"asdfghjkl";
	for(i=0;i<patternList.size();i++)
	{
		index=qstrPasswd.indexOf(patternList.at(i));
		if(index==0)
		{
			if(patternList.at(i).size()<=6||qstrPasswd.size()==patternList.at(i).size())
			{
				iScore-=3;
				reasonList.append("字符含有极易识别的键盘模式");
			}
			else
			{
				iScore-=2;
				reasonList.append("字符含有易识别的键盘模式");
			}
			break;
		}
		else if(index>0)
		{
			if(patternList.at(i).size()<=6)
			{
				iScore-=3;
				reasonList.append("字符含有极易识别的键盘模式");
			}
			else
			{
				iScore-=2;
				reasonList.append("字符含有易识别的键盘模式");
			}
			break;
		}
	}
	if(i==patternList.size())
		reasonList.append("键盘模式检验通过");
	iScore+=3;
	//333333333333333
	patternList.clear();
	QString YY="[0-9]{2}";	//33
	QString YYYY="(19|20)[0-9]{2}";	//3333
	QString MM="(0[1-9]|1[0-2])";	//11
	QString DD="(0[1-9]|[1-2][0-9]|3[0-1])";	//22
	patternList<<".*(([a-zA-Z]{2,6})?"+YYYY+MM+DD+").*"<<".*(([a-zA-Z]{2,4})?"+YY+MM+DD+").*"
			  <<".*("+DD+MM+YY+").*"<<".*("+MM+DD+YY+").*"
			  <<".*("+MM+DD+YYYY+").*"<<".*([a-zA-Z]{3,8}"+YYYY+").*"<<".*(([a-zA-Z]{5,6})?"+MM+DD+").*";
	for(i=0;i<patternList.size();i++)
	{
		re.setPattern(patternList.at(i));
		index=re.indexIn(qstrPasswd);
		if(index==0)
		{
			if(qstrPasswd.size()==re.cap(1).size())
			{
				iScore-=3;
				reasonList.append("密码属于较弱的日期混合格式");
			}
			else
			{
				iScore-=2;
				reasonList.append("密码中含有较弱的日期格式");
			}
			break;
		}
		else if(index>0)
		{
			iScore-=2;
			reasonList.append("密码中含有较弱的日期格式");
			break;
		}
	}
	if(i==patternList.size())
		reasonList.append("日期格式验证通过");
	iScore+=3;
	//44444444444444444
	patternList.clear();
	patternList<<"wang"<<"xiao"<<"zhang"<<"yang"<<"chen"<<"qwer"<<"wo"<<"jian"<<"feng"<<"shi"<<"ai"
			   <<"ling"<<"chan"<<"rang"<<"shan"<<"lian"<<"ting"<<"dian"<<"chi"<<"lin"<<"tang";
	for(i=0;i<patternList.size();i++)
	{
		index=qstrPasswd.indexOf(patternList.at(i));
		if(index==0)
		{
			if(patternList.at(i).size()<=6||qstrPasswd.size()==patternList.at(i).size())
			{
				iScore-=3;
				reasonList.append("字符含有极易检测的拼音");
			}
			else
			{
				iScore-=2;
				reasonList.append("字符含有易检测的拼音");
			}
			break;
		}
		else if(index>0)
		{
			if(patternList.at(i).size()<=6)
			{
				iScore-=3;
				reasonList.append("字符含有极易检测的拼音");
			}
			else
			{
				iScore-=2;
				reasonList.append("字符含有易检测的拼音");
			}
			break;
		}
	}
	if(i==patternList.size())
		reasonList.append("拼音检查通过");
	iScore+=3;

	//55555555555555555
	patternList.clear();
	patternList<<"love"<<"she"<<"you"<<"fan"<<"men"<<"song"<<"any"<<"king"<<"his"<<"ting"<<"and"
			   <<"cat"<<"dog"<<"car"<<"son"<<"the"<<"red"<<"ill"<<"boy"<<"girl"<<"password"<<"baby";
	for(i=0;i<patternList.size();i++)
	{
		index=qstrPasswd.indexOf(patternList.at(i));
		if(index==0)
		{
			if(patternList.at(i).size()<=6||qstrPasswd.size()==patternList.at(i).size())
			{
				iScore-=3;
				reasonList.append("字符含有极易检测的单词");
			}
			else
			{
				iScore-=2;
				reasonList.append("字符含有易检测的单词");
			}
			break;
		}
		else if(index>0)
		{
			if(patternList.at(i).size()<=6)
			{
				iScore-=3;
				reasonList.append("字符含有极易检测的单词");
			}
			else
			{
				iScore-=2;
				reasonList.append("字符含有易检测的单词");
			}
			break;
		}
	}
	if(i==patternList.size())
		reasonList.append("单词检查通过");
	iScore+=3;
	re.setPattern("[^0-9a-zA-Z]");
	if(re.indexIn(qstrPasswd)<0)
		iScore-=4;

	print_value_result(iScore,reasonList);
}
