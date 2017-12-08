#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QQueue>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class AdminWindow;
}

/*!
 * \brief AdminWindow class
 * \class Holds everything an admin would need to do
 */
class AdminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = 0);
    ~AdminWindow();

private slots:
    void on_addSouvenirButton_clicked();

    void on_deleteSouvenirButton_clicked();

    void on_modifySouvenir_clicked();

    void on_updateDB_clicked();

    void on_deleteSouvenirTeamName_activated(const QString &arg1);

    void on_modifySouvenirTeamName_activated(const QString &arg1);

    void on_sumbitSouvenirButton_clicked();

    void on_deleteSouvenirButtonBox_clicked();

    void on_updateSouvenirButton_clicked();

    void on_addTeamButton_clicked();

    void on_changeTeamInfoButton_clicked();

    /*!
     * \brief reloadComboBoxes
     * \fn reloads combo boxes
     */
    void reloadComboBoxes();

    /*!
     * \brief reloadTableViews
     * \fn reloads table views
     */
    void reloadTableViews();

    /*!
     * \brief getStadiumName
     * \param teamName
     * \return QString
     * \fn gets name of stadium based on team name
     */
    QString getStadiumName(QString teamName);

private:
    Ui::AdminWindow *ui;
    QVector<QString> teamNames; /*!<    List of team names */
    QSqlDatabase db = QSqlDatabase::database(); /*!<    Database connection */
};

#endif // ADMINWINDOW_H
