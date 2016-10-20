#include "dataanalysis.h"
#include "ui_dataanalysis.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QHash>
#include <QRegExp>
#include <QElapsedTimer>

#include <QDebug>

DataAnalysis::DataAnalysis(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DataAnalysis)
{
	ui->setupUi(this);
		this->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);

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
		//ui->lineEditSrcFile->setText("");
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
			phonetic_top10_ana();
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
		if(sum%100==0)
			wait();

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
	int iStep=0;

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
				DataAnalysis::find_keyboard_loc(c,ii,jj);
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
		iStep++;
		if(iStep>=100){
			iStep=0;
			wait();
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

	int i,j;
	QHash<QString,int> hash;
	QString temple;
	QString result;
	//QChar ipre;
	int iStep=0;
	while(!fileAna.atEnd())
	{
		strLine=fileAna.readLine().trimmed();
		//strLine="11111";


		for(i=0,temple="";i<strLine.size();i++)
		{
			if(strLine[i].isDigit())
			{
				temple+=strLine[i];
			}
		}
		if(temple.size()<4||temple=="111111"||temple=="11111111"||temple=="111222"||temple=="112233"||temple=="123456"||temple=="12345678"||temple=="1111"||temple=="123123"||temple=="123321"||temple=="121212"||temple=="101010")
			continue;
		if(temple.size()>0&&is_date_format(temple))
		{
			for(i=0,j=0,result="";i<strLine.size();i++)
			{
				if(strLine[i].isDigit())
					result+=temple[j++];
				else if(strLine[i].isLetter())
					result+="A";
				else
					result+="S";
			}
			//qDebug()<<strLine;
			result=strLine; //输出模型注释此行
			if(hash.contains(result))
				hash[result]++;
			else
				hash.insert(result,1);
		}
		iStep++;
		if(iStep>=100){
			iStep=0;
			wait();
		}
		//qDebug()<<strLine;
	}
	fileAna.close();

	QHash<int,QString> sort_hash;

	QString text="";

	hash_sort_insert(sort_hash,hash);

	for(i=0;i<sort_hash.size();i++)
	{
		text+=(sort_hash[i]+" -> "+QString::number(hash[sort_hash[i]])+"\r\n");
	}

	ui->textResult->append(text);
}

//拼音top10
void DataAnalysis::phonetic_top10_ana()
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
	QHash<QString,int> hash;
	int i;

	set_phonetic_hash(hash);
	int iStep=0;

	while(!fileAna.atEnd())
	{
		strLine=fileAna.readLine().trimmed();

		find_phonetic(strLine,hash);
		iStep++;
		if(iStep>=100){
			iStep=0;
			wait();
		}
	}
	fileAna.close();

	QHash<int,QString> sort_hash;

	QString text="";

	hash_sort_insert(sort_hash,hash);

	for(i=0;i<sort_hash.size();i++)
	{
		text+=(sort_hash[i]+" -> "+QString::number(hash[sort_hash[i]])+"\r\n");
	}

	ui->textResult->append(text);
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
	QHash<QString,int> hash;
	int i;
	int iStep=0;
	set_word_hash(hash);
	while(!fileAna.atEnd())
	{
		strLine=fileAna.readLine().trimmed();
		find_word(strLine,hash);
		iStep++;
		if(iStep>=100){
			iStep=0;
			wait();
		}
	}

	QHash<int,QString> sort_hash;

	QString text="";

	hash_sort_insert(sort_hash,hash);

	for(i=0;i<sort_hash.size();i++)
	{
		text+=(sort_hash[i]+" -> "+QString::number(hash[sort_hash[i]])+"\r\n");
	}

	ui->textResult->append(text);
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

void DataAnalysis::find_keyboard_loc(QChar c,int &i,int &j)
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

bool DataAnalysis::is_date_format(QString &strT)
{
	int i;
	bool flag=false;
	QString mode;
	if(strT.size()<4)	//不足4位数返回假
		return false;
	if(strT.size()==4)	//4或5位数字时
	{
		i=0;
		while(strT.size()-i>=4)
		{
			int y=strT.mid(i,4).toInt();
			if(y>1900&&y<2100)	//1900-2100间识别为年
			{
				flag=true;
				mode="YYYY";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,4),1122)))
			{
				mode="MMDD";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,4),2211)))
			{
				mode="DDMM";
				break;
			}
			i++;
		}
		if(flag)
		{
			if(i==0&&strT.size()>=5)
				strT.replace(4,1,"N");
			else if(i==1&&strT.size()>=5)
				strT.replace(0,1,"N");
			strT.replace(i,4,mode);
		}
		return flag;
	}
	if(strT.size()==6)	//7位或8位数字
	{
		i=0;
		while(strT.size()-i>=6)
		{
			if((flag=set_date_format(strT.midRef(i,6),331122)))
			{
				mode="YYMMDD";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,6),113322)))
			{
				mode="MMYYDD";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,6),112233)))
			{
				mode="DDMMYY";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,6),332211)))
			{
				mode="YYDDMM";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,6),223311)))
			{
				mode="DDYYMM";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,6),221133)))
			{
				mode="MMDDYY";
				break;
			}
			i++;
		}

		if(flag)
		{
			if(i==0&&strT.size()>6)
				strT.replace(6,1,"N");
			else if(i==1&&strT.size()>6)
				strT.replace(0,1,"N");
			strT.replace(i,6,mode);
		}
		return flag;
	}
	if(strT.size()==8)
	{
		i=0;
		while(strT.size()-i>=8)
		{
			if((flag=set_date_format(strT.midRef(i,8),33331122)))
			{
				mode="YYYYMMDD";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,8),11333322)))
			{
				mode="MMYYYYDD";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,8),11223333)))
			{
				mode="DDMMYYYY";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,8),33332211)))
			{
				mode="YYYYDDMM";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,8),22333311)))
			{
				mode="DDYYYYMM";
				break;
			}
			if(!flag&&(flag=set_date_format(strT.midRef(i,8),22113333)))
			{
				mode="MMDDYYYY";
				break;
			}
			i++;
		}

		if(flag)
		{
			if(strT.size()>8)
			{
				if(i>0)
					strT.replace(0,i,QString("N").repeated(i));
				if(strT.size()-i-8>0)
					strT.replace(i+8,strT.size()-i-8,QString("N").repeated(strT.size()-i-8));
			}
			strT.replace(i,8,mode);
		}
		return flag;
	}
	return false;
}
/**
 * @param format 11=MM,22=DD,3333=YYYY,33=YY
 **/

bool DataAnalysis::set_date_format(QStringRef strT, int format)
{
	QString YY="[0-9]{2}";	//33
	QString YYYY="(19|20)[0-9]{2}";	//3333
	QString MM="(0[1-9]|1[0-2])";	//11
	QString DD="(0[1-9]|[1-2][0-9]|3[0-1])";	//22
	QString pattern;
	switch(format)
	{
		case 1122:pattern=MM+DD;break;
		case 2211:pattern=DD+MM;break;
		case 331122:pattern=YY+MM+DD;break;
		case 113322:pattern=MM+YY+DD;break;
		case 112233:pattern=MM+DD+YY;break;
		case 332211:pattern=YY+DD+MM;break;
		case 223311:pattern=DD+YY+MM;break;
		case 221133:pattern=DD+MM+YY;break;
		case 33331122:pattern=YYYY+MM+DD;break;
		case 11333322:pattern=MM+YYYY+DD;break;
		case 11223333:pattern=MM+DD+YYYY;break;
		case 33332211:pattern=YYYY+DD+MM;break;
		case 22333311:pattern=DD+YYYY+MM;break;
		case 22113333:pattern=DD+MM+YYYY;break;
		default:pattern="null";break;
	}
	if(pattern=="null")
		return false;
	QRegExp re(pattern);
	return re.exactMatch(strT.toString());
}

void DataAnalysis::find_phonetic(QString s,QHash<QString, int> &phonetic_hash)
{
	int i,j=0,k;
	QString ts;
	QString qsPre[10];
	int count=0,out=0;
	//s="camille";
	for(i=0;i<s.size();i++)
	{
		if(s[i].isLetter())
		{
			for(j=s.size()-1;j>i;j--)
			{
				while(j>i&&!s[j].isLetter())
					j--;
				ts=s.mid(i,j-i+1);
				if(phonetic_hash.contains(ts))
				{
					if(ts.size()>3)
					{
						phonetic_hash[ts]++;
					}
					else if(count>=4)	//三个及三个以上的拼音才会被识别
					{
						for(k=0;k<10;k++)
						{
							if(qsPre[k].isEmpty())
								break;
							phonetic_hash[qsPre[k]]++;
							qsPre[k].clear();
							out=0;
						}
						phonetic_hash[ts]++;
						//qDebug()<<s;
					}
					else{
						if(ts.size()==3)
							count+=2;
						if(ts.size()<2)
							count-=2;
						else
							count++;

						qsPre[out++]=ts;
					}
					i=-1;
					break;
				}

			}

		}
		if(i==-1)
			i=j;
		else
			out=count=0;

	}
}

void DataAnalysis::find_word(QString s, QHash<QString, int> &word_hash)
{
	int i,j=0,k;
	QString ts;
	QString qsPre[30];
	int count=0,out=0;
	//s="camille";
	for(i=0;i<s.size();i++)
	{
		if(s[i].isLetter())
		{
			for(j=s.size()-1;j>i;j--)
			{
				while(j>i&&!s[j].isLetter())
					j--;
				ts=s.mid(i,j-i+1);
				if(word_hash.contains(ts))
				{
					if(ts.size()>2)
					{
						word_hash[ts]++;
					}
					else if(count>=2)	//三个及三个以上的连续拼音才会被识别
					{
						for(k=0;k<count-out;k++)
						{
							if(!qsPre[k].isEmpty())
							{
								word_hash[qsPre[k]]++;
								qsPre[k].clear();
							}
							out++;
						}
						word_hash[ts]++;
						//qDebug()<<s;
					}
					else
						qsPre[count++-out]=ts;
					i=-1;
					break;
				}

			}

		}
		if(i==-1)
			i=j;
		else
			out=count=0;

	}
}

void DataAnalysis::set_phonetic_hash(QHash<QString, int> &phonetic_hash)
{
	phonetic_hash.insert("a",0);
	phonetic_hash.insert("ai",0);
	phonetic_hash.insert("an",0);
	phonetic_hash.insert("ang",0);
	phonetic_hash.insert("ao",0);
	phonetic_hash.insert("ba",0);
	phonetic_hash.insert("bai",0);
	phonetic_hash.insert("ban",0);
	phonetic_hash.insert("bang",0);
	phonetic_hash.insert("bao",0);
	phonetic_hash.insert("bei",0);
	phonetic_hash.insert("ben",0);
	phonetic_hash.insert("beng",0);
	phonetic_hash.insert("bi",0);
	phonetic_hash.insert("bian",0);
	phonetic_hash.insert("biao",0);
	phonetic_hash.insert("bie",0);
	phonetic_hash.insert("bin",0);
	phonetic_hash.insert("bing",0);
	phonetic_hash.insert("bo",0);
	phonetic_hash.insert("bu",0);
	phonetic_hash.insert("ca",0);
	phonetic_hash.insert("cai",0);
	phonetic_hash.insert("can",0);
	phonetic_hash.insert("cang",0);
	phonetic_hash.insert("cao",0);
	phonetic_hash.insert("ce",0);
	phonetic_hash.insert("cen",0);
	phonetic_hash.insert("ceng",0);
	phonetic_hash.insert("cha",0);
	phonetic_hash.insert("chai",0);
	phonetic_hash.insert("chan",0);
	phonetic_hash.insert("chang",0);
	phonetic_hash.insert("chao",0);
	phonetic_hash.insert("che",0);
	phonetic_hash.insert("chen",0);
	phonetic_hash.insert("cheng",0);
	phonetic_hash.insert("chi",0);
	phonetic_hash.insert("chong",0);
	phonetic_hash.insert("chou",0);
	phonetic_hash.insert("chu",0);
	phonetic_hash.insert("chua",0);
	phonetic_hash.insert("chuai",0);
	phonetic_hash.insert("chuan",0);
	phonetic_hash.insert("chuang",0);
	phonetic_hash.insert("chui",0);
	phonetic_hash.insert("chun",0);
	phonetic_hash.insert("chuo",0);
	phonetic_hash.insert("ci",0);
	phonetic_hash.insert("cong",0);
	phonetic_hash.insert("cou",0);
	phonetic_hash.insert("cu",0);
	phonetic_hash.insert("cuan",0);
	phonetic_hash.insert("cui",0);
	phonetic_hash.insert("cun",0);
	phonetic_hash.insert("cuo",0);
	phonetic_hash.insert("da",0);
	phonetic_hash.insert("dai",0);
	phonetic_hash.insert("dan",0);
	phonetic_hash.insert("dang",0);
	phonetic_hash.insert("dao",0);
	phonetic_hash.insert("de",0);
	phonetic_hash.insert("dei",0);
	phonetic_hash.insert("den",0);
	phonetic_hash.insert("deng",0);
	phonetic_hash.insert("di",0);
	phonetic_hash.insert("dia",0);
	phonetic_hash.insert("dian",0);
	phonetic_hash.insert("diao",0);
	phonetic_hash.insert("die",0);
	phonetic_hash.insert("ding",0);
	phonetic_hash.insert("diu",0);
	phonetic_hash.insert("dong",0);
	phonetic_hash.insert("dou",0);
	phonetic_hash.insert("du",0);
	phonetic_hash.insert("duan",0);
	phonetic_hash.insert("dui",0);
	phonetic_hash.insert("dun",0);
	phonetic_hash.insert("duo",0);
	phonetic_hash.insert("e",0);
	phonetic_hash.insert("en",0);
	phonetic_hash.insert("eng",0);
	phonetic_hash.insert("er",0);
	phonetic_hash.insert("fa",0);
	phonetic_hash.insert("fan",0);
	phonetic_hash.insert("fang",0);
	phonetic_hash.insert("fei",0);
	phonetic_hash.insert("fen",0);
	phonetic_hash.insert("feng",0);
	phonetic_hash.insert("fiao",0);
	phonetic_hash.insert("fo",0);
	phonetic_hash.insert("fou",0);
	phonetic_hash.insert("fu",0);
	phonetic_hash.insert("ga",0);
	phonetic_hash.insert("gai",0);
	phonetic_hash.insert("gan",0);
	phonetic_hash.insert("gang",0);
	phonetic_hash.insert("gao",0);
	phonetic_hash.insert("ge",0);
	phonetic_hash.insert("gei",0);
	phonetic_hash.insert("gen",0);
	phonetic_hash.insert("geng",0);
	phonetic_hash.insert("gong",0);
	phonetic_hash.insert("gou",0);
	phonetic_hash.insert("gu",0);
	phonetic_hash.insert("gua",0);
	phonetic_hash.insert("guai",0);
	phonetic_hash.insert("guan",0);
	phonetic_hash.insert("guang",0);
	phonetic_hash.insert("gui",0);
	phonetic_hash.insert("gun",0);
	phonetic_hash.insert("guo",0);
	phonetic_hash.insert("ha",0);
	phonetic_hash.insert("hai",0);
	phonetic_hash.insert("han",0);
	phonetic_hash.insert("hang",0);
	phonetic_hash.insert("hao",0);
	phonetic_hash.insert("he",0);
	phonetic_hash.insert("hei",0);
	phonetic_hash.insert("hen",0);
	phonetic_hash.insert("heng",0);
	phonetic_hash.insert("hong",0);
	phonetic_hash.insert("hou",0);
	phonetic_hash.insert("hu",0);
	phonetic_hash.insert("hua",0);
	phonetic_hash.insert("huai",0);
	phonetic_hash.insert("huan",0);
	phonetic_hash.insert("huang",0);
	phonetic_hash.insert("hui",0);
	phonetic_hash.insert("hun",0);
	phonetic_hash.insert("huo",0);
	phonetic_hash.insert("ji",0);
	phonetic_hash.insert("jia",0);
	phonetic_hash.insert("jian",0);
	phonetic_hash.insert("jiang",0);
	phonetic_hash.insert("jiao",0);
	phonetic_hash.insert("jie",0);
	phonetic_hash.insert("jin",0);
	phonetic_hash.insert("jing",0);
	phonetic_hash.insert("jiong",0);
	phonetic_hash.insert("jiu",0);
	phonetic_hash.insert("ju",0);
	phonetic_hash.insert("juan",0);
	phonetic_hash.insert("jue",0);
	phonetic_hash.insert("ka",0);
	phonetic_hash.insert("kai",0);
	phonetic_hash.insert("kan",0);
	phonetic_hash.insert("kang",0);
	phonetic_hash.insert("kao",0);
	phonetic_hash.insert("ke",0);
	phonetic_hash.insert("ken",0);
	phonetic_hash.insert("keng",0);
	phonetic_hash.insert("kong",0);
	phonetic_hash.insert("kou",0);
	phonetic_hash.insert("ku",0);
	phonetic_hash.insert("kua",0);
	phonetic_hash.insert("kuai",0);
	phonetic_hash.insert("kuan",0);
	phonetic_hash.insert("kuang",0);
	phonetic_hash.insert("kui",0);
	phonetic_hash.insert("kun",0);
	phonetic_hash.insert("kuo",0);
	phonetic_hash.insert("la",0);
	phonetic_hash.insert("lai",0);
	phonetic_hash.insert("lan",0);
	phonetic_hash.insert("lang",0);
	phonetic_hash.insert("lao",0);
	phonetic_hash.insert("le",0);
	phonetic_hash.insert("lei",0);
	phonetic_hash.insert("leng",0);
	phonetic_hash.insert("li",0);
	phonetic_hash.insert("lia",0);
	phonetic_hash.insert("lian",0);
	phonetic_hash.insert("liang",0);
	phonetic_hash.insert("liao",0);
	phonetic_hash.insert("lie",0);
	phonetic_hash.insert("lin",0);
	phonetic_hash.insert("ling",0);
	phonetic_hash.insert("liu",0);
	phonetic_hash.insert("lo",0);
	phonetic_hash.insert("long",0);
	phonetic_hash.insert("lou",0);
	phonetic_hash.insert("lu",0);
	phonetic_hash.insert("luan",0);
	phonetic_hash.insert("lun",0);
	phonetic_hash.insert("luo",0);
	phonetic_hash.insert("lv",0);
	phonetic_hash.insert("lve",0);
	phonetic_hash.insert("ma",0);
	phonetic_hash.insert("mai",0);
	phonetic_hash.insert("man",0);
	phonetic_hash.insert("mang",0);
	phonetic_hash.insert("mao",0);
	phonetic_hash.insert("me",0);
	phonetic_hash.insert("mei",0);
	phonetic_hash.insert("men",0);
	phonetic_hash.insert("meng",0);
	phonetic_hash.insert("mi",0);
	phonetic_hash.insert("mian",0);
	phonetic_hash.insert("miao",0);
	phonetic_hash.insert("mie",0);
	phonetic_hash.insert("minv",0);
	phonetic_hash.insert("ming",0);
	phonetic_hash.insert("miu",0);
	phonetic_hash.insert("mo",0);
	phonetic_hash.insert("mou",0);
	phonetic_hash.insert("mu",0);
	phonetic_hash.insert("na",0);
	phonetic_hash.insert("nai",0);
	phonetic_hash.insert("nan",0);
	phonetic_hash.insert("nang",0);
	phonetic_hash.insert("nao",0);
	phonetic_hash.insert("ne",0);
	phonetic_hash.insert("nei",0);
	phonetic_hash.insert("nen",0);
	phonetic_hash.insert("neng",0);
	phonetic_hash.insert("ni",0);
	phonetic_hash.insert("nian",0);
	phonetic_hash.insert("niang",0);
	phonetic_hash.insert("niao",0);
	phonetic_hash.insert("nie",0);
	phonetic_hash.insert("nin",0);
	phonetic_hash.insert("ning",0);
	phonetic_hash.insert("niu",0);
	phonetic_hash.insert("nong",0);
	phonetic_hash.insert("nou",0);
	phonetic_hash.insert("nu",0);
	phonetic_hash.insert("nuan",0);
	phonetic_hash.insert("nun",0);
	phonetic_hash.insert("nuo",0);
	phonetic_hash.insert("nv",0);
	phonetic_hash.insert("nve",0);
	//phonetic_hash.insert("o",0);
	phonetic_hash.insert("ou",0);
	phonetic_hash.insert("pa",0);
	phonetic_hash.insert("pai",0);
	phonetic_hash.insert("pan",0);
	phonetic_hash.insert("pang",0);
	phonetic_hash.insert("pao",0);
	phonetic_hash.insert("pei",0);
	phonetic_hash.insert("pen",0);
	phonetic_hash.insert("peng",0);
	phonetic_hash.insert("pi",0);
	phonetic_hash.insert("pian",0);
	phonetic_hash.insert("piao",0);
	phonetic_hash.insert("pie",0);
	phonetic_hash.insert("pin",0);
	phonetic_hash.insert("ping",0);
	phonetic_hash.insert("po",0);
	phonetic_hash.insert("pou",0);
	phonetic_hash.insert("pu",0);
	phonetic_hash.insert("qi",0);
	phonetic_hash.insert("qia",0);
	phonetic_hash.insert("qian",0);
	phonetic_hash.insert("qiang",0);
	phonetic_hash.insert("qiao",0);
	phonetic_hash.insert("qie",0);
	phonetic_hash.insert("qin",0);
	phonetic_hash.insert("qing",0);
	phonetic_hash.insert("qiong",0);
	phonetic_hash.insert("qiu",0);
	phonetic_hash.insert("qu",0);
	phonetic_hash.insert("quan",0);
	phonetic_hash.insert("que",0);
	phonetic_hash.insert("qun",0);
	phonetic_hash.insert("ran",0);
	phonetic_hash.insert("rang",0);
	phonetic_hash.insert("rao",0);
	phonetic_hash.insert("re",0);
	phonetic_hash.insert("ren",0);
	phonetic_hash.insert("reng",0);
	phonetic_hash.insert("ri",0);
	phonetic_hash.insert("rong",0);
	phonetic_hash.insert("rou",0);
	phonetic_hash.insert("ru",0);
	phonetic_hash.insert("rua",0);
	phonetic_hash.insert("ruan",0);
	phonetic_hash.insert("rui",0);
	phonetic_hash.insert("run",0);
	phonetic_hash.insert("ruo",0);
	phonetic_hash.insert("sa",0);
	phonetic_hash.insert("sai",0);
	phonetic_hash.insert("san",0);
	phonetic_hash.insert("sang",0);
	phonetic_hash.insert("sao",0);
	phonetic_hash.insert("se",0);
	phonetic_hash.insert("sen",0);
	phonetic_hash.insert("seng",0);
	phonetic_hash.insert("sha",0);
	phonetic_hash.insert("shai",0);
	phonetic_hash.insert("shan",0);
	phonetic_hash.insert("shang",0);
	phonetic_hash.insert("shao",0);
	phonetic_hash.insert("she",0);
	phonetic_hash.insert("shei",0);
	phonetic_hash.insert("shen",0);
	phonetic_hash.insert("sheng",0);
	phonetic_hash.insert("shi",0);
	phonetic_hash.insert("shou",0);
	phonetic_hash.insert("shu",0);
	phonetic_hash.insert("shua",0);
	phonetic_hash.insert("shuai",0);
	phonetic_hash.insert("shuan",0);
	phonetic_hash.insert("shuang",0);
	phonetic_hash.insert("shui",0);
	phonetic_hash.insert("shun",0);
	phonetic_hash.insert("shuo",0);
	phonetic_hash.insert("si",0);
	phonetic_hash.insert("song",0);
	phonetic_hash.insert("sou",0);
	phonetic_hash.insert("su",0);
	phonetic_hash.insert("suan",0);
	phonetic_hash.insert("sui",0);
	phonetic_hash.insert("sun",0);
	phonetic_hash.insert("suo",0);
	phonetic_hash.insert("ta",0);
	phonetic_hash.insert("tai",0);
	phonetic_hash.insert("tan",0);
	phonetic_hash.insert("tang",0);
	phonetic_hash.insert("tao",0);
	phonetic_hash.insert("te",0);
	phonetic_hash.insert("tei",0);
	phonetic_hash.insert("teng",0);
	phonetic_hash.insert("ti",0);
	phonetic_hash.insert("tian",0);
	phonetic_hash.insert("tiao",0);
	phonetic_hash.insert("tie",0);
	phonetic_hash.insert("ting",0);
	phonetic_hash.insert("tong",0);
	phonetic_hash.insert("tou",0);
	phonetic_hash.insert("tu",0);
	phonetic_hash.insert("tuan",0);
	phonetic_hash.insert("tui",0);
	phonetic_hash.insert("tun",0);
	phonetic_hash.insert("tuo",0);
	phonetic_hash.insert("wa",0);
	phonetic_hash.insert("wai",0);
	phonetic_hash.insert("wan",0);
	phonetic_hash.insert("wang",0);
	phonetic_hash.insert("wei",0);
	phonetic_hash.insert("wen",0);
	phonetic_hash.insert("weng",0);
	phonetic_hash.insert("wo",0);
	phonetic_hash.insert("wu",0);
	phonetic_hash.insert("xi",0);
	phonetic_hash.insert("xia",0);
	phonetic_hash.insert("xian",0);
	phonetic_hash.insert("xiang",0);
	phonetic_hash.insert("xiao",0);
	phonetic_hash.insert("xie",0);
	phonetic_hash.insert("xin",0);
	phonetic_hash.insert("xing",0);
	phonetic_hash.insert("xiong",0);
	phonetic_hash.insert("xiu",0);
	phonetic_hash.insert("xu",0);
	phonetic_hash.insert("xuan",0);
	phonetic_hash.insert("xue",0);
	phonetic_hash.insert("xun",0);
	phonetic_hash.insert("ya",0);
	phonetic_hash.insert("yan",0);
	phonetic_hash.insert("yang",0);
	phonetic_hash.insert("yao",0);
	phonetic_hash.insert("ye",0);
	phonetic_hash.insert("yi",0);
	phonetic_hash.insert("yin",0);
	phonetic_hash.insert("ying",0);
	phonetic_hash.insert("yo",0);
	phonetic_hash.insert("yong",0);
	phonetic_hash.insert("you",0);
	phonetic_hash.insert("yu",0);
	phonetic_hash.insert("yuan",0);
	phonetic_hash.insert("yue",0);
	phonetic_hash.insert("yun",0);
	phonetic_hash.insert("za",0);
	phonetic_hash.insert("zai",0);
	phonetic_hash.insert("zan",0);
	phonetic_hash.insert("zang",0);
	phonetic_hash.insert("zao",0);
	phonetic_hash.insert("ze",0);
	phonetic_hash.insert("zei",0);
	phonetic_hash.insert("zen",0);
	phonetic_hash.insert("zeng",0);
	phonetic_hash.insert("zha",0);
	phonetic_hash.insert("zhai",0);
	phonetic_hash.insert("zhan",0);
	phonetic_hash.insert("zhang",0);
	phonetic_hash.insert("zhao",0);
	phonetic_hash.insert("zhe",0);
	phonetic_hash.insert("zhei",0);
	phonetic_hash.insert("zhen",0);
	phonetic_hash.insert("zheng",0);
	phonetic_hash.insert("zhi",0);
	phonetic_hash.insert("zhong",0);
	phonetic_hash.insert("zhou",0);
	phonetic_hash.insert("zhu",0);
	phonetic_hash.insert("zhua",0);
	phonetic_hash.insert("zhuai",0);
	phonetic_hash.insert("zhuan",0);
	phonetic_hash.insert("zhuang",0);
	phonetic_hash.insert("zhui",0);
	phonetic_hash.insert("zhun",0);
	phonetic_hash.insert("zhuo",0);
	phonetic_hash.insert("zi",0);
	phonetic_hash.insert("zong",0);
	phonetic_hash.insert("zou",0);
	phonetic_hash.insert("zu",0);
	phonetic_hash.insert("zuan",0);
	phonetic_hash.insert("zui",0);
	phonetic_hash.insert("zun",0);
	phonetic_hash.insert("zuo",0);
}

void DataAnalysis::set_word_hash(QHash<QString, int> &word_hash)
{
	QFile file("words.txt");

	if(!file.open(QIODevice::ReadOnly ))
	{
		//打开错误
		QMessageBox::warning(this, "打开错误", "打开文件错误：" + file.errorString());
		return; //不处理文件
	}

	QString strLine;

	while(!file.atEnd())
	{
		strLine=file.readLine().trimmed();
		word_hash.insert(strLine,0);
	}

	file.close();

	qDebug()<<"complete!";
}

void DataAnalysis::on_buttonClear_clicked()
{
	ui->textResult->clear();
}

void DataAnalysis::wait()
{
	QElapsedTimer et;
	et.start();
	while(et.elapsed()<50){
		QApplication::processEvents();
	}
}
