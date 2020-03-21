#include "widget.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    char L;
//    int n,aa,bb,cc,dd;
//    cin>>n;
//    for(int i=0;i<n;i++) {
//        cin>>L>>aa>>bb>>cc>>dd;
//        add_diagram(L,aa,bb,cc,dd);
//    }

    Widget w;
    w.show();

    return a.exec();
}
