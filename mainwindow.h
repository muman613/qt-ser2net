#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTelnet.h>

namespace Ui {
class MainWindow;
}

typedef void (*CALLBACK)(QStringList & data);

class MainWindow : public QMainWindow
{
    Q_OBJECT

    typedef enum {
        STATE_INIT,
        STATE_IDLE,
        STATE_INCMD,
    } STATE;

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void    sendCommand(const QString & cmd, CALLBACK cb = nullptr);

signals:

protected slots:
    void newData(const char *buff, int len);
    void stateChanged(QAbstractSocket::SocketState state);

protected:
    QTelnet                         telnet;

private slots:
    void on_sendButton_clicked();
    void on_actionHelp_triggered();
    void on_actionVersion_triggered();
    void on_actionShowPort_triggered();

    void on_actionClear_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *                ui                  = nullptr;
    bool                            bCmdInProgress      = false;
    QString                         command;
    QString                         response;
    CALLBACK                        _cb                 = nullptr;

    STATE                           state               = STATE_INIT;
};

#endif // MAINWINDOW_H
