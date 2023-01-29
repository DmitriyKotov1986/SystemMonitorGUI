#ifndef LOGS_H
#define LOGS_H

//Qt
#include <QSqlDatabase>
#include <QAction>

//My
#include "Common/tdbloger.h"
#include "tconfig.h"
#include "tdbquery.h"
#include "subwindow.h"

namespace Ui
{
    class Logs;
}

class Logs : public SubWindow
{
    Q_OBJECT

public:
    explicit Logs(const int id, QWidget *parent = nullptr);
    ~Logs();

    const QString& typeWindow() const override { return TYPE_WINDOW; };

public slots:
    void selectResult(const TDBQuery::TResultRecords& result);
    void errorOccurred(const QString& Msg);

signals:
    void selectQuery(const QString& queryText);

private slots:
    void search();
    void saveToFile();

private:
    void saveSettings();
    void loadSettings();

    void setEnabled(bool enabled);

private:
    static const qsizetype MAX_RECORDS_COUNT;
    static const QString TYPE_WINDOW;

    Ui::Logs *ui = nullptr;

    TConfig* _cnf = nullptr;
    Common::TDBLoger *_loger = nullptr;

    QString _pathForSaveFile;
};

#endif // LOGS_H
