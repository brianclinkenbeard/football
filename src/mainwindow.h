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

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::database();
    login Login;
    int tracker;

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
