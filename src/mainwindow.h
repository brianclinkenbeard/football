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
#include <QSound>
#include "graph.h"
#include "cart.h"
#include <QTableWidgetItem>

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

    void on_comboBox_filters_activated(const QString &arg1);

    void on_comboBox_filters_selectTeamName(const QString &arg1,int index);

    void on_pushButton_team_information_clicked();

    void on_pushButton_single_team_back_clicked();

    void on_comboBox_single_team_activated(const QString &arg1);

    void reloadComboBoxes();

    void on_pushButton_DFS_clicked();

    void on_PB_Back_Trip_clicked();

    void on_tableWidget_Trip_itemClicked(QTableWidgetItem *item);

    void on_pushButton_BFS_clicked();

    void on_PB_Next_Trip_clicked();

    void on_pushButton_Back_Summary_clicked();

    void on_pushButton_Add_Trip_clicked();

    void on_pushButton_Comfirm_Summary_clicked();

    void on_pushButton_distance_checker_clicked();

    void on_pushButton_get_distance_clicked();

    void on_pushButton_select_stadium_back_clicked();

    void on_comboBox_start_stadium_activated(const QString &arg1);

    void on_tableView_select_stadium_doubleClicked(const QModelIndex &index);

    void on_pushButton_clear_stadium_clicked();

    void on_pushButton_custom_trip_clicked();

    void on_pushButton_begin_shortest_custom_trip_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::database();
    login Login;

    Graph<QString> graph;
    Cart cart;



    // helper functions
    void play_sound();
    enum teams { ALL, AFC, NFC };
    void populate_teams(teams selection);
    void populate_stadiums(bool open_roof);
    void populate_players();
    void populate_surfaces();
    void populate_teamNames();
};

#endif // MAINWINDOW_H
