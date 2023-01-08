#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//QT
#include <QMainWindow>
#include <QSqlDatabase>

//My
#include "tconfig.h"
#include "Common/tdbloger.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showLogs();
    void showLevelGauge();

private:
    Ui::MainWindow *ui = nullptr;

    TConfig* _cnf = nullptr;
    Common::TDBLoger *_loger = nullptr;

    QSqlDatabase _logdb;

};
#endif // MAINWINDOW_H
