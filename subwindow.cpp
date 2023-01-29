#include "subwindow.h"

SubWindow::SubWindow(const int id, QWidget *parent)
    : QWidget{parent}
    , _id(id)
{

}

void SubWindow::showProgressDialog(int stepCount, const QString& labelText)
{
    if (_progress != nullptr)
    {
        closeProgressDialog();
    }

    _progress = new QProgressDialog(this);
    _progress->setCancelButton(nullptr);
    _progress->setRange(0, stepCount);
    _progress->setWindowModality(Qt::WindowModal);

    setValueProgressDialog(0, labelText);

    _progress->show();
}

void SubWindow::closeProgressDialog()
{
    Q_CHECK_PTR(_progress);

    if (_progress == nullptr)
    {
        return;
    }

    setValueProgressDialog(2, "Готово.");

    _progress->close();
    _progress->deleteLater();

    _progress = nullptr;
}

void SubWindow::setValueProgressDialog(int step, const QString &labelText)
{
    Q_CHECK_PTR(_progress);

    if (_progress == nullptr)
    {
        return;
    }

    _progress->setLabelText(labelText);
    _progress->setValue(step);
}
