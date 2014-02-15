#ifndef ADDVALUE_H
#define ADDVALUE_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class AddValue;
}

class MainWindow;

class AddValue : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddValue(QWidget *parent = 0);
    ~AddValue();

    void setMainWindow(MainWindow *mw);

private slots:
    void on_pushButtonClose_clicked();

    void on_comboBox_activated(int index);

    void on_pushButtonAdd_clicked();

private:
    Ui::AddValue *ui;

    QString type;

    MainWindow *mw;
};

#endif // ADDVALUE_H
