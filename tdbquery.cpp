#include "tdbquery.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMutexLocker>

//My

TDBQuery::TDBQuery(const QString& queryText, QObject *parent)
    : QObject(parent)
    , _queryText(queryText)
    , _cnf(TConfig::config())
{
    Q_CHECK_PTR(_cnf);

    qRegisterMetaType<TDBQuery::TResultRecords>("TDBQuery::TResultRecords");

    _currentConnetionNumber = connectionNumber();

    //настраиваем подключение БД логирования
    _db = QSqlDatabase::addDatabase(_cnf->db_Driver(), QString("LogDB_%1").arg(_currentConnetionNumber));
    _db.setDatabaseName(_cnf->db_DBName());
    _db.setUserName(_cnf->db_UserName());
    _db.setPassword(_cnf->db_Password());
    _db.setConnectOptions(_cnf->db_ConnectOptions());
    _db.setPort(_cnf->db_Port());
    _db.setHostName(_cnf->db_Host());
}

TDBQuery::~TDBQuery()
{
    _db.removeDatabase(QString("LogDB_%1").arg(_currentConnetionNumber));
}

void TDBQuery::selectQuery()
{
    //подключаемся к БД
    if (!_db.open())
    {
        exitWithError(QString("Cannot connect to log database. Error: %1").arg(_db.lastError().text()));

        return;
    };

    QSqlQuery query(_db);
    query.setForwardOnly(true);

    if (!query.exec(_queryText))
    {
        exitWithError(QString("Cannot execute query. Error: %1. Query: %2").arg(query.lastError().text()).arg(query.lastQuery()));

        return;
    }

    auto record = query.record();
    TResultRecords result;

    while (query.next())
    {
        TRow row;
        for (int column = 0; column < record.count(); ++column)
        {
            row.push_back(query.value(column));
        }

        result.push_back(row);
    }

    emit selectResult(result);

    //отключаемся от БД
    _db.close();

    emit finished();
}

void TDBQuery::exitWithError(const QString &msg)
{
    emit errorOccurred(msg);
    emit finished();
}

int TDBQuery::_connectionNumber = 0;

int TDBQuery::connectionNumber()
{
    QMutexLocker locker(&_mutex);

    return _connectionNumber++;
}
