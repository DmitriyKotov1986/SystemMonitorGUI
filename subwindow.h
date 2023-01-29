#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QObject>
#include <QWidget>
#include <QProgressDialog>

class SubWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SubWindow(const int id, QWidget *parent = nullptr);

    int id() const { return _id; }
    virtual const QString& typeWindow() const = 0;

protected:
    void showProgressDialog(int stepCount, const QString& labelText);
    void closeProgressDialog();
    void setValueProgressDialog(int step, const QString& labelText);

private:
    const int _id = 0;

    QProgressDialog* _progress = nullptr;

};

#endif // SUBWINDOW_H
