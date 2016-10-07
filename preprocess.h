#ifndef PREPROCESS_H
#define PREPROCESS_H


#include <QDialog>


namespace Ui {
	class PreProcess;
}

class PreProcess : public QDialog
{
	Q_OBJECT
public:
	explicit PreProcess(QWidget *parent = 0);
	~PreProcess();
private slots:
	void on_buttonOpenFile_clicked();

	void on_buttonProcess_clicked();

	void on_comboBoxMode_currentIndexChanged(int index);

	void on_buttonClear_clicked();

private:
	Ui::PreProcess *ui;
};

#endif // PREPROCESS_H
