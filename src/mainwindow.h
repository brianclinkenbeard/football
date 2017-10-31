#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "login.h"
#include <QMainWindow>
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDebug>

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

    void on_btn_stadium_capacity_back_clicked();

    void on_pushButton_teams_stadiums_clicked();

    void on_comboBox_selection_currentIndexChanged(int index);

    void on_comboBox_filters_currentIndexChanged(int index);

    void on_pushButton_team_stadium_back_clicked();

    void on_adminLoginButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::database();
    login Login;

    // helper functions
    enum teams { ALL, AFC, NFC };
    void populate_teams(teams selection);
    void populate_stadiums(bool open_roof);
    void populate_players();
    void populate_surfaces();
};

#endif // MAINWINDOW_H
