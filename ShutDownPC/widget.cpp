#include "widget.hpp"
#include "ui_widget.h"

#include <QTime>
#include <QProcess>
#include <QSettings>
#include <QByteArray>
#include <QDebug>
#include <QPixmap>
#include <QPainter>

static int run_hour = 0;
static int run_minute = 0;

QSettings settings("Reyfel", "Alarm");



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
    // setWindowFlags(Qt::WindowStaysOnTopHint);
    // setWindowFlags(Qt::FramelessWindowHint);

    createMinimalizeToTry();

    run_hour = settings.value("hour").toInt();
    run_minute = settings.value("minute").toInt();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    // QTimer::singleShot(0, this, SLOT(hide()));

    applicationPath = "/home/rafal/Pulpit/android-studio/bin/./studio.sh";
    ui->lineEdit->setText(applicationPath);
    ui->timeEdit->setTime(QTime(run_hour,run_minute));
}

Widget::~Widget()
{
    settings.setValue("hour", run_hour);
    settings.setValue("minute",run_minute);
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

bool killall(const QString &process){
    QProcess tasklist;
    QStringList args;
    args << process;
    tasklist.start("killall", args);
    tasklist.waitForFinished();
}

bool kill(const int pid){
    QProcess tasklist;
    QStringList args;
    args << QString::number(pid);
    tasklist.start("kill -s SIGTERM", args);
    tasklist.waitForFinished();
}

/* return pid list */
QList<QByteArray> isRunning(const QString &process) {
    QProcess tasklist;
    QStringList args;
    args << process;
    tasklist.start("pgrep", args);
    tasklist.waitForFinished();
    QList<QByteArray> output = tasklist.readAllStandardOutput().split('\n');
    output.removeLast();
    return output;
}


void Widget::showTime()
//! [1] //! [2]
{




    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    ui->label->setText(text);

    if((time.hour() >= run_hour) && (time.minute() >= run_minute) ){


        if(isActive == false){
            show();
            raise();
            activateWindow();

            QList<QByteArray> listaPidow= isRunning("qtcreator");
            for(int i = 0; i < listaPidow.size(); i++){
                qDebug() << listaPidow.at(i).toInt();
                kill(listaPidow.at(i).toInt());
            }

            runProcess(applicationPath);
            isActive = true;
        }

    }
}

void Widget::on_close()
{
    QApplication::quit();
}

void Widget::on_timeEdit_timeChanged(const QTime &time)
{
    run_hour = time.hour();
    run_minute = time.minute();

    settings.setValue("hour", run_hour);
    settings.setValue("minute",run_minute);
    qDebug() << "On time changed" ;
}

void Widget::on_timeEdit_editingFinished()
{
    qDebug() << "On editing finished";
}
