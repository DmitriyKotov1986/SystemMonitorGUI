#include "logs.h"
#include "ui_logs.h"

#include<QThread>
#include<QDebug>


Logs::Logs(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::Logs),
    _cnf(TConfig::config()),
    _loger(Common::TDBLoger::DBLoger())
{
    Q_CHECK_PTR(_cnf);
    Q_CHECK_PTR(_loger);

    ui->setupUi(this);

    auto table = ui->logsTableWidget;
    table->horizontalHeader()->setStretchLastSection(true);

    ui->startDateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

Logs::~Logs()
{
    delete ui;
}

void Logs::selectResult(const TDBQuery::TResultRecords &result)
{
    auto table = ui->logsTableWidget;

 //   table->clear();
    table->setRowCount(result.count());

    int row = 0;

    for (auto it_rowRecord = result.begin(); it_rowRecord != result.end(); ++it_rowRecord)
    {
        auto rowRecord = (*it_rowRecord);

        Q_ASSERT(rowRecord.count() == table->columnCount());

        QTableWidgetItem* itemDateTime = new QTableWidgetItem(rowRecord[0].toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
        itemDateTime->setFlags(itemDateTime->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 0, itemDateTime);

        QTableWidgetItem* itemUID = new QTableWidgetItem(rowRecord[1].toString());
        itemUID->setFlags(itemDateTime->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 1, itemUID);

        QTableWidgetItem* itemModule = new QTableWidgetItem(rowRecord[2].toString());
        itemModule->setFlags(itemDateTime->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 2, itemModule);

        QTableWidgetItem* itemMessage = new QTableWidgetItem(rowRecord[3].toString());
        itemMessage->setFlags(itemDateTime->flags() & ~Qt::ItemIsEditable);
        table->setItem(row, 3, itemMessage);

        table->setRowHeight(row, 10);

        ++row;
    }
}

void Logs::errorOccurred(const QString &Msg)
{
    qDebug() << Msg;
}

void Logs::on_actionSearch_triggered()
{
    QString queryText = QString("SELECT TOP (10000) [DateTime], [UID], [Sender], [Msg] "
                                "FROM [Log] "
                                "ORDER BY [ID] DESC ");

    TDBQuery* query = new TDBQuery(queryText);
    QThread* thread = new QThread();

    query->moveToThread(thread);

    QObject::connect(thread, SIGNAL(started()), query, SLOT(selectQuery()));
    QObject::connect(query, SIGNAL(selectResult(const TDBQuery::TResultRecords&)), SLOT(selectResult(const TDBQuery::TResultRecords&)));
    QObject::connect(query, SIGNAL(errorOccurred(const QString&)), SLOT(errorOccurred(const QString&)));

    QObject::connect(query, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(query, SIGNAL(finished()), query, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

