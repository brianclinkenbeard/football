#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stWid->setCurrentWidget(ui->page_home);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_go_to_stadium_capacity_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_stadium_capacity);
    QSqlQuery *query = new QSqlQuery(db);

    /////////////////////////////////////////
    //Sets up the table
    query->prepare("SELECT StadiumName, TeamName, SeatingCapacity FROM TeamInfo ORDER BY SeatingCapacity");
    query->exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(*query);
    ui->tableView_stadium_capacity_list->setModel(model);
    ui->tableView_stadium_capacity_list->setColumnWidth(0,220);
    ui->tableView_stadium_capacity_list->setColumnWidth(1,220);
    ui->tableView_stadium_capacity_list->setColumnWidth(2,100);
    /////////////////////////////////////////
    //Gets the total seating capacity (ignores duplicate stadium)
    query->prepare("SELECT DISTINCT StadiumName FROM TeamInfo");
    query->exec();
    long totalCapacity = 0;

    QSqlQuery *seatQuery = new QSqlQuery(db);
    while(query->next()){
        seatQuery->prepare("SELECT DISTINCT StadiumName, SeatingCapacity FROM TeamInfo WHERE StadiumName = (:stadium) ");
        QString temp = query->value(0).toString();
        seatQuery->bindValue(":stadium", temp);
        seatQuery->exec();
        seatQuery->next();
        totalCapacity += seatQuery->value(1).toInt();
    }
    ui->lineEdit_stadium_capacity->setText(QString::number(totalCapacity));
}

void MainWindow::on_btn_stadium_capacity_back_clicked()
{
    //Deletes content of table and lineEdit
    QSqlQueryModel *model = new QSqlQueryModel();
    ui->tableView_stadium_capacity_list->setModel(model);
    ui->lineEdit_stadium_capacity->clear();
    ui->stWid->setCurrentWidget(ui->page_home);
}
