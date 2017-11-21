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
    ui->tableView_single_team_info->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_Trip->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // populate team name combobox for single team info (in constructor so it happens only once)
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT DISTINCT TeamName FROM TeamInfo");
    query->exec();
    while (query->next())
        ui->comboBox_single_team->addItem(query->value(0).toString());

    graph.loadGraph(graph);

//    graph.DFS("Hard Rock Stadium");
//    QVector<QString> temp = graph.getOrder();
//    qDebug() << "DFS traversal: \n";
//    for(int i=0; i<temp.size(); ++i){
//        qDebug() << temp[i];
//    }
//    qDebug() << "end\n";

//    graph.BFS("Lambeau Field");
//    temp = graph.getOrder();
//    qDebug() << "BFS traversal: \n";
//    for(int i=0; i<temp.size(); ++i){
//        qDebug() << temp[i];
//    }
//    qDebug() << "end\n";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_go_to_stadium_capacity_clicked()
{ 
    reloadComboBoxes();
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
    reloadComboBoxes();
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
        ui->comboBox_filters->addItems({ "All Teams", "American Football Conference", "National Football Conference" });
        tracker = 0;
        break;
    case 1: // stadiums
        ui->comboBox_filters->addItems({ "All Stadiums", "Open Roof Stadiums" });
        tracker = 1;
        break;
    case 2: // star players
        ui->comboBox_filters->addItems({ "All Players" });
        populate_players();
        tracker = 2;
        break;
    case 3: // surfaces
        ui->comboBox_filters->addItems({ "All Surfaces" });
        populate_surfaces();
        tracker = 3;
        break;
    case 4: //Souvenirs
        ui->comboBox_filters->addItems({ "Select Team" });
        populate_teamNames();
        tracker = 4;
        //fall through
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
    if (open_roof) {
        proxyModel->setFilterRegExp("Open");
        ui->openRoofCountLabel->setText("Open Roof Count: " + QString::number(proxyModel->rowCount()));
    }

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

void MainWindow::populate_teamNames()
{
    QSqlQuery query(db);
    query.prepare("SELECT DISTINCT TeamName FROM TeamInfo ORDER BY TeamName asc");
    query.exec();

    while(query.next())
    {
        ui->comboBox_filters->addItem(query.value(0).toString());
    }
}

void MainWindow::on_pushButton_team_stadium_back_clicked()
{

    ui->stWid->setCurrentWidget(ui->page_home);
}

void MainWindow::on_adminLoginButton_clicked()
{

    Login.show();
}

void MainWindow::on_comboBox_filters_selectTeamName(const QString &arg1,int index)
{
    if(index == 4)
    {
        QSqlQueryModel * souvenirModel = new QSqlQueryModel();

        //sets up the query
        QSqlQuery *query = new QSqlQuery(db);
        query->prepare("SELECT * FROM Souvenirs WHERE Team == (:teamName)");
        query->bindValue(":teamName",arg1);
        query->exec();

        souvenirModel->setQuery(*query);

        ui->tableView_teams_stadiums->setModel(souvenirModel);
    }
}

void MainWindow::on_comboBox_filters_activated(const QString &arg1)
{
    on_comboBox_filters_selectTeamName(arg1,tracker);
}

void MainWindow::on_pushButton_team_information_clicked()
{
    reloadComboBoxes();
    ui->stWid->setCurrentWidget(ui->page_single_team_info);
}

void MainWindow::on_pushButton_single_team_back_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_home);
}

void MainWindow::on_comboBox_single_team_activated(const QString &arg1)
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT * FROM TeamInfo WHERE TeamName == (:teamName)");
    query->bindValue(":teamName", arg1);
    query->exec();

    model->setQuery(*query);

    ui->tableView_single_team_info->setModel(model);
}

void MainWindow::reloadComboBoxes()
{
    QSqlQuery *query = new QSqlQuery(db);
    ui->comboBox_single_team->clear();

    query->prepare("SELECT DISTINCT TeamName FROM TeamInfo");
    query->exec();
    while (query->next())
        ui->comboBox_single_team->addItem(query->value(0).toString());
}




void MainWindow::on_pushButton_DFS_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_Trip);
    graph.DFS("Hard Rock Stadium");
    QVector<QString> list = graph.getOrder();

    ui->tableWidget_Trip->setRowCount(list.size());
    for(int i=0; i<list.size(); ++i){
        QTableWidgetItem *insert = new QTableWidgetItem(list[i]);
        ui->tableWidget_Trip->setItem(i,0,insert);
    }
}


void MainWindow::on_pushButton_BFS_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_Trip);
    graph.BFS("Lambeau Field");
    QVector<QString> list = graph.getOrder();

    ui->tableWidget_Trip->setRowCount(list.size());
    for(int i=0; i<list.size(); ++i){
        QTableWidgetItem *insert = new QTableWidgetItem(list[i]);
        ui->tableWidget_Trip->setItem(i,0,insert);
    }
}

void MainWindow::on_PB_Back_Trip_clicked()
{
    ui->tableWidget_Trip->clearContents();
    ui->tableWidget_Trip->setRowCount(0);
    ui->lineEdit_Distance_Trip->clear();
    ui->doubleSpinBox_Trip->setValue(0);
    ui->tableView_Trip->setModel(new QSqlQueryModel);
    ui->stWid->setCurrentWidget(ui->page_home);
}

void MainWindow::on_tableWidget_Trip_itemClicked(QTableWidgetItem *item)
{
    qDebug() << item->text();
    QString input = item->text();
    QSqlQuery *teamQuery = new QSqlQuery(db);
    teamQuery->prepare("SELECT TeamName FROM TeamInfo WHERE StadiumName == :stadium");
    teamQuery->bindValue(":stadium", input);
    teamQuery->exec();

//    QSqlQuery *souvenirQuery = new QSqlQuery(db);
//    while(teamQuery->next()){
//        qDebug() << teamQuery->value(0);
//        souvenirQuery->prepare("SELECT * FROM Souvenirs WHERE Team == :team");
//        souvenirQuery->bindValue(":team", teamQuery->value(0).toString());
//        souvenirQuery->exec();




//      }

//    QSqlQueryModel *model = new QSqlQueryModel();
//    model->setQuery(*teamQuery);

//    ui->tableView_Trip->setModel(model);

}

