#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTelnet.h>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void    sendCommand(const QString & cmd);

signals:

protected slots:
    void newData(const char *buff, int len);

protected:
    QTelnet                         telnet;

//    void                            showEvent(QShowEvent *);
private slots:
    void on_sendButton_clicked();
    void on_actionHelp_triggered();
    void on_actionVersion_triggered();
    void on_actionShowPort_triggered();

private:
    Ui::MainWindow *                ui                  = nullptr;
    QString                         command;
    QString                         response;
};

#endif // MAINWINDOW_H
