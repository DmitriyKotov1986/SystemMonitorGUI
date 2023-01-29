#include "levelgauge.h"
#include "ui_levelgauge.h"

//Qt
#include <QThread>

//Static const
const qsizetype LevelGauge::MAX_RECORDS_COUNT = 100000;
const QString LevelGauge::TYPE_WINDOW = "LevelGauge";

//Class
LevelGauge::LevelGauge(const int id, QWidget *parent) :
    SubWindow{id, parent},
    ui(new Ui::LevelGauge)
{
    ui->setupUi(this);
}

LevelGauge::~LevelGauge()
{
    delete ui;
}

void LevelGauge::tankConfigFromDB(const TDBQuery::TResultRecords& dataFromDB)
{
    AZSesInfo.clear();

    for (const auto& record : dataFromDB)
    {
        Q_ASSERT(record.size() == 6);
        if (record.size() < 6)
        {
            continue;
        }

        const QString AZSCode = record[0].toString();
        Q_ASSERT(!AZSCode.isEmpty());
        if (AZSCode.isEmpty())
        {
            continue;
        }
        const int tankNumber = record[1].toInt();

        TTankInfo tmp;
        tmp.enabled = record[2].toBool();
        tmp.volume = record[3].toInt();
        tmp.diametr = record[4].toInt();

        tmp.product = record[5].toString();
        Q_ASSERT(!tmp.product.isEmpty());
        if (tmp.product.isEmpty())
        {
            continue;
        }

        tmp.description = record[5].toString();

        auto AZSTanks = AZSesInfo[AZSCode];
        AZSTanks[tankNumber] = tmp;
    }

    emit updateAZSesInfo();
}

void LevelGauge::refreshData()
{
    for (auto AZS_it = AZSesInfo.begin(); AZS_it != AZSesInfo.end(); ++AZS_it)
    {
        auto AZSCode = AZS_it.key();
        auto AZSTanks = AZS_it.value();
        for (auto tank_it = AZSTanks.begin(); tank_it != AZSTanks.end(); ++tank_it)
        {
            auto tankNumber = tank_it.key();
            auto tankInfo = tank_it.value();

            ui->AZSTanksTreeView.
        }
    }
}

void LevelGauge::requestTankConfig()
{
    showProgressDialog(2, "Обработка запроса к БД...");


    QString queryText = QString("SELECT TOP (%1) "
                                "[AZSCode], [TankNumber], [Enabled], [Volume], [Diametr], [Product], [Description] "
                                "FROM [TanksInfo] "
                                "ORDER BY [AZSCode], [TankNumber] ")
                                .arg(MAX_RECORDS_COUNT);

    //qDebug() << queryText;

    TDBQuery* query = new TDBQuery(queryText);
    QThread* thread = new QThread();

    query->moveToThread(thread);

    QObject::connect(thread, SIGNAL(started()), query, SLOT(selectQuery()));
    QObject::connect(query, SIGNAL(selectResult(const TDBQuery::TResultRecords&)), SLOT(tankConfigFromDB(const TDBQuery::TResultRecords&)));
    QObject::connect(query, SIGNAL(errorOccurred(const QString&)), SLOT(errorOccurred(const QString&)));

    QObject::connect(query, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(query, SIGNAL(finished()), query, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();

    //далее ждем обработки слота tankConfigFromDB или ошибки
}
