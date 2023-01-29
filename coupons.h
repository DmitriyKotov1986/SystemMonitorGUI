#ifndef COUPONS_H
#define COUPONS_H

#include <QWidget>

namespace Ui {
class Coupons;
}

class Coupons : public QWidget
{
    Q_OBJECT

public:
    explicit Coupons(QWidget *parent = nullptr);
    ~Coupons();

private:
    Ui::Coupons *ui;
};

#endif // COUPONS_H
