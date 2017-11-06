#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);

	// das is tileh!
	setWindowTitle("Maddavo System/Station CSV to MySQL Converter v0.1");

	// disable pushbuttons at start so user will have to follow correct order ;)
	ui->pushButton_convertStations->setDisabled(true);
	ui->pushButton_convertSystems->setDisabled(true);
	ui->pushButton_Station->setDisabled(true);
}

Widget::~Widget()
{
	delete ui;
}

/*
 * load Station.csv
 * */
void Widget::on_pushButton_Station_clicked()
{
	stationFileName = QFileDialog::getOpenFileName(this, tr("CSV"), "", tr("CSV files (*.csv)"));
	ui->textEdit->append("File selected: " + stationFileName);
	ui->pushButton_convertStations->setEnabled(true);
}


/*
 * load System.csv
 * */
void Widget::on_pushButton_System_clicked()
{
	systemFileName = QFileDialog::getOpenFileName(this, tr("CSV"), "", tr("CSV files (*.csv)"));
	ui->textEdit->append("File selected: " + systemFileName);
	ui->pushButton_convertSystems->setEnabled(true);
}


/*
 * convert stations into sql format
 */
void Widget::on_pushButton_convertStations_clicked()
{
	ui->textEdit->append("system_names size is: " + QString::number(system_names.size()));

	// read
	QFile file(stationFileName);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);

	// write
	QFile outFile(stationFileName + ".sql");
	outFile.open(QIODevice::WriteOnly);
	QTextStream out(&outFile);

	// header
	in.readLine();

	QString line,stub;
	QStringList list;
	int id=0;

	// insert sql header
	out << "INSERT INTO `station` (`station_id`, `name`, `system_id`, `ls_from_star`, `max_pad_size`, `modified`, `planetary`) VALUES";

	while (!in.atEnd())
	{
		// sql ID
		id++;

		line = in.readLine();
		list = line.split(",");
		//ui->textEdit->append("System: " + list[0] + ", Station: " + list[1] + ", Ls: " + list[2] + ", Pad: " + list[4]);

		// clear the qstring just in case
		stub.clear();

		//(18, 'export unique objects list.', 'describe me please', 1230764400, '2016-09-01 08:55:35', 'coding', 0, 0),
		// start making new qstring of the current sql line

		// station_id
		// first line requires \n at start
		stub = "\n(" + QString::number(id) + ", ";

		// station name
		stub.append(list[1] + ", ");

		// system_id
		// extract system_id from our qvector system_names
		stub.append(QString::number(system_names.indexOf(list[0])) + ", ");

		// distance from star
		stub.append(list[2] + ", ");

		// max pad size
		stub.append(list[4] + ", ");

		// modified
		stub.append("now(), ");

		// planetary
		stub.append(list[12] + ")");

		// check if we are at the end of the FILE
		if (!in.atEnd())
			stub.append(",");
		else
			stub.append(";");

		// write our sql line
		out << stub;
	}

	file.close();
	outFile.close();

	ui->textEdit->append("Station.csv Converted!");
}


/*
 * convert systems into sql format
 */
void Widget::on_pushButton_convertSystems_clicked()
{
	// read
	QFile file(systemFileName);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);

	// write
	QFile outFile(systemFileName + ".sql");
	outFile.open(QIODevice::WriteOnly);
	QTextStream out(&outFile);

	// header
	in.readLine();

	QString line,stub;
	QStringList list;
	int id=0;

	// insert sql header
	out << "INSERT INTO `system` (`system_id`, `name`, `pos_x`, `pos_y`, `pos_z`, `added_by`, `modified`) VALUES";

	while (!in.atEnd())
	{
		// sql ID
		id++;

		line = in.readLine();
		list = line.split(",");

		// clear the qstring just in case
		stub.clear();

		// start making new qstring of the current sql line

		// system_id
		// first line requires \n at start
		stub = "\n(" + QString::number(id) + ", ";

		// system name
		// first into QVector
		if (!system_names.contains(list[0])) system_names.push_back(list[0]);
		// then into our textstream stub
		stub.append(list[0] + ", ");

		// pos_x
		stub.append(list[1] + ", ");

		// pos_y
		stub.append(list[2] + ", ");

		// pos_z
		stub.append(list[3] + ", ");

		// added_by
		stub.append(list[4] + ", ");

		// modified
		stub.append("now())");

		// check if we are at the end of the FILE
		if (!in.atEnd())
			stub.append(",");
		else
			stub.append(";");

		// write our sql line
		out << stub;
	}

	file.close();
	outFile.close();

	ui->textEdit->append("System.csv Converted!");

	// enable pushbuttons
	ui->pushButton_Station->setEnabled(true);
}
