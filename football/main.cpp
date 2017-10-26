#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login Login;
    Login.show();

    QSqlDatabase db = QSqlDatabase::addDatabase("SQLITE");
    db.setDatabaseName("");

    return a.exec();
}
