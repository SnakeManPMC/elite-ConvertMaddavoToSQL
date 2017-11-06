#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

private slots:
	void on_pushButton_Station_clicked();

	void on_pushButton_System_clicked();

	void on_pushButton_convertStations_clicked();

	void on_pushButton_convertSystems_clicked();

private:
	Ui::Widget *ui;

	QString systemFileName,stationFileName;
	QVector<QString> system_names;
};

#endif // WIDGET_H
