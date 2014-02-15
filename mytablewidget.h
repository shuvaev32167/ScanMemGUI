#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QTableWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QCheckBox>
#include <unistd.h>

extern bool flagDel;
extern bool isStop;

class myTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    myTableWidget ( QWidget * parent = 0 );

    enum enumColomn{hold =0, addr =1, type =2, value =3 };

signals:

public slots:

protected:
    virtual void keyPressEvent ( QKeyEvent * event );

    virtual void keyReleaseEvent(QKeyEvent *event);
private:
    QSet<int> keys;
};
#endif // MYTABLEWIDGET_H
