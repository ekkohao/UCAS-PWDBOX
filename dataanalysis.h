#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H

#include <QDialog>
#include <QString>
#include <QHash>

namespace Ui {
	class DataAnalysis;
}

class DataAnalysis : public QDialog
{
	Q_OBJECT
public:
	explicit DataAnalysis(QWidget *parent = 0);
	~DataAnalysis();

public slots:

private slots:
	void on_buttonOpenFile_clicked();

	void on_buttonAnalysis_clicked();

	void on_buttonClear_clicked();

private:
	Ui::DataAnalysis *ui;

	//成分分析
	void composition_ana();
	//键盘密码构成分析
	void keyboard_ana();
	//日期密码构成分析
	void date_ana();
	//拼音top10
	void phonetic_top10_anay();
	//英文单词top10
	void word_top10_ana();

	//对source_hash生成一个排序的sort_hash
	void hash_sort_insert(QHash<int,QString> &sort_hash,QHash<QString,int> &source_hash);

	//返回键盘位置
	void find_keyboard_loc(QChar &c,int &i,int &j);

};

#endif // DATAANALYSIS_H
