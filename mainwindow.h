#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//QT
#include <QMainWindow>
#include <QSqlDatabase>
#include <QMdiArea>
#include <QCloseEvent>

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

private slots:
    void showLogs();
    void showLevelGauge();
    void showAbout();

    void windowsTile();
    void windowsCascade();

private:
    void closeEvent(QCloseEvent *event) override;

    int currentIdSubWindow();

    void saveSettings();
    void loadSettings();

    QMdiSubWindow* showSubWindow(QWidget* subWidget);

private:
    Ui::MainWindow *_ui = nullptr;
    QMdiArea *_mdiArea = nullptr;

    TConfig* _cnf = nullptr;
    Common::TDBLoger *_loger = nullptr;

    QSqlDatabase _logdb;

    int _currentIdSubWindow = 0;
};

#endif // MAINWINDOW_H
