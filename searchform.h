#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class SearchForm;
}

extern bool isStop;

class MainWindow;

class SearchForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = 0);
    ~SearchForm();

    Ui::SearchForm *ui;

    bool flagSearch;

    void setMainWindow(MainWindow * mw);

private slots:

    void on_pushButtonClose_clicked();

    void on_comboBox_activated(int index);

    void on_pushButtonSearch_clicked();

    void on_horizontalSlider_valueChanged(int value);

public slots:
    virtual void show();

private:
    MainWindow *mw;
};

#endif // SEARCHFORM_H
