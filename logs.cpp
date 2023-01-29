#include "logs.h"
#include "ui_logs.h"

#include <QThread>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

//static const
const qsizetype Logs::MAX_RECORDS_COUNT = 100000;
const QString Logs::TYPE_WINDOW = "Logs";

//class
Logs::Logs(const int id, QWidget *parent) :
    SubWindow{id, parent},
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

    loadSettings();
}

Logs::~Logs()
{
    saveSettings();

    delete ui;
}

void Logs::selectResult(const TDBQuery::TResultRecords &result)
{
    setValueProgressDialog(1, "Обработка данных...");

    auto table = ui->logsTableWidget;

    auto recordCount = result.count();

  //   table->clear();
    table->setRowCount(recordCount);

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

    closeProgressDialog();

    if (recordCount >= MAX_RECORDS_COUNT)
    {
        QMessageBox::information(this, "SystemMonitorGUI Логирование", "Количество записей превышает допустимое. Отображены не все записи.",
            QMessageBox::Ok);
    }
    else if (recordCount == 0)
    {
        QMessageBox::information(this, "SystemMonitorGUI Логирование", "Записи удовлетворяющие запросу не найдены.",
            QMessageBox::Ok);
    }

    setEnabled(true);
}

void Logs::errorOccurred(const QString &Msg)
{
    closeProgressDialog();
    setEnabled(true);

    QMessageBox::warning(this, "SystemMonitorGUI Логирование", "Не удалось обработать запрос к БД.", QMessageBox::Ok);

    qDebug() << Msg;
}

void Logs::search()
{
    setEnabled(false);
    showProgressDialog(2, "Обработка запроса к БД...");

    auto UID = ui->UIDLineEdit->text();
    auto messageText = ui->messageLineEdit->text();
    auto sender = ui->moduleComboBox->currentText();

    QString queryText = QString("SELECT TOP (%1) [DateTime], [UID], [Sender], [Msg] "
                                "FROM [Log] "
                                "WHERE %2 AND %3 %4 %5 %6 "
                                "ORDER BY [ID] DESC ")
        .arg(MAX_RECORDS_COUNT)
        .arg("[DateTime] >= CAST('" +  ui->startDateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss") + "' AS DATETIME2)")
        .arg("[DateTime] <= CAST('" +  ui->endDateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss") + "' AS DATETIME2)")
        .arg(UID.isEmpty() ? "" : "AND [UID] LIKE '" + UID + "'")
        .arg(sender == "All" ? "" : "AND [Sender] = '" + sender + "'")
        .arg(messageText.isEmpty() ? "" : "AND [Msg] LIKE '" + messageText + "'");

    //qDebug() << queryText;

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

void Logs::saveToFile()
{
    auto fileName = QFileDialog::getSaveFileName(this, "Save log to file", _pathForSaveFile, "CSV files (*.csv)");
    if (fileName.isEmpty())
    {
        return;
    }

    //сохраняем путь к файлу
    QFileInfo fileInfo(fileName);
    _pathForSaveFile = fileInfo.absolutePath();

    //qDebug() << QString("Save log to file: %1").arg(fileName);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "SystemMonitorGUI Логирование", QString("Не удалось открыть файл для записи: %1").arg(fileName),
            QMessageBox::Ok);

        return;
    }

    QTextStream st(&file);
    for (int row = 0; row < ui->logsTableWidget->rowCount(); ++row)
    {
        QStringList tmp;
        for (int column = 0; column < ui->logsTableWidget->columnCount(); ++column)
        {
            tmp.push_back(QString("\"%1\"").arg(ui->logsTableWidget->item(row, column)->text()));
        }
        st << tmp.join(';') << "\n";
    }

    file.close();
}

void Logs::saveSettings()
{
    _cnf->setPathForSaveFile(_pathForSaveFile);
}

void Logs::loadSettings()
{
    Q_CHECK_PTR(_cnf);

    _pathForSaveFile = _cnf->pathForSaveFile();
}

void Logs::setEnabled(bool enabled)
{
    ui->startDateTimeEdit->setEnabled(enabled);
    ui->endDateTimeEdit->setEnabled(enabled);
    ui->UIDLineEdit->setEnabled(enabled);
    ui->messageLineEdit->setEnabled(enabled);
    ui->moduleComboBox->setEnabled(enabled);
    ui->searchPushButton->setEnabled(enabled);
    ui->savePushButton->setEnabled(enabled);
}
