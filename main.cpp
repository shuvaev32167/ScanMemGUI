#include "mainwindow.h"
#include <QApplication>
#include <thread>
#include "ui_mainwindow.h"

long PID = 0;
bool flagExit;

bool flagDel;

void underThread(Ui::MainWindow *ui);
void update(Ui::MainWindow *ui, MainWindow *w);
void hold(Ui::MainWindow *ui, MainWindow *w);
//QString HexToInt (const QString &qs);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*QString qs = "0C 00 00 00 00 00 00 00";
    qDebug() << qs;
    DumpToStandarnHex(qs);
    qDebug() << qs;
    qDebug() << qs.toUInt(0,16);
    qDebug() << QString::number(qs.toUInt(0,16), 16);*/
    MainWindow w;
    PID= -1;
    flagExit = false;
    std::thread th(underThread, w.ui);
    std::thread thUpdate(update, w.ui, &w);
    std::thread thHold(hold, w.ui, &w);
    w.show();
    //th.detach();
    a.exec();
    flagExit = true;
    QTime timer;
    timer.start();
    while (timer.elapsed() <= 10000);
    {
        if (!th.joinable() && !thUpdate.joinable() && !thHold.joinable())
            return 0;
    }
//    th.join();
//    thUpdate.join();
//    thHold.join();
    return 0;
}

void DumpToStandarnHex (QString &qs)
{
    qDebug() <<qs;
    QStringList qsl =qs.split(' ');
    qs="";
    int i=qsl.count()-1;
    for (;i>=0;i--)
    {
        qs+=qsl[i];
    }
    qDebug() <<qs;
//    return;
}

void update(Ui::MainWindow *ui, MainWindow *w)
{
    QString qs;
    QProcess qpr;
    qpr.start("scanmem");
    qpr.waitForStarted();
    QTime time;
    while (1)
    {
        time.start();
        if (isEditPid0)
        {
            qs= "pid "+QString::number(PID)+'\n';
            qpr.write(qs.toLatin1());
            qpr.waitForBytesWritten();
            isEditPid0=false;
            qDebug() << "Изменение PID";
            ui->pushButtonWeed->setEnabled(0);
            qDebug() << PID;
        }
        if (flagExit)
        {
            qpr.close();
            qpr.waitForFinished();
            return;
        }
        if (w->pr.isOpen())
        {
            for (int i=0; i<ui->tableValue->rowCount(); i++)
            {
                if (isStop || isEditData||flagExit||flagDel)
                    break;
                if (/*ui->tableValue->item(i,0) != 0 &&*/
                        ui->tableValue->item(i,myTableWidget::enumColomn::addr) !=0 &&
                        ui->tableValue->item(i,myTableWidget::enumColomn::type) !=0 &&
                        ui->tableValue->item(i,myTableWidget::enumColomn::value) !=0)
                {
                    qDebug() << "Обновление (i = "<<i<<")";
                    if (/*dynamic_cast<QCheckBox *>(ui->tableValue->item(i,0))->isChecked()*/
                            !dynamic_cast<QCheckBox *>(ui->tableValue->cellWidget(i,myTableWidget::enumColomn::hold))->isChecked())
                    {
                        qDebug() << "Обновление";
                        if (isStop || isEditData||flagExit||flagDel)
                        {
                            break;
                        }
                        qDebug() << "Обновление - 1";
                        QString buf, type = ui->tableValue->item(i,myTableWidget::enumColomn::type)->text().split('-').first().toLower();
                        qDebug() << "type = "<< type;
                        if (type == "i64" || type == "i64s" || type == "i64u" || type == "f64" || type == "f64s" || type == "f64u")
                            buf="8";
                        else
                            if (type == "i32" || type == "i32s" || type == "i32u" || type == "f32" || type == "f32s" || type == "f32u")
                                buf ="4";
                            else
                                if (type == "i16" || type == "i16s" || type == "i16u")
                                    buf ="2";
                                else
                                    if (type == "i8" || type == "i8s" || type == "i8u")
                                        buf = "1";
                        qs=qpr.readAll();
                        if (isStop || isEditData||flagExit||flagDel)
                        {
                            break;
                        }
                        qs=qpr.readAllStandardError();
                        qs=qpr.readAllStandardOutput();
                        while (qpr.bytesAvailable())
                        {
                            qs=qpr.readAllStandardError();
                            qs=qpr.readAllStandardOutput();
                        }
                        qs = "dump "+ui->tableValue->item(i,myTableWidget::enumColomn::addr)->text()+' '+buf+'\n';
                        qpr.write(qs.toLatin1());
                        qpr.waitForBytesWritten();
                        qpr.waitForReadyRead();
                        qDebug() << "qs = " <<qs <<'\n';
                        qs=qpr.readAll();
                        QString qsError = qpr.readAllStandardError();
                        QTime timer;
                        timer.start();
                        while (!qs.endsWith("> ") || qs.split('\n').count()<3
                               || !(qs.indexOf(": ") > 0))
                        {
                            qpr.waitForReadyRead();
                            qs+=qpr.readAll();
                            qsError += qpr.readAllStandardError();
                            qDebug() << qs;
                            qDebug() << qsError;
                            if (isStop || isEditData||flagExit||flagDel)
                                break;
                            if (timer.elapsed() > 5000)
                                break;
                        }
                        qDebug() << "qs = " << qs;
                        qs=qs.split(": ").last().split("  ").first();
                        DumpToStandarnHex(qs);
                        qDebug() << qs;
                        if (type == "i64" || type == "i64s" || type == "i64u")
                        {
                            unsigned long long ull=qs.toULongLong(0,16);
                            long long ll = ull;
                            if (type == "i64" || type == "i64s")
                                qs=QString::number(ll);
                            else
                                if (type == "i64u")
                                    qs=QString::number(ull);
                            qDebug() <<
                                        "qs = " << qs <<'\n' <<
                                        "ull = "<<ull << '\n'<<
                                        "ll = "<<ll<<'\n';
                            //                                            "qs.toULongLong(0,16) = " << qs.toULongLong(0,16);
                        }
                        else
                            if (type == "i32" || type == "i32s" || type == "i32u")
                            {
                                unsigned int ui=qs.toUInt(0,16);
                                int i = ui;
                                if (type == "i32" || type == "i32s")
                                    qs=QString::number(i);
                                else
                                    if (type == "i32u")
                                        qs=QString::number(ui);
                                qDebug() <<
                                            "qs = " << qs <<'\n' <<
                                            "ui = "<<ui << '\n'<<
                                            "i = "<<i<<'\n';
                                //                                                "qs.toULong(0,16) = " << qs.toULong(0,16);
                            }
                            else
                                if (type == "i16" || type == "i16s" || type == "i16u")
                                {
                                    unsigned short us=qs.toUShort(0,16);
                                    short s = us;
                                    if (type == "i16" || type == "i16s")
                                        qs=QString::number(s);
                                    else
                                        if (type == "i16u")
                                            qs=QString::number(us);
                                    qDebug() <<
                                                "qs = " << qs <<'\n' <<
                                                "us = "<<us << '\n'<<
                                                "s = "<<s<<'\n';
                                    //                                                    "qs.toUShort(0,16) = " << qs.toUShort(0,16);
                                }
                                else
                                    if (type == "i8" || type == "i8s" || type == "i8u")
                                    {
                                        unsigned char uc=qs.toUShort(0,16);
                                        signed char sc = uc;
                                        if (type == "i8" || type == "i8s")
                                            qs=QString::number(sc);
                                        else
                                            if (type == "i16u")
                                                qs=QString::number(uc);
                                        qDebug() <<
                                                    "qs = " << qs <<'\n' <<
                                                    "uc = "<<uc << '\n'<<
                                                    "sc = "<<sc<<'\n';
                                        //                                                        "qs.toULongLong(0,16) = " << qs.toULongLong(0,16);
                                    }
                                    /*else
                                        if (type == "f64" || type == "f64s" || type == "f64u")
                                        {
                                           float f = qs.to
                                           qs=QString::number(f);
                                        }
                                        else
                                            if (type == "f32" || type == "f32s" || type == "f32u")
                                            {
                                               double d = qs.toDouble(0,16);
                                               qs=QString::number(d);
                                            }*/
                        if (isStop || isEditData||flagExit||flagDel)
                            break;
                        qDebug() << "Обновление - 2";
                        bool block = ui->tableValue->blockSignals(true);
                        ui->tableValue->item(i,myTableWidget::enumColomn::value)->setText(qs);
                        ui->tableValue->blockSignals(block);
                        //                            usleep(500);
                        qDebug() << "Обновление - 3";
                    }
                }
            }
            if (isEditData)
            {
                while (1)
                {
                    qDebug() << "Изменение значения";
                    if (isStop||flagExit||flagDel)
                    {
                        break;
                    }
                    QString buf = ui->tableValue->item(myData.first().toUInt(),
                                                       myTableWidget::enumColomn::type)->text().split('-').first().toLower();
                    int k;
                    if ((k=buf.lastIndexOf('s'))>0 ||
                            (k=buf.lastIndexOf('u'))>0)
                        buf.truncate(k);
                    if (flagDel)
                    {
                        break;
                    }
                    qs = "write " +buf+' '+
                            ui->tableValue->item(myData.first().toUInt(),myTableWidget::enumColomn::addr)->text()+
                            ' '+ui->tableValue->item(myData.first().toUInt(),myTableWidget::enumColomn::value)->text() +'\n';
                    qDebug() << qs;
                    qpr.write(qs.toLatin1());
                    qDebug() << qs.toLatin1();
                    qpr.waitForBytesWritten();
                    myData.removeFirst();
                    if (myData.count() == 0)
                    {
                        isEditData=false;
                        break;
                    }
                }
            }
        }
        while (time.elapsed() <=2000)
        {
            if (flagExit)
            {
                break;
            }
            if (isEditData)
            {
                while (1)
                {
                    qDebug() << "Изменение значения";
                    QString buf = ui->tableValue->item(myData.first().toUInt(),
                                                       myTableWidget::enumColomn::type)->text().split('-').first().toLower();
                    int k;
                    if ((k=buf.lastIndexOf('s'))>0 ||
                            (k=buf.lastIndexOf('u'))>0)
                        buf.truncate(k);
                    if (flagDel)
                    {
                        break;
                    }
                    qs = "write " +buf+' '+
                            ui->tableValue->item(myData.first().toUInt(),myTableWidget::enumColomn::addr)->text()+
                            ' '+ui->tableValue->item(myData.first().toUInt(),myTableWidget::enumColomn::value)->text() +'\n';
                    qDebug() << qs;
                    qpr.write(qs.toLatin1());
                    qDebug() << qs.toLatin1();
                    qpr.waitForBytesWritten();
                    myData.removeFirst();
                    if (myData.count() == 0)
                    {
                        isEditData=false;
                        break;
                    }
                }
            }
        }
        }
}

void hold(Ui::MainWindow *ui, MainWindow *w)
{
    QString qs;
    QProcess qpr;
    qpr.start("scanmem");
    qpr.waitForStarted();
    QTime time;
    while (1)
    {
        time.start();
        if (isEditPid1)
        {
            qs= "pid "+QString::number(PID)+'\n';
            qpr.write(qs.toLatin1());
            qpr.waitForBytesWritten();
            isEditPid1=false;
            qDebug() << "Изменение PID";
            ui->pushButtonWeed->setEnabled(0);
            qDebug() << PID;
        }
        if (flagExit)
        {
            qpr.close();
            qpr.waitForFinished();
            return;
        }
        if (w->pr.isOpen())
        {
            for (int i=0; i<ui->tableValue->rowCount(); i++)
            {
                if (isStop || isEditData||flagExit||flagDel)
                    break;
                qDebug() << "-1";
                if (/*ui->tableValue->item(i,0) != 0 &&*/
                        ui->tableValue->item(i,myTableWidget::enumColomn::addr) !=0 &&
                        ui->tableValue->item(i,myTableWidget::enumColomn::type) !=0 &&
                        ui->tableValue->item(i,myTableWidget::enumColomn::value) !=0)
                {
                    qDebug() << "0";
                    qDebug() << "Заморозка (i = "<<i<<")";
                    if (/*dynamic_cast<QCheckBox *>(ui->tableValue->item(i,0))->isChecked()*/
                            dynamic_cast<QCheckBox *>(ui->tableValue->cellWidget(i,myTableWidget::enumColomn::hold))->isChecked())
                    {
                        qDebug() << "Замарозка";
                        if (isStop || isEditData||flagExit||flagDel)
                        {
                            break;
                        }
                        qDebug() << "1";
                        QString buf = ui->tableValue->item(i,myTableWidget::enumColomn::type)->text().split('-').first().toLower();
                        if (buf.lastIndexOf('s')>0)
                            buf.truncate(buf.lastIndexOf('s'));
                        if (isStop || isEditData||flagExit||flagDel)
                        {
                            break;
                        }
                        qDebug() << "2";
                        qs = "write " + buf+' '+
                                ui->tableValue->item(i,myTableWidget::enumColomn::addr)->text()+
                                ' '+ui->tableValue->item(i,myTableWidget::enumColomn::value)->text() +'\n';
                        qDebug() << "3";
                        qpr.write(qs.toLatin1());
                        qpr.waitForBytesWritten();
                        qpr.waitForReadyRead();
                    }
                }
            }
        }
        while (time.elapsed() <=1000)
        {
        }
    }
}

void underThread(Ui::MainWindow *ui)
{
    QProcess qpr;
    QString qs;
    QStringList qsl;
    bool pid =0;
    while(1)
    {
        if (flagExit)
        {
            qpr.close();
            qpr.waitForFinished();
            return;
        }
        qs = "ps aux";
        qpr.start(qs);
        qpr.waitForFinished();
        qs = qpr.readAllStandardOutput();
        qsl = qs.split("\n");
        if (qsl.count() <= 1)
            qs += qpr.readAllStandardOutput();
        if (qsl.count() > 3)
        {
            qsl.removeLast();
            //                qsl.removeLast();
            qsl.removeFirst();
        }
        //            qDebug() << qs;
        pid =0;
        for (int i =0; i < qsl.count(); i++)
        {
            qsl[i] = qsl[i].simplified();
            if (qsl[i].split (' ').count ()>=2)
                if (qsl[i].split(' ')[1].toLong() == PID)
                {
                    ui->pushButtonSearch->setEnabled(1);
                    ui->pushButtonAdd->setEnabled(1);
                    pid =1;
                    break;
                }
        }
        if (!pid)
        {
            ui->pushButtonSearch->setEnabled(0);
            ui->pushButtonAdd->setEnabled(0);
            ui->pushButtonWeed->setEnabled(0);
        }
        qpr.close();
        qpr.waitForBytesWritten();
    }
}
