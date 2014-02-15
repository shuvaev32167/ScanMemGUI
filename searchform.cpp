#include "searchform.h"
#include "ui_searchform.h"
#include "ui_mainwindow.h"

SearchForm::SearchForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SearchForm)
{
    ui->setupUi(this);

    mw =0;

    ui->comboBox->addItem ("number");
    ui->comboBox->addItem ("int");
    ui->comboBox->addItem ("int8");
    ui->comboBox->addItem ("int16");
    ui->comboBox->addItem ("int32");
    ui->comboBox->addItem ("int64");
    ui->comboBox->addItem ("float");
    ui->comboBox->addItem ("float32");
    ui->comboBox->addItem ("float64");
    ui->comboBox->addItem ("string");

    ui->comboBox->setCurrentIndex(1);

    ui->lineEdit->setFocus();
}

SearchForm::~SearchForm()
{
    delete ui;
}

void SearchForm::setMainWindow(MainWindow *mw)
{
    this->mw = mw;
}

void SearchForm::on_pushButtonClose_clicked()
{
    this->hide();
    ui->lineEdit->setFocus();
}

void SearchForm::on_comboBox_activated(int index)
{
    switch (index) {
    case 0:
        mw->pr.write("option scan_data_type number\n");
        mw->pr.waitForBytesWritten();
        qDebug() << tr("Выбранны все цифры");
        break;
    case 1:
        mw->pr.write("option scan_data_type int\n");
        mw->pr.waitForBytesWritten();
        ui->lineEdit->setFocus();
        break;
    case 2:
        mw->pr.write("option scan_data_type int8\n");
        mw->pr.waitForBytesWritten();
        ui->lineEdit->setFocus();
        break;
    case 3:
        mw->pr.write("option scan_data_type int16\n");
        mw->pr.waitForBytesWritten();
        ui->lineEdit->setFocus();
        break;
    case 4:
        mw->pr.write("option scan_data_type int32\n");
        mw->pr.waitForBytesWritten();
        ui->lineEdit->setFocus();
        break;
    case 5:
        mw->pr.write("option scan_data_type int64\n");
        mw->pr.waitForBytesWritten();
        ui->lineEdit->setFocus();
        break;
    case 6:
        mw->pr.write("option scan_data_type float\n");
        mw->pr.waitForBytesWritten();
        ui->lineEdit->setFocus();
        break;
    case 7:
        mw->pr.write("option scan_data_type float32\n");
        mw->pr.waitForBytesWritten();
        ui->lineEdit->setFocus();
        break;
    case 8:
        mw->pr.write("option scan_data_type float64\n");
        mw->pr.waitForBytesWritten();
        ui->lineEdit->setFocus();
        break;
    case 9:
        mw->pr.write("option scan_data_type string\n");
        mw->pr.waitForBytesWritten();
        ui->lineEdit->setFocus();
        break;
    default:
        qDebug() << tr("Ничё не выбранно");
        break;
    }
}

void SearchForm::on_pushButtonSearch_clicked()
{
    isStop = true;
    ui->lineEdit->setEnabled(0);
    ui->comboBox->setEnabled(0);
    ui->pushButtonSearch->setEnabled(0);
    ui->pushButtonClose->setEnabled(0);
    ui->horizontalSlider->setEnabled(0);
//    mw->isCorrectPid=true;
//    sleep(1);
    qDebug() << "Поехали!";
    QString qs = mw->pr.readAll();
    if (flagSearch)
    {
        mw->pr.write("reset\n");
        mw->pr.waitForBytesWritten();
        mw->pr.waitForReadyRead();
    }
    QString qsError = mw->pr.readAllStandardError();
    mw->pr.write(ui->lineEdit->text().toLatin1()+"\n");
    mw->pr.waitForBytesWritten();
//    mw->pr.waitForReadyRead();
    qs = mw->pr.readAll();
    qsError = mw->pr.readAllStandardError();
    QStringList qslError;
//        qDebug() << qsError;
    //    QString qs = qstream.readAll();
    bool flag;
    if (flagSearch)
        flag = !(qs.endsWith(QString("> ")) && qs.split('\n').count() >= 3);
    else
        flag = !qs.endsWith(QString("> "));
    while (flag)
    {
        qsError += mw->pr.readAllStandardError();
        if (flagSearch)
        {
            qslError = qsError.split('\n');
            if (qslError.count() > 0)
            {
                qslError = qslError.last().split(' ');
                qslError = qslError.first().split('/');
                if (qslError.last() != "")
                {
                    ui->progressBar->setValue(qslError.first().toDouble() /
                                              qslError.last().toDouble() * 100);
                }
            }
        }
        else
        {
            qslError = qsError.split('i');
            qslError = qslError.first().split('.');
            ui->progressBar->setValue((qslError.count() -1) * 10);
        }
//        qDebug() << qslError.count();
        QApplication::processEvents(0);
        repaint();
//        mw->pr.waitForReadyRead();
        qs+=mw->pr.readAll();
//        qDebug() << qs;
        //        qs+=qstream.readAll();
        //        qDebug() << pr.readAllStandardError();
        if (flagSearch)
            flag = !(qs.endsWith(QString("> ")) && qs.split('\n').count() >= 3);
        else
            flag = !qs.endsWith(QString("> "));
    }
    ui->progressBar->setValue(100);
    qDebug() << qs;
    QStringList qsl = qs.split('\n');
    qsl=qsl.last().split('>');
    mw->ui->label->setText(QString("Найдено: ") + qsl.first());

    mw->ui->pushButtonWeed->setEnabled(1);

    mw->listValue();

    isStop = false;
    this->hide();

    ui->lineEdit->setFocus();
}

void SearchForm::on_horizontalSlider_valueChanged(int value)
{
    QString qs;
//    qDebug() << value;
    switch (value)
    {
    case 1:
        qs="1";
        ui->labelSearch->setText(tr("Быстро"));
        ui->labelSearch->setAlignment(Qt::AlignLeft);
        qDebug() << "value = " << value;
//        qDebug() << "ui->horizontalSlider->tickPosition() = " << ui->horizontalSlider->tickPosition();
        break;
    case 2:
        qs="2";
//        qs="";
        ui->labelSearch->setText(tr("Нормально"));
        ui->labelSearch->setAlignment(Qt::AlignHCenter);
        qDebug() << "value = " << value;
//        qDebug() << "ui->horizontalSlider->tickPosition() = " << ui->horizontalSlider->tickPosition();
        break;
    case 3:
        qs="3";
//        qs="";
        ui->labelSearch->setText(tr("Медленно"));
        ui->labelSearch->setAlignment(Qt::AlignRight);
        qDebug() << "value = " << value;
//        qDebug() << "ui->horizontalSlider->tickPosition() = " << ui->horizontalSlider->tickPosition();
        break;
    default:
        break;
    }
    mw->pr.write("option region_scan_level "+qs.toLatin1()+'\n');
    mw->pr.waitForBytesWritten();
    mw->pr.waitForReadyRead();
    qDebug() << "option region_scan_level "+qs.toLatin1();
}

void SearchForm::show()
{
    ui->progressBar->setValue(0);
    ui->lineEdit->setEnabled(1);
    ui->comboBox->setEnabled(1);
    ui->pushButtonSearch->setEnabled(1);
    ui->pushButtonClose->setEnabled(1);
    ui->horizontalSlider->setEnabled(1);
    ui->lineEdit->setFocus();
    if (flagSearch)
        ui->lineEdit->setText("");
    QMainWindow::show();
}
