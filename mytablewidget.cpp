#include "mytablewidget.h"

myTableWidget::myTableWidget ( QWidget * parent)
  : QTableWidget(parent)
{
    isStop = 1;
}

void myTableWidget::keyPressEvent(QKeyEvent *event)
{
    keys.insert(event->key());
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down
            || event->key() == Qt::Key_Control)
    {
        if (keys.contains(Qt::Key_Control) && (keys.contains(Qt::Key_Up) || keys.contains(Qt::Key_Down)) &&
                                               (keys.count() == 2))
        {
            QTableWidgetItem *item1,*item2;
            if (this->selectedItems().count()>0)
            {
//                int row = this->currentRow();
                qDebug() << this->currentRow();
                isStop = 1;
                bool oldBlock = this->blockSignals(true);
                if (keys.contains(Qt::Key_Up))
                {
                    if (this->currentRow() != 0)
                    {
                        bool isCheked=dynamic_cast<QCheckBox *>(this->cellWidget(this->currentRow(),enumColomn::hold))
                                ->isChecked();
                        dynamic_cast<QCheckBox *>(this->cellWidget(this->currentRow(),enumColomn::hold))
                                ->setChecked(dynamic_cast<QCheckBox *>(this->cellWidget(this->currentRow()-1,enumColomn::hold))
                                             ->isChecked());
                        dynamic_cast<QCheckBox *>(this->cellWidget(this->currentRow()-1,enumColomn::hold))->setChecked(isCheked);
                        for (int i=1; i < this->columnCount(); i++)
                        {
//                            qDebug() << "i = " << i;
                            item1 = new QTableWidgetItem(this->item(this->currentRow(),i)->text());
                            item2 = new QTableWidgetItem(this->item(this->currentRow()-1,i)->text());
                            if (i!=enumColomn::value)
                            {
                                item1->setFlags(Qt::ItemIsEditable^2);
                                item1->setTextColor(Qt::black);

                                item2->setFlags(Qt::ItemIsEditable^2);
                                item2->setTextColor(Qt::black);
                            }

//                            qDebug() << item1->text();
//                            qDebug() << item2->text();

                            delete this->item (this->currentRow()-1,i);
                            this->setItem(this->currentRow()-1,i, item1);

                            delete this->item (this->currentRow(),i);
                            this->setItem(this->currentRow(),i, item2);

//                            qDebug() << item1->text();
//                            qDebug() << item2->text();

//                            qDebug() << "i = " << i;

                        }

                    }
                }
                else
                    if (keys.contains(Qt::Key_Down))
                    {
                        if (this->currentRow() != this->rowCount()-1)
                        {
                            bool isCheked=dynamic_cast<QCheckBox *>(this->cellWidget(this->currentRow(),enumColomn::hold))
                                    ->isChecked();
                            dynamic_cast<QCheckBox *>(this->cellWidget(this->currentRow(),enumColomn::hold))
                                    ->setChecked(dynamic_cast<QCheckBox *>(this->cellWidget(this->currentRow()+1,enumColomn::hold))
                                                 ->isChecked());
                            dynamic_cast<QCheckBox *>(this->cellWidget(this->currentRow()+1,enumColomn::hold))->setChecked(isCheked);
                            for (int i=1; i < this->columnCount(); i++)
                            {
                                item1 = new QTableWidgetItem(this->item(this->currentRow(),i)->text());
                                item2 = new QTableWidgetItem(this->item(this->currentRow()+1,i)->text());
                                if (i!=enumColomn::value)
                                {
                                    item1->setFlags(Qt::ItemIsEditable^2);
                                    item1->setTextColor(Qt::black);

                                    item2->setFlags(Qt::ItemIsEditable^2);
                                    item2->setTextColor(Qt::black);
                                }

    //                            qDebug() << item1->text();s
    //                            qDebug() << item2->text();

                                delete this->item (this->currentRow()+1,i);
                                this->setItem(this->currentRow()+1,i, item1);

                                delete this->item (this->currentRow(),i);
                                this->setItem(this->currentRow(),i, item2);
                            }
                        }
                    }
//                isStop = 0;
                qDebug() << "Выход";
                this->blockSignals(oldBlock);
//                sleep(10);
                QTableWidget::keyPressEvent(event);
            }
        }
        else
            QTableWidget::keyPressEvent(event);
    }
//    qDebug() << event->key();
    else
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Shift)
      {
        if (keys.contains(Qt::Key_Shift) && keys.contains(Qt::Key_Delete) &&
                                                       (keys.count() == 2))
        {
            this->setRowCount(0);
        }
        else
            if (event->key() == Qt::Key_Delete)
            {
                if (this->selectedItems().count()>0)
                {
                    flagDel = true;
                    usleep(500);
                    this->removeRow(this->currentRow());
                    this->selectRow(this->currentRow());
                    flagDel = 0;
                }
            }
            else
            {
                QTableWidget::keyPressEvent(event);
            }
      }
    else
    {
        QTableWidget::keyPressEvent(event);
    }
}

void myTableWidget::keyReleaseEvent(QKeyEvent *event)
{
    keys.remove(event->key());
}

