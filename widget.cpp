#include "widget.h"
#include "ui_widget.h"
#include<stdio.h>
#include<string.h>
//#include<malloc.h>
#define ABS(x,y) (x)<(y)?((y)-(x)):((x)-(y))
#define isPixel(buff,i,j) buff[(i)*2+(j)/8]&((0x80)>>((j)%8))

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    length=0;
    startPosition=0;
    originalSize=size();
    foo=true;
    file=fopen("HZK16","rb");

    inputData=(char *)calloc(200,1);
    printf(getcwd(inputData,200));
    strcpy(inputData,"北京航空航天大学欢迎你");
    this->getPixBuff(inputData);
    draw();
    timer=new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(timerHandle()));
    timer->start(200);


}
void Widget::getPixBuff(char * data)
{
    int len=strlen(data);
    if(len%2)
    {
        QMessageBox::information(this,"has none-gb2312 code","has none-gb2312 code");
        return;
    }
    int i=0;
    for(;i<len;i+=2)
    {
        char temp=data[i+2];
        data[i+2]=0;
        int pos=this->getPosAtHZK(data+i);
        fseek(file,pos,SEEK_SET);
        fread(buff[i/2],1,32,file);
        data[i+2]=temp;
    }
}
int Widget::getPosAtHZK(char *data)
{
    int i,j;
    unsigned int pos=0;
    char temp[2]={0};
    for(i=0;i<2;i++)
    {
            for(j=0;j<8;j++)
            {
                    temp[i]+=(data[i]&(0x80>>j));
            }
            temp[i]-=160;
    }
    pos=((temp[0]-1)*94+temp[1]-1)<<5;
    return pos;
}
void Widget::timerHandle()
{
    int temp=strlen(inputData)*8;
    if(length>temp)//16*strlen(inputData))
    {

        startPosition=80;
    }
    if(startPosition)
    {
        startPosition--;
        length=0;
    }
    else
    {
        length++;
    }

    draw(length);
    update();
}
void Widget::draw(int dx)
{
    pixmap=QPixmap(this->size());
    pixmap.fill(this,0,0);
    QPainter painter(&pixmap);
    painter.initFrom(this);
    int width=this->size().width();
    int height=this->size().height();
    int avgSize=width/80;
    int margin=2,border=20;
    int j=margin,i=border;
    int countI=0,countJ=dx;
    bool drawPix=false;
    while(i+avgSize<height)
    {
        countJ=dx;
        j=margin;
        while(j+avgSize<width)
        {
            if(countJ>=startPosition)
            {
                int tempJ=countJ;
                if(startPosition)
                {
                    countJ=countJ-startPosition;
                }
                int temp=countJ>>4;

                if(countI<16&&temp<(strlen(inputData)/2)&&isPixel(buff[temp],countI,countJ&0xF))
                {
                   painter.fillRect(j,i,avgSize*2/3,avgSize*2/3,Qt::red);
                }
                else
                {
                   ;//painter.drawRect(j,i,avgSize*2/3,avgSize*2/3);
                }

                countJ=tempJ;

            }
            countJ++;
            j+=avgSize;
        }
        countI++;
        i+=avgSize;

    } 

}


void Widget::resizeEvent ( QResizeEvent * event )
{

    QSize nowSize=size();
    if(ABS(nowSize.width(),originalSize.width())>QApplication::startDragDistance()
        ||ABS(nowSize.height(),originalSize.height())>QApplication::startDragDistance())
    {
        originalSize=nowSize;
        draw(length);
        update();


    }


}
Widget::~Widget()
{
    delete ui;
    fclose(file);
    delete inputData;
}
void Widget::paintEvent ( QPaintEvent * event )
{

    QPainter painter(this);

    painter.drawPixmap(0,0,pixmap);

}
void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
