#include "coupons.h"
#include "ui_coupons.h"

Coupons::Coupons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Coupons)
{
    ui->setupUi(this);
}

Coupons::~Coupons()
{
    delete ui;
}
