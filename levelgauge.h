#ifndef LEVELGAUGE_H
#define LEVELGAUGE_H

//Qt
#include <QWidget>

//My
#include "subwindow.h"
#include "tdbquery.h"

namespace Ui {
class LevelGauge;
}

class LevelGauge : public SubWindow
{
    Q_OBJECT
private:
   struct TTankInfo
   {
       bool enabled = false;
       int volume = 0;
       int diametr = 0;
       QString product;
       QString description;
   };
   typedef QMap<int, TTankInfo> TAZSInfo;
   typedef QMap<QString, TAZSInfo> TAZSesInfo;  //key - azs code, AZS - info about tanks

public:
    explicit LevelGauge(const int id, QWidget *parent = nullptr);
    ~LevelGauge();

    const QString& typeWindow() const override { return TYPE_WINDOW; };

signals:
    void updateAZSesInfo();

private slots:
    void tankConfigFromDB(const TDBQuery::TResultRecords& dataFromDB);

private:
    void refreshData();
    void requestTankConfig();

private:
    static const qsizetype MAX_RECORDS_COUNT;
    static const QString TYPE_WINDOW;
    Ui::LevelGauge *ui;

    static TAZSesInfo AZSesInfo;
};

#endif // LEVELGAUGE_H
