#ifndef LOGS_H
#define LOGS_H

#include <QDockWidget>
#include <QSqlDatabase>
#include <QAction>

//My
#include "Common/tdbloger.h"
#include "tconfig.h"
#include "tdbquery.h"

namespace Ui
{
    class Logs;
}

class Logs : public QDockWidget
{
    Q_OBJECT

public:
    explicit Logs(QWidget *parent = nullptr);
    ~Logs();

    const QString& errorString() const { return _errorString; }
    bool isError() const { return !_errorString.isEmpty(); }

public slots:
    void selectResult(const TDBQuery::TResultRecords& result);
    void errorOccurred(const QString& Msg);

signals:
    void selectQuery(const QString& queryText);

private slots:
    void on_actionSearch_triggered();

private:
    Ui::Logs *ui;

    TConfig* _cnf = nullptr;
    Common::TDBLoger *_loger = nullptr;

    QString _errorString;
};

#endif // LOGS_H
