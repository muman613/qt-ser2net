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
    connect(&telnet, &QTelnet::stateChanged, this, &MainWindow::stateChanged);

    telnet.connectToHost("wunderbar-ub18-dev", 2000);

#ifdef CMD_ON_TIMER
    QTimer::singleShot(1000, this, [=]() {
        sendCommand("version", [](QStringList & x) {
            qDebug() << x;
        });
    });
#endif
}

void MainWindow::sendCommand(const QString &cmd, CALLBACK cb)
{
    if (state != STATE_IDLE) {
        qDebug() << "Cannot issue new command while old command has not responded";
        return;
    }

    _cb = cb;

    command = cmd;
    response.clear();
    QByteArray ar = cmd.toLatin1();
    ar.append('\n');

    telnet.sendData(ar);
    state = STATE_INCMD;
}

void MainWindow::newData(const char *buff, int len) {
    QByteArray bytes(buff, len);
    bool bPrompt = false;

    qDebug() << Q_FUNC_INFO;

    if (bytes.endsWith("-> ")) {
        qDebug() << "Found prompt!";
        bPrompt = true;
    }

    if (state == STATE_INIT) {
        if (bPrompt) {
            qDebug() << "Received 1st prompt";
            state = STATE_IDLE;
            return;
        }
    }
    if (state != STATE_INCMD) {
        qDebug() << "Received data while not waiting for command response...";
        return;
    }

    response = bytes;

    if (_cb != nullptr) {
        QStringList strlst = response.split('\n');

        qDebug() << "Calling callback function...";
        (*_cb)(strlst);

        _cb = nullptr;
    }

    ui->mainText->append(response);
    QFile out("/tmp/netlog.log");

    if (out.open(QFile::WriteOnly|QFile::Append)) {
        out.write(buff, len);
    }

    state = STATE_IDLE;
}

void MainWindow::stateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << Q_FUNC_INFO << state;

    if (state == QAbstractSocket::ConnectedState) {
        QTimer::singleShot(1000, this, [=]() {
            sendCommand("version", [](QStringList & x) {
                qDebug() << x;
            });
        });
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
    sendCommand("version", [](QStringList & x) {
        qDebug() << x;
    });
}

void MainWindow::on_actionShowPort_triggered()
{
    sendCommand("showport", [](QStringList & x) {
        qDebug() << x;
    });
}

void MainWindow::on_actionClear_triggered()
{
    ui->mainText->clear();
}

void MainWindow::on_actionSave_triggered()
{

}
