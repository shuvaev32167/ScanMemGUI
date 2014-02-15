#include "pidvarionts.h"
#include "ui_pidvarionts.h"
#include "ui_mainwindow.h"

PidVarionts::PidVarionts(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PidVarionts)
{
    ui->setupUi(this);

    mainWindow = 0;

//    ui->tableProcess->setRowCount(0);

//    thr = new std::thread(std::mem_fn(&PidVarionts::refreshTable), this);
//    QtConcurrent::run(this, &PidVarionts::refreshProcess);
//    refreshProcess();
    /*auto f =std::async(std::mem_fn(&PidVarionts::refreshTable),this);
    f.get();*/
    /*connect(&qthr,SIGNAL(started()),this,SLOT(refreshTable()));
    this->moveToThread(&qthr);
    qthr.start();*/

    ui->tableProcess->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableProcess->verticalHeader()->setDefaultSectionSize(20);
    //    ui->tableAddr->setSelectionMode(QAbstractItemView::);
    ui->tableProcess->setSelectionBehavior( QAbstractItemView::SelectRows );

    this->flagExit = false;

    thr = new std::thread(std::mem_fn(&PidVarionts::refreshProcess), this);
    ui->tableProcess->setColumnCount (3);
//    refreshProcess();
    ui->tableProcess->setHorizontalHeaderItem (0, new QTableWidgetItem(tr("PID")));
    ui->tableProcess->setHorizontalHeaderItem (1, new QTableWidgetItem(tr("Пользователь")));
    ui->tableProcess->setHorizontalHeaderItem (2, new QTableWidgetItem(tr("Процесс")));

    ui->tableProcess->setColumnWidth (0,50);
    ui->tableProcess->setColumnWidth (1,100);
    ui->tableProcess->horizontalHeader()->setStretchLastSection(true);

    ui->pushButton->hide();
    ui->lineEdit->hide();

//    refreshProcess();
}

PidVarionts::~PidVarionts()
{
    delete ui;
}

void PidVarionts::setMainWindows(MainWindow *const w)
{
    mainWindow=w;
}

void PidVarionts::on_pushButton_clicked()
{
    PID = ui->lineEdit->text().toLong();
    if (mainWindow!=0)
    {
        QString qs = "pid "+QString::number(PID) +'\n';
        mainWindow->pr.write(qs.toLatin1());
        mainWindow->pr.waitForBytesWritten();
        mainWindow->pr.waitForReadyRead();
        qs = mainWindow->pr.readAllStandardError();
        if (qs.endsWith("Enter \"help\" for other commands.\n"))
            return;
        isEditPid0=true;
//        mainWindow->pr.write("option scan_data_type number");
//        mainWindow->pr.waitForBytesWritten();
    }
    close();
//    delete this;
}

void PidVarionts::refreshProcess()
{
    QProcess qpr;
//    if (qpr.isOpen())
//    {
//        qpr.close();
//        qpr.waitForFinished();
//    }
    qDebug()<<"start";
    QString qs = "";
    if (this->flagExit)
        return;
    qpr.start("ps aux");
//    qs = qpr.readAll();
//    qDebug() << "qs = "<<qs;
//    qDebug() << "Error\n" << qpr.readAllStandardError();
//    qpr.waitForStarted();
    qpr.waitForFinished(2000);
//    sleep(1);
    qs += qpr.readAll();
    while (qpr.size()!=0)
        qs+=qpr.readAll();
    qDebug() << "qs = "<<qs;
//    qDebug() << "qs = " << qs;
//    qs=qs.simplified();
//    qDebug() << "qs = " << qs;
    globalQsl = qs.split('\n');
//    qDebug() << "qsl = " << qsl;
    if (!globalQsl.empty())
        globalQsl.removeLast();
//    qDebug() << "globalQsl = " << globalQsl;
//    globalQsl.removeLast();
    if (!globalQsl.empty())
        globalQsl.removeFirst();
//    qDebug() << "qs = " << qs;
//    qDebug() << "1";
    for (int i =0; i <globalQsl.count(); i++)
        globalQsl[i]=globalQsl[i].simplified();
    qsl=globalQsl;
//    qDebug() << 2;
//    qDebug() << "qsl = " << qsl;
    refreshTable();
//    qDebug() << "qsl = " << qsl;
    qpr.close();
    qpr.waitForFinished();

}

void PidVarionts::refreshTable()
{
    qDebug() << "refresh table";
    ui->tableProcess->setRowCount(0);
//    qDebug() << "i";
    QTableWidgetItem *item;
    QStringList qslist;
    QString qs;
//    qDebug() << 3;
//    qDebug() << "qsl.count() = " << qsl.count();
    for (int i =0; i<qsl.count();i++)
    {
        qslist = qsl[i].split(' ');
        ui->tableProcess->insertRow(ui->tableProcess->rowCount ());
//        qDebug() << i;
        item = new QTableWidgetItem(qslist[1]);
        delete ui->tableProcess->item (ui->tableProcess->rowCount ()-1,0);
        ui->tableProcess->setItem(ui->tableProcess->rowCount ()-1,0, item);
//        qDebug() << i;
        item = new QTableWidgetItem(qslist.first());
        delete ui->tableProcess->item (ui->tableProcess->rowCount ()-1,1);
        ui->tableProcess->setItem(ui->tableProcess->rowCount ()-1,1, item);

        qs="";
        for(int j=10; j<qslist.count();j++)
        {
            /*qDebug() << "Мы тут";
            qDebug() <<"j = " << j;
            qDebug() << qsl[i].split(' ').count();*/
            qs+=qslist[j]+' ';
//            qDebug() << "qs = "<<qs;
        }
        item = new QTableWidgetItem(qs);
        delete ui->tableProcess->item (ui->tableProcess->rowCount ()-1,2);
        ui->tableProcess->setItem(ui->tableProcess->rowCount ()-1,2, item);
    }
//    qDebug() << "Мы вышли";

//    this->repaint();
//    repaint();
}

bool PidVarionts::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object);
    if (event->type() == QEvent::Close)
    {
        this->flagExit=true;
        hide();
//        sleep(5);
        thr->join();
    }
    return false;
}

void PidVarionts::on_lineEditProcName_textChanged(const QString &arg1)
{
    qsl.clear();
    QStringList buf, qslist;
    QString qs;
    for (int i = 0; i <globalQsl.count(); i++)
    {
        qslist = globalQsl[i].split(' ');
        qs="";
        for(int j=10; j<qslist.count();j++)
        {
            qs+=qslist[j]+' ';
        }
        if (qs.indexOf(arg1)>=0)
            buf <<globalQsl[i];
    }
    for (int i=0; i < buf.count(); i++)
        if (buf[i].split(' ').first().indexOf(ui->lineEditUser->text()) >=0)
            qsl << buf[i];
//    qsl = buf;
    thr = new std::thread(std::mem_fn(&PidVarionts::refreshTable),this);
//    qDebug() << qsl;
}

void PidVarionts::on_lineEditUser_textChanged(const QString &arg1)
{
    qsl.clear();
    QStringList buf, qslist;
    QString qs;
    for (int i = 0; i <globalQsl.count(); i++)
    {
        qslist = globalQsl[i].split(' ');
        if (qslist.first().indexOf(arg1)>=0)
            buf <<globalQsl[i];
    }
    for (int i=0; i < buf.count(); i++)
    {
        qslist = buf[i].split(' ');
        qs="";
        for(int j=10; j<qslist.count();j++)
        {
            qs+=qslist[j]+' ';
        }
        if (qs.indexOf(ui->lineEditProcName->text())>=0)
            qsl <<buf[i];
//        qDebug() << qs;
    }
//    qsl = buf;
    thr = new std::thread(std::mem_fn(&PidVarionts::refreshTable),this);
//    qDebug() << qsl;
}

void PidVarionts::on_tableProcess_doubleClicked(const QModelIndex &index)
{
    PID = ui->tableProcess->item(index.row(),0)->text().toLong();
    if (mainWindow!=0)
    {
//        QStringList qsl;
//        qsl << "-p" << QString::number(PID);
//        qsl << ">" << QApplication::applicationDirPath()+"/scanmemOut";
//        mainWindow->pr.setStandardOutputFile("scanmemOut");
//        mainWindow->pr.setStandardErrorFile("scanmemErr");
        //mainWindow->pr.setProcessChannelMode(QProcess::MergedChannels);
        /*if (mainWindow->pr.isOpen())
        {
            mainWindow->pr.close();
            mainWindow->pr.waitForFinished();
        }
//        mainWindow->pr.start("scanmem", qsl);
        mainWindow->pr.waitForStarted();*/
        QString qs = "pid "+QString::number(PID) +'\n';
        mainWindow->pr.write(qs.toLatin1());
        mainWindow->pr.waitForBytesWritten();
        mainWindow->pr.waitForReadyRead();
        qs = mainWindow->pr.readAllStandardError();
        if (qs.endsWith("Enter \"help\" for other commands.\n"))
            return;
        isEditPid0=true;
        isEditPid1=1;
        mainWindow->ui->pushButtonSearch->setEnabled(1);
        mainWindow->ui->pushButtonAdd->setEnabled(1);
//        mainWindow->pr.write("option scan_data_type number");
//        mainWindow->pr.waitForBytesWritten();
    }
    close();
//    delete this;
}
