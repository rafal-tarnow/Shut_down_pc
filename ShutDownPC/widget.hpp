#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>

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
     void on_timeEdit_timeChanged(const QTime &time);

     void on_timeEdit_editingFinished();

private:
    void closeEvent(QCloseEvent *event);
    void createMinimalizeToTry();
    bool isActive = false;
    Ui::Widget *ui;
    QTimer *timer;
    QSystemTrayIcon *icon;

    QMenu *menu;
    QAction *hide_window;
    QAction *restore;
    QAction *quitAction;
    QString applicationPath;
};

#endif // WIDGET_HPP
