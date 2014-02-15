#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include <QKeyEvent>
#include <QCheckBox>
#include <unistd.h>
#include <thread>
#include <iostream>
#include "pidvarionts.h"
#include "mytablewidget.h"
#include "searchform.h"
#include "addvalue.h"

extern long PID;

extern bool flagDel;

void DumpToStandarnHex (QString &qs);

extern QStringList myData;

extern bool isEditData;

extern bool isEditPid0,isEditPid1;

//std::thread *underStream;

namespace Ui {
class MainWindow;
}

class PidVarionts;
class SearchForm;

extern bool flagExit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

      QProcess pr;
      Ui::MainWindow *ui;

      void listValue();

      bool isCorrectPid;

      void addValie (const QString &type, const QString &value);

private slots:
    void on_pushButtonPID_clicked();

    void on_pushButtonSearch_clicked();

    void on_pushButtonWeed_clicked();

    void on_tableAddr_cellDoubleClicked(int row, int column);
    void on_tableValue_cellChanged(int row, int column);

    void on_pushButtonAdd_clicked();

    void on_tableAddr_customContextMenuRequested(const QPoint &pos);

    void autoReplacementAddr ();

    void replacementAddr ();

private:

    SearchForm *sf;

//    std::thread *thr;

//    QThread *qthr;

protected:
    virtual bool eventFilter(QObject *, QEvent *); //Перехватчик событий

//    virtual void keyPressEvent(QKeyEvent *); //перехвадчик нажатий
};

#endif // MAINWINDOW_H
