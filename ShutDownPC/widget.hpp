#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QTime>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void showTime();
    void on_close();
    void on_pushButton_save_clicked();

private:
    void saveTimeValues();
    void readTimesFromSettings();
    void updateTimeTextOnWidted(QTime &time);
    void closeEvent(QCloseEvent *event);
    void createMinimalizeToTry();
    bool shutDownCommandWasActivated = false;
    Ui::Widget *ui;
    QTimer *timer;
    QSystemTrayIcon *icon;

    QMenu *menu;
    QAction *hide_window;
    QAction *restore;
    QAction *quitAction;

    QTime showWindowTime ;
    QTime shutDownTime;

    bool flagAppStartedAfterOffTime = false;
};

#endif // WIDGET_HPP
