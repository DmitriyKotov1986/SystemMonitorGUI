#include "levelgauge.h"
#include "ui_levelgauge.h"

LevelGauge::LevelGauge(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::LevelGauge)
{
    ui->setupUi(this);
}

LevelGauge::~LevelGauge()
{
    delete ui;
}
