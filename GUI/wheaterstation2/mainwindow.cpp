#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectToBluetoothDevice();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete socket;
}

void MainWindow::readBluetoothData()
{

    /*
    QByteArray message = QByteArray::number(1, 10);
    socket->write(message + "\n");
    */

    QString str = "1\n";
    QByteArray message = str.toUtf8();
    socket->write(message);

    QObject::connect(socket, &QBluetoothSocket::readyRead, [&](){
        QByteArray data = socket->readAll();
       // QString message = QString::fromLatin1(data.constData(), data.length());
        qDebug() << "Received message: " << data;


        QList<QByteArray> inputList = data.split('\n');
        if (inputList.size() == 4) {
            float temp = inputList[0].toFloat();
            QString myString = QString::number(temp);
           // myString.remove(0, 1);
            ui->temp->setText(myString + " °C");
            qDebug() << "First value: " << temp;

            float wind = inputList[1].toFloat();
            ui->win->setText(QString::number(wind) + " m/s");
            qDebug() << "Second value: " << wind;

            float solar = inputList[2].toFloat();
            ui->sol->setText(QString::number(solar) + " W/m^2");
            qDebug() << "Third value: " << solar;
        }
        QObject::disconnect(socket, &QBluetoothSocket::readyRead, nullptr, nullptr);
    });
}

void MainWindow::askTime()
{
    //QByteArray message = QByteArray::number(2, 10);
    QDateTime dateTime = QDateTime::currentDateTime();
    int dayOfWeek = dateTime.date().dayOfWeek();

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("yyyyddMMhhmmss");
    QByteArray a = formattedDateTime.toUtf8();
    qDebug() << "Date and time: " << a;
    QString message = "2\n" + formattedDateTime + QString::number(dayOfWeek);
    QByteArray messageData = message.toUtf8();
    socket->write(messageData);
}


void MainWindow::connectToBluetoothDevice()
{
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    socket->connectToService(QBluetoothAddress("=JVJ-01"), QBluetoothUuid(QBluetoothUuid::SerialPort));

    QObject::connect(socket, &QBluetoothSocket::stateChanged, [&](QBluetoothSocket::SocketState state){

    if (state == QBluetoothSocket::ConnectedState) {
        qDebug() << "Connected to HC-05";
        }
    });
}


void MainWindow::on_Update_clicked()
{
    readBluetoothData();
}


void MainWindow::on_TimeButton_clicked()
{
    askTime();
}


void MainWindow::on_getHistorical_clicked()
{
    QString str = "3\n";
    QByteArray message = str.toUtf8();
    socket->write(message);

    QObject::connect(socket, &QBluetoothSocket::readyRead, [&](){
        QByteArray data = socket->readAll();
        qDebug() << "Received message: " << data;

        //float dataa = data.toFloat();

        ui->textBrowser->setText(data);


        QObject::disconnect(socket, &QBluetoothSocket::readyRead, nullptr, nullptr);
    });
}

