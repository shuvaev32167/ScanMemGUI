#include "addvalue.h"
#include "ui_addvalue.h"
#include "ui_mainwindow.h"

AddValue::AddValue(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddValue)
{
    ui->setupUi(this);

    ui->comboBox->addItem ("int8");
    ui->comboBox->addItem ("int16");
    ui->comboBox->addItem ("int32");
    ui->comboBox->addItem ("int64");
    ui->comboBox->addItem ("float32");
    ui->comboBox->addItem ("float64");

    type = "i8";
}

AddValue::~AddValue()
{
    delete ui;
}

void AddValue::setMainWindow(MainWindow *mw)
{
    this->mw=mw;
}

void AddValue::on_pushButtonClose_clicked()
{
   close();
//   delete this;
}

void AddValue::on_comboBox_activated(int index)
{
    switch (index) {
    case 0:
        type = "i8";
        break;
    case 1:
        type = "i16";
        break;
    case 2:
        type = "i32";
        break;
    case 3:
        type = "i64";
        break;
    case 4:
        type = "f32";
        break;
    case 5:
        type = "f64";
        break;
    default:
        qDebug() << tr("Ничё не выбранно");
        break;
    }
}

void AddValue::on_pushButtonAdd_clicked()
{
//    qDebug() << "1";
    mw->addValie(ui->comboBox->currentText(), ui->lineEdit->text());
//    qDebug() << "2";
    close();
}
