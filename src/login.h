#ifndef LOGIN_H
#define LOGIN_H

#include "adminwindow.h"
#include <QDialog>
#include <QVector>

/*!
 * \brief user struct
 * \struct holds user information
 */
struct user
{
    QString name;
    QString password;
    bool okAdmin;
};

namespace Ui {
class login;
}

/*!
 * \brief login class
 * \class Login class
 */
class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

    /*!
     * \brief isAccount
     * \param nameIn
     * \param passIn
     * \return bool
     * \fn checks if there is an account
     */
    bool isAccount(QString nameIn, QString passIn);

    /*!
     * \brief isAdmin
     * \param nameIn
     * \param passIn
     * \return bool
     * \fn checks if user is admin or not
     */
    bool isAdmin(QString nameIn, QString passIn);

    //QString index(QString nameIn, QString passIn);

    /*!
     * \brief addAccount
     * \param nameIn
     * \param passIn
     * \param adminIn
     * \fn adds account
     */
    void addAccount(QString nameIn, QString passIn, bool adminIn);



private slots:
    void on_LoginButton_clicked();

private:
    Ui::login *ui;
    QVector<user> accounts; /*!<   Vector of accounts */
    AdminWindow adminWindow; /*!<    AdminWindow reference */
};

#endif // LOGIN_H
