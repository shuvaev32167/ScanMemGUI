#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "ui_searchform.h"

QStringList myData;

bool isEditData;

bool isEditPid0,isEditPid1;

bool isStop;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->installEventFilter(this);
//    ui->tableAddr->installEventFilter (this);
//    ui->tableValue->installEventFilter (this);
    ui->tableAddr->setEditTriggers(QAbstractItemView::NoEditTriggers); //Запрет на редактирование
    ui->tableAddr->verticalHeader()->setDefaultSectionSize(16);
    //    ui->tableAddr->setSelectionMode(QAbstractItemView::);
    ui->tableAddr->setSelectionBehavior( QAbstractItemView::SelectRows );//выбор всей строки

    ui->tableAddr->setSelectionMode(QAbstractItemView::SingleSelection);//запред множественного выбора
    ui->tableValue->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableValue->verticalHeader()->setDefaultSectionSize(18);
    //    ui->tableValue->setItemDelegateForColumn(1, new NonEditTableColumnDelegate());

    ui->pushButtonWeed->setEnabled(0);

    ui->tableAddr->setColumnCount (3);
    ui->tableAddr->setHorizontalHeaderItem (0, new QTableWidgetItem(tr("Адресс")));
    ui->tableAddr->setHorizontalHeaderItem (1, new QTableWidgetItem(tr("Тип")));
    ui->tableAddr->setHorizontalHeaderItem (2, new QTableWidgetItem(tr("Значение")));

    ui->tableValue->setColumnCount (4);
    ui->tableValue->setHorizontalHeaderItem (myTableWidget::enumColomn::hold, new QTableWidgetItem(tr("Заморозка")));
    ui->tableValue->setHorizontalHeaderItem (myTableWidget::enumColomn::addr, new QTableWidgetItem(tr("Адресс")));
    ui->tableValue->setHorizontalHeaderItem (myTableWidget::enumColomn::type, new QTableWidgetItem(tr("Тип")));
    ui->tableValue->setHorizontalHeaderItem(myTableWidget::enumColomn::value, new QTableWidgetItem(tr("Значение")));

    ui->tableAddr->setColumnWidth (0,ui->tableAddr->width () * 0.34);
    ui->tableAddr->setColumnWidth (1,ui->tableAddr->width () * 0.23);
    ui->tableAddr->setColumnWidth (2,ui->tableAddr->width () * 0.34);

    ui->tableValue->setColumnWidth (myTableWidget::enumColomn::hold,ui->tableValue->width () * 0.15);
    ui->tableValue->setColumnWidth (myTableWidget::enumColomn::addr,ui->tableValue->width () * 0.3);
    ui->tableValue->setColumnWidth (myTableWidget::enumColomn::type,ui->tableValue->width () * 0.2);
    ui->tableValue->setColumnWidth (myTableWidget::enumColomn::value,ui->tableValue->width () * 0.3);

    sf = new SearchForm();
    sf->setMainWindow(this);

    pr.start("scanmem");
    pr.waitForStarted();

    connect(ui->tableValue,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_tableAddr_customContextMenuRequested(QPoint)));
//    connect(ui->tableAddr,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_tableAddr_customContextMenuRequested(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonPID_clicked()
{
    PidVarionts *pid = new PidVarionts();
//    qthr = new QThread();
//    connect(qthr, SIGNAL(started()),pid,SLOT(refreshTable()));
//    pid->moveToThread(qthr);
//    qthr->start();
    pid->setMainWindows(this);
    pid->show();
}

void MainWindow::on_pushButtonSearch_clicked()
{
    //    if (qf.isOpen())
    //        qf.close();
    //    qf.open(QIODevice::ReadOnly);
    //    qstream.setDevice(&qf);
    //    pr.waitForReadyRead();
//    sf->ui->lineEdit->setText("");
    sf->flagSearch=true;
    sf->setWindowTitle(tr("Поиск"));
    sf->show();

    //    if (qsl.first().toULongLong() <= 1000)
//    listValue();
}

void MainWindow::on_pushButtonWeed_clicked()
{
    sf->flagSearch=false;
    sf->setWindowTitle(tr("Отсеять"));
    sf->show();
}

void MainWindow::listValue()
{
    ui->tableAddr->setRowCount (0);
    if (ui->label->text().split(' ').last().toUInt() > 15000)
        return;
    pr.write("list\n");
    pr.waitForBytesWritten();
    pr.waitForReadyRead();
    QString qs = pr.readAll();
    //    QString qs=qstream.readAll();
    //    qDebug() << qs;
    while(!qs.endsWith("> "))
    {
        pr.waitForReadyRead();
        qs+=pr.readAll();
        //        qs+=qstream.readAll();
        //        qDebug() << qs;
    }
    qDebug() <<qs;
    QStringList qsl = qs.split('\n');
    qsl.removeFirst();
    qsl.removeLast();
    //    qDebug() <<qsl;

    //    ui->tableAddr->clear ();
    QTableWidgetItem *item;
    for (int i=0; i<qsl.count(); i++)
    {
//        qDebug() << "i = " << i;
        QStringList q=qsl[i].split(", ");
        q.first()=q.first().split("0x").last();
//        qDebug() << "q = "<<q;
        QStringList qslist = q.last().split(' ');
//        qDebug() << "qslist = " << qslist;
        if (qslist.count() > 1)
        {
            qslist.removeLast();
            qslist.first().remove(0,1);
            if (qslist.count() > 1)
                q.last() = qslist.first() + '-' +qslist.last();
            else
                q.last()=qslist.first();
        }
        else
            q << qslist.first();

        //        qDebug() << q;
//        qDebug() << "i = " << i;
        ui->tableAddr->setRowCount (ui->tableAddr->rowCount ()+1);

//        qDebug() << "q = "<<q;

        item = new QTableWidgetItem(q[0]);
        delete ui->tableAddr->item (ui->tableAddr->rowCount ()-1,0);
        ui->tableAddr->setItem(ui->tableAddr->rowCount ()-1,0, item);
        item = new QTableWidgetItem(q[2]);
        delete ui->tableAddr->item (ui->tableAddr->rowCount ()-1,1);
        ui->tableAddr->setItem(ui->tableAddr->rowCount ()-1,1, item);

        item = new QTableWidgetItem(q[1]);
        delete ui->tableAddr->item (ui->tableAddr->rowCount ()-1,2);
        ui->tableAddr->setItem(ui->tableAddr->rowCount ()-1,2, item);
    }
}

void MainWindow::addValie(const QString &type, const QString &value)
{
    isStop = 1;
//    sleep(1);
    ui->tableValue->insertRow(ui->tableValue->rowCount());

    QTableWidgetItem *item = new QTableWidgetItem;
    bool oldBlock = ui->tableValue->blockSignals(true);
    /*item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    delete ui->tableValue->item (ui->tableValue->rowCount ()-1,0);
    ui->tableValue->setItem(ui->tableValue->rowCount ()-1,0, item);*/
    ui->tableValue->setCellWidget(ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::hold, new QCheckBox());

    item = new QTableWidgetItem(value);
    item->setFlags(Qt::ItemIsEditable^2);
    item->setTextColor(Qt::black);
    delete ui->tableValue->item (ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::addr);
    ui->tableValue->setItem(ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::addr, item);

    QString qs = "";
    if (type == "int64")
        qs = "I64";
    else
        if (type == "int32")
            qs = "I32";
        else
            if (type == "int16")
                qs = "I16";
            else
                if (type == "int8")
                    qs = "I8";

    item = new QTableWidgetItem(qs);
    item->setFlags(Qt::ItemIsEditable^2);
    item->setTextColor(Qt::black);
    delete ui->tableValue->item (ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::type);
    ui->tableValue->setItem(ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::type, item);

    item = new QTableWidgetItem("");
    delete ui->tableValue->item (ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::value);
    ui->tableValue->setItem(ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::value, item);

    ui->tableValue->blockSignals(oldBlock);
    isStop=0;
}

void MainWindow::on_tableAddr_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    QTableWidgetItem *item;
    ui->tableValue->setRowCount (ui->tableValue->rowCount ()+1);

//    item = new QTableWidgetItem;
    bool oldBlock = ui->tableValue->blockSignals(true);
    /*item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    delete ui->tableValue->item (ui->tableValue->rowCount ()-1,0);
    ui->tableValue->setItem(ui->tableValue->rowCount ()-1,0, item);*/
    QWidget *p = new QCheckBox;
    ui->tableValue->setCellWidget(ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::hold, p);

    item = new QTableWidgetItem(ui->tableAddr->item(row,0)->text());
    item->setFlags(Qt::ItemIsEditable^2);
    item->setTextColor(Qt::black);
    delete ui->tableValue->item (ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::addr);
    ui->tableValue->setItem(ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::addr, item);

    item = new QTableWidgetItem(ui->tableAddr->item(row,1)->text());
    item->setFlags(Qt::ItemIsEditable^2);
    item->setTextColor(Qt::black);
    delete ui->tableValue->item (ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::type);
    ui->tableValue->setItem(ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::type, item);

    item = new QTableWidgetItem(ui->tableAddr->item(row,2)->text());
    delete ui->tableValue->item (ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::value);
    ui->tableValue->setItem(ui->tableValue->rowCount ()-1,myTableWidget::enumColomn::value, item);

    ui->tableValue->blockSignals(oldBlock);
}

bool MainWindow::eventFilter(QObject *pObject, QEvent *pEvent)
{
    Q_UNUSED(pObject);
    if(pEvent->type() == QEvent::Close)
    {
        flagExit = 1;
        hide();
        qDebug() << "close";
        if (pr.isOpen())
        {
//            pr.write("exit\n");
//            pr.waitForBytesWritten();
            pr.close();
            pr.waitForFinished();
        }
        QMainWindow::close();
//        delete this;
//        return true;
    }
    if(pEvent->type() == QEvent::Resize)
    {
        //        qDebug() << "table resized";

        ui->tableAddr->setColumnWidth (0,ui->tableAddr->width () * 0.34);
        ui->tableAddr->setColumnWidth (1,ui->tableAddr->width () * 0.23);
        ui->tableAddr->setColumnWidth (2,ui->tableAddr->width () * 0.34);

        ui->tableValue->setColumnWidth (myTableWidget::enumColomn::hold,25);
        ui->tableValue->setColumnWidth (myTableWidget::enumColomn::addr,ui->tableValue->width () * 0.3);
        ui->tableValue->setColumnWidth (myTableWidget::enumColomn::type,ui->tableValue->width () * 0.2);
        ui->tableValue->setColumnWidth (myTableWidget::enumColomn::value,ui->tableValue->width () * 0.3);
    }
    /*if(pEvent->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(pEvent);
//        QKeyEvent *keyEvent = QKeyEvent *(event);
//        qDebug() <<keyEvent->key();
        if (keyEvent->key() == Qt::Key_Backspace)
            if (ui->tableValue->keyPressEvent();)
                qDebug() << "1";
    }*/
    return false; // даем таблице отработать изменение размера самостоятельно
}

void MainWindow::on_tableValue_cellChanged(int row, int column)
{
    if (column == 3)
    {
        qDebug() << "Изменение";
        myData << QString::number(row)/* << ui->tableValue->item(row, column)->text()*/;
        isStop=true;
        isEditData = true;
        usleep(500);
        isStop=false;
    }
}

void MainWindow::on_pushButtonAdd_clicked()
{
    AddValue *av = new AddValue();
    av->show();
    av->setMainWindow(this);
}

void MainWindow::autoReplacementAddr()
{
    unsigned long long addrAddr=0, addrValue=0;
    long long difference=0;
    addrAddr = ui->tableAddr->item(ui->tableAddr->currentRow(),0)->text().toULongLong(0,16);
    addrValue = ui->tableValue->item(ui->tableValue->currentRow(),myTableWidget::enumColomn::addr)->text().toULongLong(0,16);
    difference = addrAddr - addrValue;
    qDebug() << "difference = " << difference;
    bool oldBlog = ui->tableValue->blockSignals(1);
    for (int i =0; i < ui->tableValue->rowCount(); i++)
    {
        addrValue = ui->tableValue->item(i,myTableWidget::enumColomn::addr)->text().toULongLong(0,16);
        qDebug() << "addrValue = " << addrValue;
        addrValue += difference;
        qDebug() << "addrValue = " << addrValue;
        ui->tableValue->item(i,myTableWidget::enumColomn::addr)->setText(QString::number(addrValue,16));
    }
    ui->tableValue->blockSignals(oldBlog);
//    qDebug() << "ui->tableAddr->currentRow() = " << ui->tableAddr->currentRow();
//    qDebug() << "ui->tableValue->currentRow() = " << ui->tableValue->currentRow();
}

void MainWindow::replacementAddr()
{
    /*unsigned long long addrAddr=0, addrValue=0;
    long long difference=0;
    addrAddr = ui->tableAddr->item(ui->tableAddr->currentRow(),0)->text().toULongLong(0,16);
    addrValue = ui->tableValue->item(ui->tableValue->currentRow(),myTableWidget::enumColomn::addr)->text().toULongLong(0,16);
    difference = addrAddr - addrValue;
    qDebug() << "difference = " << difference;*/
    bool oldBlog = ui->tableValue->blockSignals(1);
    ui->tableValue->item(ui->tableValue->currentRow(),myTableWidget::enumColomn::addr)->
            setText(ui->tableAddr->item(ui->tableAddr->currentRow(),0)->text());
    /*for (int i =0; i < ui->tableValue->rowCount(); i++)
    {
        addrValue = ui->tableValue->item(i,myTableWidget::enumColomn::addr)->text().toULongLong(0,16);
        qDebug() << "addrValue = " << addrValue;
        addrValue += difference;
        qDebug() << "addrValue = " << addrValue;
        ui->tableValue->item(i,myTableWidget::enumColomn::addr)->setText(QString::number(addrValue,16));
    }*/
    ui->tableValue->blockSignals(oldBlog);
}

void MainWindow::on_tableAddr_customContextMenuRequested(const QPoint &pos)
{
    if (ui->tableAddr->currentRow () != -1 && ui->tableValue->currentRow() != -1)
    {
        // получаем глобальные координаты
        QPoint globalPos;
        // если запрос от QAbstractScrollArea
        if (sender()->inherits("QAbstractScrollArea"))
            globalPos = ((QAbstractScrollArea*)sender())->viewport()->mapToGlobal(pos);
        // если от других виджетов
        else
            globalPos = ((QWidget*)sender())->mapToGlobal(pos);

        // Создаем меню
        QMenu menu;
        // Создаем пункт меню
        QAction *action1 = new QAction(tr("Автоизменение адрессов"), this);
        // соединяемся со слотом выбора этого пункта меню
        connect(action1, SIGNAL(triggered()), this, SLOT(autoReplacementAddr()));
        // добавляем пункт в меню
        menu.addAction(action1);
        action1 = new QAction(tr("Изменить адресс"), this);
        // соединяемся со слотом выбора этого пункта меню
        connect(action1, SIGNAL(triggered()), this, SLOT(replacementAddr()));
        // добавляем пункт в меню
        menu.addAction(action1);
        menu.exec (globalPos);
    }
}
