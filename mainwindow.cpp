#include "mainwindow.h"
#include "ui_mainwindow.h"

//My
#include "logs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _cnf(TConfig::config())
{
    Q_CHECK_PTR(_cnf);

    ui->setupUi(this);

    //настраиваем подключение БД логирования
    _logdb = QSqlDatabase::addDatabase(_cnf->db_Driver(), "LoglogDB");
    _logdb.setDatabaseName(_cnf->db_DBName());
    _logdb.setUserName(_cnf->db_UserName());
    _logdb.setPassword(_cnf->db_Password());
    _logdb.setConnectOptions(_cnf->db_ConnectOptions());
    _logdb.setPort(_cnf->db_Port());
    _logdb.setHostName(_cnf->db_Host());

    _loger = Common::TDBLoger::DBLoger(&_logdb, _cnf->sys_DebugMode());

    QObject::connect(ui->actionLogs, SIGNAL(triggered()), SLOT(showLogs()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLogs()
{
    Logs* logsWindow = new Logs();

   // addDockWidget(Qt::LeftDockWidgetArea, logsWindow);
    addDockWidget(Qt::TopDockWidgetArea, logsWindow);
    logsWindow->show();
}

