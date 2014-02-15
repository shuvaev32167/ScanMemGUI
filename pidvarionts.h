#ifndef PIDVARIONTS_H
#define PIDVARIONTS_H

#include <QMainWindow>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include "mainwindow.h"

extern long PID;

namespace Ui {
class PidVarionts;
}

class MainWindow;

class PidVarionts : public QMainWindow
{
    Q_OBJECT

public:
    explicit PidVarionts(QWidget *parent = 0);
    ~PidVarionts();

    void setMainWindows (MainWindow * const w);

    void refreshProcess();

    void refreshTable();
private slots:
    void on_pushButton_clicked();


    void on_lineEditProcName_textChanged(const QString &arg1);

    void on_lineEditUser_textChanged(const QString &arg1);

    void on_tableProcess_doubleClicked(const QModelIndex &index);

public slots:


private:
    Ui::PidVarionts *ui;
    MainWindow *mainWindow;

    QProcess pr;
//    QThread qthr;

    bool flagExit;

    QStringList qsl, globalQsl;

    std::thread *thr;
protected:
    virtual bool eventFilter(QObject *object, QEvent *event); //Перехватчик событий
};

#endif // PIDVARIONTS_H
