#include <QDebug>
#include <QFile>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug() << Q_FUNC_INFO;

    ui->setupUi(this);

    connect(&telnet, &QTelnet::newData, this, &MainWindow::newData);

    telnet.connectToHost("wunderbar-ub18-dev", 2000);

    QTimer::singleShot(200, this, [=]() {
        sendCommand("version");
    });

}

void MainWindow::sendCommand(const QString &cmd)
{
    command = cmd;
    response.clear();
    QByteArray ar = cmd.toLatin1();
    ar.append('\n');

    telnet.sendData(ar);
}

void MainWindow::newData(const char *buff, int len) {
    QByteArray bytes(buff, len);

    response = bytes;

    qDebug() << Q_FUNC_INFO;

    ui->mainText->append(response);
    QFile out("/tmp/netlog.log");

    if (out.open(QFile::WriteOnly|QFile::Append)) {
        out.write(buff, len);
    }
}

//void MainWindow::showEvent(QShowEvent *)
//{
//    qDebug() << Q_FUNC_INFO;

//    telnet.sendData("version\n");
//}

void MainWindow::on_sendButton_clicked()
{

}

void MainWindow::on_actionHelp_triggered()
{
    sendCommand("help");
}

void MainWindow::on_actionVersion_triggered()
{
    sendCommand("version");
}

void MainWindow::on_actionShowPort_triggered()
{
    sendCommand("showport");
}
