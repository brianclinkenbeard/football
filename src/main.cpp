#include "mainwindow.h"
#include "login.h"
#include "graph.h"
#include <QApplication>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //"res/NFL.db"
    db.setDatabaseName("res/NFL.db");

    // Will integrate with the login window later
    // just need to test my code.
    MainWindow window;
    window.show();

    //showing bryce how to merge

//    Graph<QString> graph;
//    graph.loadGraph(graph);
//    graph.printAdjList();
//    graph.DFS("Hard Rock Stadium");
//    graph.clearEdgeType();
//    graph.BFS("Lambeau Field");

    return a.exec();
}
