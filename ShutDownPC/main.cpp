#include "widget.hpp"
#include <QApplication>
#include <string>

using namespace std;

void redirectCoutToFile(string fileName){
    freopen(fileName.c_str(),"w",stdout);
}

int main(int argc, char *argv[])
{
    redirectCoutToFile("/home/rafal/Pulpit/LOG_ShutDownPC.txt");

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
