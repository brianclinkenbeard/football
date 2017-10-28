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

    query->prepare("SELECT DISTINCT StadiumName FROM TeamInfo");
    query->exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(*query);
    ui->tableView_stadium_capacity_list->setModel(model);

}
