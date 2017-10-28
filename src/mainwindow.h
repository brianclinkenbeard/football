#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_go_to_stadium_capacity_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::database();
};

#endif // MAINWINDOW_H
