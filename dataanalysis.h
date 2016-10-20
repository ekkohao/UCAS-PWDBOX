#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H

#include <QDialog>
#include <QString>
#include <QHash>
#include <QThread>

namespace Ui {
	class DataAnalysis;
}

class DataAnalysis : public QDialog
{
	Q_OBJECT
public:
	explicit DataAnalysis(QWidget *parent = 0);
	~DataAnalysis();

	//返回键盘位置
	static void find_keyboard_loc(QChar c,int &i,int &j);

public slots:

private slots:
	void on_buttonOpenFile_clicked();

	void on_buttonAnalysis_clicked();

	void on_buttonClear_clicked();

private:
	Ui::DataAnalysis *ui;

	void wait();

	//成分分析
	void composition_ana();
	//键盘密码构成分析
	void keyboard_ana();
	//日期密码构成分析
	void date_ana();
	//拼音top10
	void phonetic_top10_ana();
	//英文单词top10
	void word_top10_ana();

	//对source_hash生成一个排序的sort_hash
	void hash_sort_insert(QHash<int,QString> &sort_hash,QHash<QString,int> &source_hash);

	//是否符合日期格式
	bool is_date_format(QString &strT);

	bool set_date_format(QStringRef strT, int format);

	void find_phonetic(QString s,QHash<QString, int> &phonetic_hash);

	void find_word(QString s,QHash<QString, int> &word_hash);

	void set_word_hash(QHash<QString,int> &word_hash);

	void set_phonetic_hash(QHash<QString,int> &phonetic_hash);

};

//class SleeperThread : public QThread
//{
//public:
//	static void msleep(unsigned long msecs)
//	{
//		QThread::msleep(msecs);
//	}
//};

#endif // DATAANALYSIS_H
