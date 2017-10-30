#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stWid->setCurrentWidget(ui->page_home);

    // stretch columns equally to fit width of table
    ui->tableView_stadium_capacity_list->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_teams_stadiums->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_go_to_stadium_capacity_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_stadium_capacity);
    QSqlQuery *query = new QSqlQuery(db);

    // set up the table
    query->prepare("SELECT StadiumName, TeamName, SeatingCapacity FROM TeamInfo ORDER BY SeatingCapacity");
    query->exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(*query);
    ui->tableView_stadium_capacity_list->setModel(model);

    // gets the total seating capacity (ignores duplicate stadium)
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

void MainWindow::on_pushButton_teams_stadiums_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_teams_stadiums);

    // show all teams on page open
    populate_teams(ALL);
}

void MainWindow::on_comboBox_selection_currentIndexChanged(int index)
{
    // clear old filters
    ui->comboBox_filters->clear();

    // TODO: maybe rewrite helper functions into one so we dont have a bunch of reused code
    switch (index) {
    case 0: // teams
        ui->comboBox_filters->addItems({ "All Teams", "American Baseball Conference", "National Baseball Conference" });
        break;
    case 1: // stadiums
        ui->comboBox_filters->addItems({ "All Stadiums", "Open Roof Stadiums" });
        break;
    case 2: // star players
        ui->comboBox_filters->addItems({ "All Players" });
        populate_players();
        break;
    case 3: // surfaces
        ui->comboBox_filters->addItems({ "All Surfaces" });
        populate_surfaces();
        // fall through
    }
}

void MainWindow::on_comboBox_filters_currentIndexChanged(int index)
{
    switch (ui->comboBox_selection->currentIndex()) {
    case 0: // teams
        populate_teams(static_cast<teams>(index)); // cast index to teams enum
        break;
    case 1: // stadiums
        populate_stadiums(static_cast<bool>(index));
        // fall through
    }
}

void MainWindow::populate_teams(teams selection)
{
    // set up the table
    QSqlQuery query(db);
    query.prepare("SELECT TeamName, StadiumName, SeatingCapacity, Location, Conference, SurfaceType, StadiumRoofType, StarPlayer FROM TeamInfo ORDER BY TeamName");
    query.exec();

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    // filter by conference
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(4); // Conference
    if (selection == AFC)
        proxyModel->setFilterRegExp("American Football Conference");
    else if (selection == NFC)
        proxyModel->setFilterRegExp("National Football Conference");

    ui->tableView_teams_stadiums->setModel(proxyModel);
}

void MainWindow::populate_stadiums(bool open_roof)
{
    // set up the table
    QSqlQuery query(db);
    query.prepare("SELECT StadiumName, TeamName, StadiumRoofType FROM TeamInfo ORDER BY StadiumName");
    query.exec();

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    // filter by roof type
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(2); // StadiumRoofType
    if (open_roof)
        proxyModel->setFilterRegExp("Open");

    ui->tableView_teams_stadiums->setModel(proxyModel);
}

void MainWindow::populate_players()
{
    // set up the table
    QSqlQuery query(db);
    query.prepare("SELECT StarPlayer, TeamName FROM TeamInfo ORDER BY TeamName");
    query.exec();

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    ui->tableView_teams_stadiums->setModel(model);
}

void MainWindow::populate_surfaces()
{
    // set up the table
    QSqlQuery query(db);
    query.prepare("SELECT StadiumName, SurfaceType, Location FROM TeamInfo ORDER BY SurfaceType");
    query.exec();

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query);

    ui->tableView_teams_stadiums->setModel(model);
}

void MainWindow::on_pushButton_team_stadium_back_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_home);
}
