#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //"res/NFL.db"
    db.setDatabaseName("res/NFL.db");

    login Login;
    Login.show();

    return a.exec();
}
