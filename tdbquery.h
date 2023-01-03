#ifndef TDBQUERY_H
#define TDBQUERY_H

//Qt
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QVariant>
#include <QMutex>

//My
#include "tconfig.h"

class TDBQuery : public QObject
{
    Q_OBJECT

public:
    typedef QVector<QVariant> TRow;
    typedef QVector<TRow> TResultRecords;

public:
    explicit TDBQuery(const QString& queryText, QObject *parent = nullptr);
    ~TDBQuery();

public slots:
    void selectQuery();

signals:
    void selectResult(const TDBQuery::TResultRecords& result);
    void errorOccurred(const QString& Msg);
    void finished();

private:
    void exitWithError(const QString& msg);

    int connectionNumber();

private:
    TConfig* _cnf = nullptr;

    const QString _queryText;

    QSqlDatabase _db;

    static int _connectionNumber; //общая статическая переменная
    int _currentConnetionNumber;
    QMutex _mutex;

};

Q_DECLARE_METATYPE(TDBQuery::TResultRecords);

#endif // TDBQUERY_H
