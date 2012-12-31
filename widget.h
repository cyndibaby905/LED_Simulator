#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QtGui>
#include<stdio.h>
#include<stdlib.h>
namespace Ui {
    class Widget;
}

class Widget : public QWidget {
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    void draw(int dx=0);
    ~Widget();
    void getPixBuff(char *);
private slots:
    void timerHandle();

protected:
    void changeEvent(QEvent *e);
    void paintEvent ( QPaintEvent * event );
    void resizeEvent ( QResizeEvent * event );
    int getPosAtHZK(char *data);


private:
    Ui::Widget *ui;
    QPixmap pixmap;
    QPixmap pixmap1;
    QSize originalSize;
    FILE *file;
    char buff[32][32];
    char headBuff[6][32];
    char *inputData;
    char *headerData;
    QTimer *timer;
    int length;
    int startPosition;
    bool foo;

};

#endif // WIDGET_H
