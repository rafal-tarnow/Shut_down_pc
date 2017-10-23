#include "widget.hpp"
#include "ui_widget.h"

#include <QTime>
#include <QProcess>
#include <QSettings>
#include <QByteArray>
#include <QDebug>
#include <QPixmap>
#include <QPainter>


QSettings settings("Reyfel", "ShutDownPC");



void Widget::createMinimalizeToTry(void)
{
    icon= new QSystemTrayIcon(this);
    QPixmap pixmap(25,25);
    pixmap.fill(Qt::red);
    QPainter painter(&pixmap);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10));
    painter.drawText(pixmap.rect(),Qt::AlignCenter,"ALR");
    icon->setIcon(pixmap);
    icon->setToolTip("Alarm, kliknij aby rozwinąć");
    icon->setVisible(true);
    icon->show();

    menu = new QMenu(this);


    hide_window = new QAction("Minimalizuj",this);
    connect( hide_window,SIGNAL(triggered()),this,SLOT(hide()));

    quitAction = new QAction("Zamknij",this);
    connect(quitAction,SIGNAL(triggered()),this,SLOT(on_close()));

    restore = new QAction("Przywróć", this);
    connect (restore, SIGNAL(triggered()), this, SLOT(showNormal()));


    menu->addAction(hide_window);
    menu->addAction(restore);
    menu->addAction(quitAction);

    icon->setContextMenu(menu);

}

void Widget::updateTimeTextOnWidted(QTime &time){
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    ui->label->setText(text);
}

void Widget::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowStaysOnTopHint);
    //setWindowFlags(Qt::FramelessWindowHint);

    createMinimalizeToTry();

    readTimesFromSettings();


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    // QTimer::singleShot(0, this, SLOT(hide()));

    ui->timeEdit->setTime(shutDownTime);

    QTime currentTime = QTime::currentTime();

    updateTimeTextOnWidted(currentTime);
    if(currentTime >= shutDownTime){
        flagAppStartedAfterOffTime = true;
    }
     qDebug() << "flagAppStartedAfterOffTime = " << flagAppStartedAfterOffTime;
}

Widget::~Widget()
{
    saveTimeValues();

    timer->stop();
    delete timer;

    menu->removeAction(quitAction);
    menu->removeAction(restore);
    menu->removeAction(hide_window);


    delete hide_window;
    delete restore;
    delete quitAction;
    delete menu;

    delete icon;
    delete ui;
}

void runProcess(QString processName){
    QProcess::startDetached(processName);
}


void Widget::showTime()
//! [1] //! [2]
{
    QTime time = QTime::currentTime();

    updateTimeTextOnWidted(time);

    if(!flagAppStartedAfterOffTime)
    {
        return;
    }else{

        if((time.hour() >= showWindowTime.hour()) && ((time.minute()) >= showWindowTime.minute())){
            show();
            raise();
            activateWindow();
        }
        if((time.hour() >= shutDownTime.hour()) && (time.minute() >= shutDownTime.minute()) ){
            if(shutDownCommandWasActivated == false){
                qDebug() << "Start Command";
                runProcess("shutdown -P now");
                shutDownCommandWasActivated = true;
            }

        }
    }
}

void Widget::on_close()
{
    QApplication::quit();
}

void Widget::on_timeEdit_timeChanged(const QTime &time)
{

    qDebug() << "Off time = " << time.toString();
    shutDownTime = time;
    showWindowTime = shutDownTime.addSecs(-15*60);
    qDebug() << "Show time = " << showWindowTime.toString();

    saveTimeValues();

    qDebug() << "On time changed" ;
}

void Widget::saveTimeValues(){
    settings.setValue("shut_down_hour", shutDownTime.hour());
    settings.setValue("shut_down_minute",shutDownTime.minute());
    settings.setValue("show_window_hour", showWindowTime.hour());
    settings.setValue("show_window_minute", showWindowTime.minute());
}



void Widget::readTimesFromSettings(){
    shutDownTime.setHMS(settings.value("shut_down_hour").toInt(), settings.value("shut_down_minute").toInt(), 0);
    showWindowTime.setHMS(settings.value("show_window_hour").toInt(), settings.value("show_window_minute").toInt(), 0);
}



void Widget::on_timeEdit_editingFinished()
{
    qDebug() << "On editing finished";
}
