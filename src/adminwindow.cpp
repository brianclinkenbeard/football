#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    ui->addBox->hide();
    ui->deleteBox->hide();
    ui->modifyBox->hide();

    QWidget::setWindowTitle("Football Admin Window");

    QSqlQuery *nameQuery = new QSqlQuery(db);
    QSqlQuery *souvenirQuery = new QSqlQuery(db);
    QSqlQuery *newTeamQuery = new QSqlQuery(db);
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlQueryModel * newTeamModel = new QSqlQueryModel();
    QSqlQueryModel * souvenirModel = new QSqlQueryModel();

    newTeamQuery->prepare("SELECT * FROM TeamInfoExpansion");
    newTeamQuery->exec();

    newTeamModel->setQuery(*newTeamQuery);
    ui->newTeamTableView->setModel(newTeamModel);
    ui->newTeamTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    nameQuery->prepare("select distinct Team FROM Souvenirs");
    if(nameQuery->exec())
        while (nameQuery->next()) {
            teamNames.push_back(nameQuery->value(0).toString());
        }

    souvenirQuery->prepare("SELECT * FROM Souvenirs Order by Team asc");
    souvenirQuery->exec();

    souvenirModel->setQuery(*souvenirQuery);
    ui->souvenirTableView->setModel(souvenirModel);
    ui->souvenirTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->souvenirTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < teamNames.size(); i++) {
        ui->addSouvenirTeamName->addItem(teamNames[i]);
        ui->deleteSouvenirTeamName->addItem(teamNames[i]);
        ui->modifySouvenirTeamName->addItem(teamNames[i]);
        ui->teamNameComboBox->addItem(teamNames[i]);
    }

    QSqlQuery *teamQuery = new QSqlQuery(db);
    teamQuery->prepare("SELECT * FROM TeamInfo ORDER by TeamName asc");
    teamQuery->exec();

    model->setQuery(*teamQuery);
    ui->editTeamsView->setModel(model);
    ui->editTeamsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->editTeamsView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_addSouvenirButton_clicked()
{
    ui->addBox->show();
}

void AdminWindow::on_deleteSouvenirButton_clicked()
{
     ui->deleteBox->show();
}

void AdminWindow::on_modifySouvenir_clicked()
{
     ui->modifyBox->show();
}

void AdminWindow::on_updateDB_clicked()
{
    QSqlQuery *souvenirQuery = new QSqlQuery(db);
    QSqlQueryModel * model = new QSqlQueryModel();

    souvenirQuery->prepare("SELECT * FROM Souvenirs ORDER by Team asc");
    souvenirQuery->exec();

    model->setQuery(*souvenirQuery);
    ui->souvenirTableView->setModel(model);
    ui->souvenirTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->souvenirTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void AdminWindow::on_deleteSouvenirTeamName_activated(const QString &arg1)
{
    ui->deleteSouvenirName->clear();
    QVector<QString> souvenirNames;
    //sets up the querys
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT Item FROM Souvenirs WHERE Team == (:name)");
    query->bindValue(":name",arg1);
    query->exec();

    while (query->next()) {
        souvenirNames.push_back(query->value(0).toString());
    }

    for (int i = 0; i < souvenirNames.size(); i++) {
        ui->deleteSouvenirName->addItem(souvenirNames[i]);
    }
}



void AdminWindow::on_modifySouvenirTeamName_activated(const QString &arg1)
{
    ui->modifySouvenirName->clear();
    QQueue<QString> souvenirNames;
    //sets up the querys
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("SELECT Item FROM Souvenirs WHERE Team == (:name)");
    query->bindValue(":name",arg1);
    query->exec();

    while (query->next()) {
        souvenirNames.enqueue(query->value(0).toString());
    }

    for (int i = 0; i < souvenirNames.size(); i++) {
        ui->modifySouvenirName->addItem(souvenirNames[i]);
    }
}

void AdminWindow::on_sumbitSouvenirButton_clicked()
{
    QString teamName = ui->addSouvenirTeamName->currentText();
    QString name = ui->addSouvenirName->text();
    double value = ui->addSouvenirDoubleSpinBox->value();

    QString cost = QString::number(value);

    //sets up the querys
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("INSERT INTO Souvenirs (Team,Item,Price) VALUES (:team, :name, :cost)");
    query->bindValue(":team", teamName);
    query->bindValue(":name", name);
    query->bindValue(":cost", cost);

    if(query->exec())
        QMessageBox::information(this,"Souvenir successfully Added", "Souvenir Successfully Added", QMessageBox::Ok);
    else
        QMessageBox::information(this,"Souvenir not successfully Added", "Souvenir Not Successfully Added", QMessageBox::Ok);

    ui->addSouvenirName->clear();
    ui->addBox->hide();
}

void AdminWindow::on_deleteSouvenirButtonBox_clicked()
{
    QString team = ui->deleteSouvenirTeamName->currentText();
    QString name = ui->deleteSouvenirName->currentText();

    //sets up the querys
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("DELETE FROM Souvenirs WHERE Team == (:team) AND Item == (:name)");
    query->bindValue(":team", team);
    query->bindValue(":name", name);

    if(query->exec())
        QMessageBox::information(this,"Souvenir successfully deleted", "Souvenir Successfully Deleted", QMessageBox::Ok);
    else
        QMessageBox::information(this,"Souvenir not successfully deleted", "Souvenir Not Successfully Deleted", QMessageBox::Ok);

    ui->deleteBox->hide();
}

void AdminWindow::on_updateSouvenirButton_clicked()
{
    QString team = ui->modifySouvenirTeamName->currentText();
    QString name = ui->modifySouvenirName->currentText();
    double value = ui->modifySouvenirDoubleSpinBox->value();

    QString cost = QString::number(value);

    //sets up the querys
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("UPDATE Souvenirs SET Price = (:cost) WHERE Team = (:team) AND Item = (:name)");
    query->bindValue(":team", team);
    query->bindValue(":name", name);
    query->bindValue(":cost",cost);

    if(query->exec())
        QMessageBox::information(this,"Souvenir successfully Updated", "Souvenir Successfully Updated", QMessageBox::Ok);
    else
        QMessageBox::information(this,"Souvenir not successfully Updated", "Souvenir Not Successfully Updated", QMessageBox::Ok);
    ui->modifyBox->hide();
}

void AdminWindow::on_addTeamButton_clicked()
{
    QSqlQuery *insertQuery = new QSqlQuery(db);
    insertQuery->prepare("INSERT INTO TeamInfo SELECT * From TeamInfoExpansion");
    if(insertQuery->exec())
        ui->addTeamButton->setEnabled(false);
    else
        QMessageBox::critical(this,"Failure to add new teams","Failure to add new teams",QMessageBox::Ok);

    QSqlQuery *insertSouv = new QSqlQuery(db);
    insertSouv->prepare("INSERT INTO Souvenirs SELECT * From SouvenirsExpansion");
    if(insertSouv->exec())
        ui->addTeamButton->setEnabled(false);
    else
        QMessageBox::critical(this,"Failure to add new teams","Failure to add new teams",QMessageBox::Ok);

    QSqlQuery *insertDistance = new QSqlQuery(db);
    insertDistance->prepare("INSERT INTO Distance SELECT * From DistanceExpansion");
    if(insertDistance->exec())
        ui->addTeamButton->setEnabled(false);
    else
        QMessageBox::critical(this,"Failure to add new teams","Failure to add new teams",QMessageBox::Ok);

    reloadComboBoxes();
    reloadTableViews();
}

void AdminWindow::on_changeTeamInfoButton_clicked()
{
    QString teamName = ui->teamNameComboBox->currentText();
    QString stadiumName = ui->newStadiumName->text();
    int capacity = ui->newCapacityBox->value();

    QString cap = QString::number(capacity);

//    unsigned int length = cap.length(); // Get the length of the string, so we know when we have to stop
//    QString finalString; // Will be our output
//    unsigned int commaOffset = length%3; // Get the comma offset
//    for (unsigned int i = 0; i < length; ++i) {
//        // If our Index%3 == CommaOffset and this isn't first character, add a comma
//        if(i%3 == commaOffset && i)
//            finalString += ','; // Add the comma

//        finalString.append(cap[i]); // Add the original character
//    }


    QSqlQuery *updateQuery = new QSqlQuery(db);
    QSqlQuery *teamQuery = new QSqlQuery(db);
    QSqlQueryModel * model = new QSqlQueryModel();

    //Updates the current info
    updateQuery->prepare("UPDATE TeamInfo SET StadiumName = (:stadiumName), SeatingCapacity = (:capacityNum) WHERE Team = (:team)");
    updateQuery->bindValue(":stadiumName",stadiumName);
    updateQuery->bindValue(":capacityNum",cap);
    updateQuery->bindValue(":team",teamName);
    updateQuery->exec();

    //Shows the updated db info
    teamQuery->prepare("SELECT * FROM TeamInfo ORDER by TeamName asc");
    teamQuery->exec();

    model->setQuery(*teamQuery);
    ui->editTeamsView->setModel(model);
    ui->editTeamsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->editTeamsView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void AdminWindow::reloadComboBoxes()
{
    QSqlQuery *nameQuery = new QSqlQuery(db);
    ui->addSouvenirTeamName->clear();
    ui->deleteSouvenirTeamName->clear();
    ui->modifySouvenirTeamName->clear();
    ui->teamNameComboBox->clear();
    teamNames.clear();

    nameQuery->prepare("select distinct Team FROM Souvenirs");
    if(nameQuery->exec())
        while (nameQuery->next()) {
            teamNames.push_back(nameQuery->value(0).toString());
        }

    for (int i = 0; i < teamNames.size(); i++) {
        ui->addSouvenirTeamName->addItem(teamNames[i]);
        ui->deleteSouvenirTeamName->addItem(teamNames[i]);
        ui->modifySouvenirTeamName->addItem(teamNames[i]);
        ui->teamNameComboBox->addItem(teamNames[i]);
    }
}

void AdminWindow::reloadTableViews()
{
    QSqlQuery *souvenirQuery = new QSqlQuery(db);
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlQueryModel * souvenirModel = new QSqlQueryModel();

    souvenirQuery->prepare("SELECT * FROM Souvenirs Order by Team asc");
    souvenirQuery->exec();

    souvenirModel->setQuery(*souvenirQuery);
    ui->souvenirTableView->setModel(souvenirModel);
    ui->souvenirTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->souvenirTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    QSqlQuery *teamQuery = new QSqlQuery(db);
    teamQuery->prepare("SELECT * FROM TeamInfo ORDER by TeamName asc");
    teamQuery->exec();

    model->setQuery(*teamQuery);
    ui->editTeamsView->setModel(model);
    ui->editTeamsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->editTeamsView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
