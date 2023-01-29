#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QLabel>
#include <QMdiSubWindow>
#include <QMessageBox>

//My
#include "logs.h"
#include "levelgauge.h"
#include "about.h"
#include "subwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _mdiArea(new QMdiArea)
    , _cnf(TConfig::config())
{
    Q_CHECK_PTR(_cnf);

    _ui->setupUi(this);

    setCentralWidget(_mdiArea);
    _mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //настраиваем подключение БД логирования
    _logdb = QSqlDatabase::addDatabase(_cnf->db_Driver(), "LoglogDB");
    _logdb.setDatabaseName(_cnf->db_DBName());
    _logdb.setUserName(_cnf->db_UserName());
    _logdb.setPassword(_cnf->db_Password());
    _logdb.setConnectOptions(_cnf->db_ConnectOptions());
    _logdb.setPort(_cnf->db_Port());
    _logdb.setHostName(_cnf->db_Host());

    _loger = Common::TDBLoger::DBLoger(&_logdb, _cnf->sys_DebugMode());

    //выполняем коннект сигналов/слотов

    //Восстанавливаем геометрию окн прочие настройки
    loadSettings();
}

MainWindow::~MainWindow()
{
    TConfig::deleteConfig();

    delete _ui;
    delete _mdiArea;
}

void MainWindow::showLogs()
{
    showSubWindow(new Logs(currentIdSubWindow(), this));
}

void MainWindow::showLevelGauge()
{
    showSubWindow(new LevelGauge(currentIdSubWindow(), this));
}

void MainWindow::showAbout()
{
    auto aboutWindow = new About;

    aboutWindow->exec();
}

void MainWindow::windowsTile()
{
    _mdiArea->tileSubWindows();
}

void MainWindow::windowsCascade()
{
    _mdiArea->cascadeSubWindows();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, "SystemMonitorGUI", "Завершить работу?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
    {
        event->ignore();

        return;
    }

    saveSettings();
    event->accept();
}

int MainWindow::currentIdSubWindow()
{
   return _currentIdSubWindow++;
}

void MainWindow::saveSettings()
{
    Q_CHECK_PTR(_cnf);

    _cnf->clearWindowParam();

    _cnf->setWindowParam("MainWindow", "Geometry", saveGeometry());
    _cnf->setWindowParam("MainWindow", "State", saveState());

    _cnf->setWindowParam("SubWindow", "CurrentIDSubWindow", _currentIdSubWindow);
    QStringList subWindowsIdList;
    auto subWindowsList = _mdiArea->subWindowList();
    for (const auto& subWindowItem : subWindowsList)
    {
        if (subWindowItem->isVisible())
        {
            auto subWindow = static_cast<SubWindow*>(subWindowItem->widget());
            auto id = subWindow->id();

            subWindowsIdList.push_back(QString::number(id));

            _cnf->setWindowParam("SubWindow", QString("%1_Geometry").arg(id), subWindowItem->saveGeometry());
            _cnf->setWindowParam("SubWindow", QString("%1_Type").arg(id), subWindow->typeWindow());
            _cnf->setWindowParam("SubWindow", QString("%1_Active").arg(id), (subWindowItem == _mdiArea->activeSubWindow()));
        }

    }
    _cnf->setWindowParam("SubWindow", "List", subWindowsIdList);
}

void MainWindow::loadSettings()
{
    Q_CHECK_PTR(_cnf);

    restoreGeometry(_cnf->windowParam("MainWindow", "Geometry").toByteArray());
    restoreState(_cnf->windowParam("MainWindow", "State").toByteArray());

    QMdiSubWindow* activeSubWindow = nullptr;
    for (const auto& subWindowId : _cnf->windowParam("SubWindow", "List").toStringList())
    {
        auto type = _cnf->windowParam("SubWindow", QString("%1_Type").arg(subWindowId)).toString();
        if (type == "Logs")
        {
            auto subWindowWidget = new Logs(subWindowId.toInt(), this);
            auto subWindow = showSubWindow(subWindowWidget);

            subWindow->restoreGeometry(_cnf->windowParam("SubWindow", QString("%1_Geometry").arg(subWindowId)).toByteArray());

            if (_cnf->windowParam("SubWindow", QString("%1_Active").arg(subWindowId)).toBool())
            {
                activeSubWindow = subWindow;
            }
        }
        else
        {
            qDebug() << "Undefine type subwindows. ID:" << type;
        }
    }

    _mdiArea->setActiveSubWindow(activeSubWindow);

    _currentIdSubWindow = _cnf->windowParam("SubWindow", "CurrentIDSubWindow").toInt();
}

QMdiSubWindow* MainWindow::showSubWindow(QWidget* subWidget)
{
    auto subWindow = _mdiArea->addSubWindow(subWidget);
    subWindow->show();

    return subWindow;
}

