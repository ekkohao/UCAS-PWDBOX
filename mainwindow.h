#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	int rand_seed=0;
	Ui::MainWindow *ui;
	void widget_init();
	void save(QStringList &strList);
	QString random_qchar_in_qstring(QString qstr, int len);
	void produce_weak(QStringList &strList, int total);
	void produce_kb(QStringList &strList, int total);
	void produce_date(QStringList &strList, int total);
	void produce_pinyin(QStringList &strList, int total);
	void produce_word(QStringList &strList, int total);
	void wait();
	QStringList get_pinyin_list();
	QStringList get_word_list();
	void print_value_result(int iScore,QStringList &msgList);
private slots:
	void slot_open_preprocess_ui();
	void slot_open_dataanalysis_ui();
	void on_buttonOpenDic_clicked();
	void on_buttonProduce_clicked();
	void on_action_close_triggered();
	void on_action_about_triggered();
	void on_pushButtonValue_clicked();
};


#endif // MAINWINDOW_H
