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
    ui->tableView_Trip->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_Summary->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_select_stadium->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    qsrand(2175734);
    // populate team name combobox for single team info (in constructor so it happens only once)
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT DISTINCT TeamName FROM TeamInfo");
    query->exec();
    while (query->next())
        ui->comboBox_single_team->addItem(query->value(0).toString());

    graph.loadGraph(graph);

    // hide roof count label
    ui->openRoofCountLabel->hide();

    StadiumMap stadiumMap;
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
        StadiumMap stadiumMap;
        MapStadium mapStadium;
        mapStadium.key = qrand() % 5;
        mapStadium.name = query->value(0).toString();
        stadiumMap.doubleHash(mapStadium);
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
    ui->openRoofCountLabel->hide();

    // clear old filters
    ui->comboBox_filters->clear();

    // TODO: maybe rewrite helper functions into one so we dont have a bunch of reused code
    switch (index) {
    case 0: // teams
        ui->comboBox_filters->addItems({ "All Teams", "American Football Conference", "National Football Conference" });
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
        break;
    case 4: //Souvenirs
        ui->comboBox_filters->addItems({ "Select Team" });
        populate_teamNames();
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

    QSqlQuery *roofCount = new QSqlQuery(db);
    roofCount->prepare("SELECT COUNT(DISTINCT StadiumName) FROM TeamInfo WHERE StadiumRoofType == 'Open'");
    roofCount->exec();
    roofCount->next();

    int openCount = roofCount->value(0).toInt();
    qDebug() << openCount;

    // filter by roof type
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(2); // StadiumRoofType
    if (open_roof) {
        ui->openRoofCountLabel->show();
        proxyModel->setFilterRegExp("Open");
        ui->openRoofCountLabel->setText("Open Roof Count: " + QString::number(openCount));
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
    on_comboBox_filters_selectTeamName(arg1, ui->comboBox_selection->currentIndex());
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
    graph.loadGraph(graph);
    graph.DFS("Hard Rock Stadium");
    QVector<QString> list = graph.getOrder();

    ui->tableWidget_Trip->setRowCount(list.size());
    for(int i=0; i<list.size(); ++i){
        QTableWidgetItem *insert = new QTableWidgetItem(list[i]);
        ui->tableWidget_Trip->setItem(i,0,insert);
    }

    ui->lineEdit_Distance_Trip->insert(QString::number(graph.getTotalDistance()));
    ui->lineEdit_Distance_Summary->insert(QString::number(graph.getTotalDistance()));


}

void MainWindow::on_pushButton_BFS_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_Trip);
    graph.loadGraph(graph);
    graph.BFS("Lambeau Field");
    QVector<QString> list = graph.getOrder();

    ui->tableWidget_Trip->setRowCount(list.size());
    for(int i=0; i<list.size(); ++i){
        QTableWidgetItem *insert = new QTableWidgetItem(list[i]);
        ui->tableWidget_Trip->setItem(i,0,insert);
    }
    ui->lineEdit_Distance_Trip->insert(QString::number(graph.getTotalDistance()));
    ui->lineEdit_Distance_Summary->insert(QString::number(graph.getTotalDistance()));
}

void MainWindow::on_PB_Back_Trip_clicked()
{
    cart.clear();
    ui->tableWidget_Trip->clearContents();
    ui->tableWidget_Trip->setRowCount(0);
    ui->lineEdit_Distance_Trip->clear();
    ui->lineEdit_Cost_Summary->clear();
    ui->lineEdit_Distance_Summary->clear();
    ui->spinBox_Trip->setValue(0);
    ui->tableView_Trip->setModel(new QSqlQueryModel);
    ui->stWid->setCurrentWidget(ui->page_home);
    cart.clear();
}

void MainWindow::on_tableWidget_Trip_itemClicked(QTableWidgetItem *item)
{
    qDebug() << item->text();
    QString input = item->text();
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT Team, Item, Price FROM Souvenirs WHERE Stadium == :stadium");
    query->bindValue(":stadium", input);
    query->exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(*query);

    ui->tableView_Trip->setModel(model);

}

void MainWindow::on_pushButton_Add_Trip_clicked()
{
    if(ui->tableView_Trip->currentIndex().row() >-1 && ui->spinBox_Trip->text().toInt() > 0){

        QString stadium = ui->tableWidget_Trip->currentIndex().data(0).toString();
        QString team = ui->tableView_Trip->currentIndex().sibling(ui->tableView_Trip->currentIndex().row(), 0).data(0).toString();
        QString item = ui->tableView_Trip->currentIndex().sibling(ui->tableView_Trip->currentIndex().row(), 1).data(0).toString();
        double price = ui->tableView_Trip->currentIndex().sibling(ui->tableView_Trip->currentIndex().row(), 2).data(0).toDouble();

        qDebug() << stadium;
        qDebug() << team;
        qDebug() << item;
        qDebug() << price;

        cart.addSouvenir(stadium, team, item, price, ui->spinBox_Trip->text().toInt());

       QMessageBox::information(this, "Added to Cart", item + " was successfully added to your cart.");
    }
    else{
        QMessageBox::warning(this, "No Item Selected", "Please select the item you wish to\npurchase and the quantity.");

         qDebug() << "Souvenir or Quantity was not selected";
    }
    ui->spinBox_Trip->setValue(0);
}


void MainWindow::on_PB_Next_Trip_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_Summary);
    ui->lineEdit_Cost_Summary->insert(QString::number(cart.getTotalAmount()));

//    ui->lineEdit_Distance_Summary->insert(QString::number(graph.getTotalDistance()));
    ui->tableView_Trip->setModel(new QSqlQueryModel());

    QVector<Stadium> list = cart.getStadiumList();

    for(int i=0; i<list.size(); ++i){

        ui->tableWidget_Summary->insertRow(ui->tableWidget_Summary->rowCount());
        ui->tableWidget_Summary->setItem(ui->tableWidget_Summary->rowCount() - 1, 0, new QTableWidgetItem(list[i].getName()));
        ui->tableWidget_Summary->setItem(ui->tableWidget_Summary->rowCount() - 1, 1, new QTableWidgetItem(QString::number(list[i].getSouvenirQuantity())));
        ui->tableWidget_Summary->setItem(ui->tableWidget_Summary->rowCount() - 1, 2, new QTableWidgetItem(QString::number(list[i].getTotalAmount())));

    }
}

void MainWindow::on_pushButton_Back_Summary_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_Trip);
    ui->lineEdit_Cost_Summary->clear();
//    ui->lineEdit_Distance_Summary->clear();
    ui->tableWidget_Summary->clearContents();
    ui->tableWidget_Summary->setRowCount(0);
}


void MainWindow::on_pushButton_Comfirm_Summary_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_home);
    ui->lineEdit_Distance_Trip->clear();
    ui->lineEdit_Cost_Summary->clear();
    ui->lineEdit_Distance_Summary->clear();
    ui->tableWidget_Trip->clearContents();
    ui->tableWidget_Trip->setRowCount(0);
    ui->tableWidget_Summary->clearContents();
    ui->tableWidget_Summary->setRowCount(0);
    cart.clear();
}

void MainWindow::on_pushButton_distance_checker_clicked()
{
    graph.loadGraph(graph);

    ui->stWid->setCurrentWidget(ui->page_distance_checker);

    ui->distance_checker_label->setText("Pick any stadium to see the distance it will take to get to from " + getStadiumName("Los Angeles Rams")  + '!');

    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT DISTINCT Start FROM Distance WHERE Start != :teamName");
    query->bindValue(":teamName",getStadiumName("Los Angeles Rams"));
    query->exec();

    ui->combobox_distance_checker->addItem("<Select Stadium>");
    while(query->next()){
        ui->combobox_distance_checker->addItem(query->value(0).toString());
    }
    graph.Dijkstra(getStadiumName("Los Angeles Rams"));
}

QString MainWindow::getStadiumName(QString teamName)
{
    QString stadiumName;
    QSqlQuery *nameQuery = new QSqlQuery(db);

    nameQuery->prepare("SELECT StadiumName FROM TeamInfo WHERE TeamName == (:teamName)");
    nameQuery->bindValue(":teamName",teamName);
    nameQuery->exec();

    while (nameQuery->next()) {
         stadiumName = nameQuery->value(0).toString();
    }

    return stadiumName;
}

void MainWindow::on_pushButton_get_distance_clicked()
{
    if(ui->combobox_distance_checker->currentText() != "<Select Stadium>")
    {
        QString stadiumToVisit = ui->combobox_distance_checker->currentText();
        ui->stWid->setCurrentWidget(ui->page_Trip);

        ui->tableWidget_Trip->setRowCount(2);
        QTableWidgetItem *insert = new QTableWidgetItem(getStadiumName("Los Angeles Rams"));
        ui->tableWidget_Trip->setItem(0,0,insert);


        insert = new QTableWidgetItem(stadiumToVisit);
        ui->tableWidget_Trip->setItem(1,0,insert);

        int distance;

        distance = graph.getCost(stadiumToVisit);
        ui->lineEdit_Distance_Trip->clear();
        ui->lineEdit_Distance_Summary->clear();
        ui->lineEdit_Distance_Trip->setText(QString::number(distance));
        ui->lineEdit_Distance_Summary->setText(QString::number(distance));

        qDebug() << distance;
        ui->combobox_distance_checker->clear();
    }
}

void MainWindow::on_pushButton_Distance_Checker_Back_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_home);
    ui->combobox_distance_checker->clear();
}

void MainWindow::on_pushButton_select_stadium_back_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_home);
    ui->comboBox_start_stadium->clear();
    ui->listWidget_selected_stadium->clear();

    ui->tableView_select_stadium->setModel(new QSqlQueryModel());
}

void MainWindow::on_comboBox_start_stadium_activated(const QString &arg1)
{
    ui->listWidget_selected_stadium->clear();
    ui->tableView_select_stadium->setModel(new QSqlQueryModel());
    if(arg1 != "<Select Starting Stadium>"){
        ui->listWidget_selected_stadium->clear();
        qDebug() << "Starting stadium: " << arg1;

        QSqlQuery *query = new QSqlQuery(db);
        query->prepare("SELECT DISTINCT StadiumName FROM TeamInfo WHERE StadiumName != :stadium");
        query->bindValue(":stadium", arg1);
        query->exec();

        //unhides rows
        for(int i=0; i<query->size() ; ++i){
            ui->tableView_select_stadium->showRow(i);
        }

        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(*query);

        ui->tableView_select_stadium->setModel(model);
    }
    else{
        ui->tableView_select_stadium->setModel(new QSqlQueryModel());
    }
}

void MainWindow::on_tableView_select_stadium_doubleClicked(const QModelIndex &index)
{
    ui->tableView_select_stadium->hideRow(index.row());
    //dont know how this works
    QModelIndex i = index.sibling(index.row(), 0);
    ui->listWidget_selected_stadium->addItem(i.model()->data(index).toString());
}

void MainWindow::on_pushButton_clear_stadium_clicked()
{
    //clears list widget and resets the table view
    ui->listWidget_selected_stadium->clear();
    ui->tableView_select_stadium->setModel(new QSqlQueryModel());
    if(ui->comboBox_start_stadium->currentText() != "<Select Starting Stadium>")
    {
        QSqlQuery *query = new QSqlQuery(db);
        query->prepare("SELECT DISTINCT StadiumName FROM TeamInfo WHERE StadiumName != :stadium");
        query->bindValue(":stadium", ui->comboBox_start_stadium->currentText());
        query->exec();
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(*query);
        ui->tableView_select_stadium->setModel(model);
    }
}

void MainWindow::on_pushButton_custom_trip_clicked()
{
    ui->stWid->setCurrentWidget(ui->page_select_stadium);
    ui->comboBox_start_stadium->insertItem(0, "<Select Starting Stadium>");
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT DISTINCT StadiumName FROM TeamInfo");
    query->exec();

    int row = 1;
    while(query->next()){
        qDebug() << "Adding to combobox: " << query->value(0).toString();
        ui->comboBox_start_stadium->insertItem(row, query->value(0).toString());
        ++row;
    }
}

void MainWindow::on_pushButton_begin_shortest_custom_trip_clicked()
{
    graph.loadGraph(graph);
    if(ui->listWidget_selected_stadium->count() > 0)
    {
        graph.nameVector.clear();
        graph.resetDistance();
        graph.nameVector.push_back(ui->comboBox_start_stadium->currentText());
        for(int i = 0; i < ui->listWidget_selected_stadium->count(); i++) {
            graph.nameVector.push_back(ui->listWidget_selected_stadium->item(i)->text());
        }
        graph.recursiveDijkstra(ui->comboBox_start_stadium->currentText(),ui->listWidget_selected_stadium->count()+1);

        QVector<QString> list = graph.getOrder();

        ui->tableWidget_Trip->setRowCount(list.size());
        for(int i=0; i<list.size(); ++i){
            QTableWidgetItem *insert = new QTableWidgetItem(list[i]);
            ui->tableWidget_Trip->setItem(i,0,insert);
        }

        ui->lineEdit_Distance_Trip->insert(QString::number(graph.getTotalDistance()));
        ui->lineEdit_Distance_Summary->insert(QString::number(graph.getTotalDistance()));
        ui->stWid->setCurrentWidget(ui->page_Trip);

        ui->comboBox_start_stadium->clear();
        ui->tableView_select_stadium->setModel(new QSqlQueryModel());
        ui->listWidget_selected_stadium->clear();
    }

}

void MainWindow::on_pushButton_begin_specific_custom_trip_clicked()
{
    graph.loadGraph(graph);
    if(ui->listWidget_selected_stadium->count() > 0)
    {
        graph.nameVector.clear();
        graph.resetDistance();
        graph.nameVector.push_back(ui->comboBox_start_stadium->currentText());
        for(int i = 0; i < ui->listWidget_selected_stadium->count(); i++) {
            graph.nameVector.push_back(ui->listWidget_selected_stadium->item(i)->text());
        }
        graph.startSpecificRoute(ui->comboBox_start_stadium->currentText(),ui->listWidget_selected_stadium->count()+1);

        QVector<QString> list = graph.getOrder();

        ui->tableWidget_Trip->setRowCount(list.size());
        for(int i=0; i<list.size(); ++i){
            QTableWidgetItem *insert = new QTableWidgetItem(list[i]);
            ui->tableWidget_Trip->setItem(i,0,insert);
        }

        ui->lineEdit_Distance_Trip->insert(QString::number(graph.getTotalDistance()));
        ui->lineEdit_Distance_Summary->insert(QString::number(graph.getTotalDistance()));
        ui->stWid->setCurrentWidget(ui->page_Trip);

        ui->comboBox_start_stadium->clear();
        ui->tableView_select_stadium->setModel(new QSqlQueryModel());
        ui->listWidget_selected_stadium->clear();
    }
}

void MainWindow::on_pushButton_mst_clicked()
{
    //calls from database just incase stadium name is changed.
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT Start FROM Distance");
    query->exec();

    query->next();

    graph.loadGraph(graph);
    int distance = graph.MST(query->value(0).toString());

    QMessageBox message;
    message.setText("MST Total Distance: " + QString::number(distance));
    message.exec();

}


