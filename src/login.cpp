#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    addAccount("A", "A", true);
}

login::~login()
{
    delete ui;
}

bool login::isAccount(QString nameIn, QString passIn)
{
    /*
     * Input: STRING username guess, STRING password guess
     * Output: BOOL true if found in accounts vector, false if otherwise
     */
    // this will be returned as the state of the inputted variables
    for (int index = 0; index < accounts.size(); index++) {
        if (nameIn == accounts[index].name && passIn == accounts[index].password)
            return true;
    }

    // if no account found return false
    return false;
}

bool login::isAdmin(QString nameIn, QString passIn)
{
    int accountIndex;     //index of the account searched for

    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].name == nameIn)
            accountIndex = i;
    }

    if (isAccount(nameIn,passIn)) {
       if (accounts.at(accountIndex).okAdmin == true)
           return true;
    }

    // if no account return false
    return false;
}

void login::addAccount(QString nameIn, QString passIn, bool adminIn)
{
    user insert;

    // assumes that the argument is valid variables
    insert.name = nameIn;
    insert.password = passIn;
    insert.okAdmin = adminIn;
    // adds the inserted variables into the struct
    accounts.push_back(insert);
}


void login::on_LoginButton_clicked()
{
    QString inName = ui->UserNameInput->text();
    QString inPass = ui->PasswordInput->text();

    if (isAccount(inName, inPass) && isAdmin(inName, inPass))
        this->close();
}
