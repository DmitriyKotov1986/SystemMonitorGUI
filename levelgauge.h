#ifndef LEVELGAUGE_H
#define LEVELGAUGE_H

#include <QDockWidget>

namespace Ui {
class LevelGauge;
}

class LevelGauge : public QDockWidget
{
    Q_OBJECT

public:
    explicit LevelGauge(QWidget *parent = nullptr);
    ~LevelGauge();

private:
    Ui::LevelGauge *ui;
};

#endif // LEVELGAUGE_H
