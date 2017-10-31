#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class AdminWindow;
}

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

private:
    Ui::AdminWindow *ui;
    QSqlDatabase db = QSqlDatabase::database();
};

#endif // ADMINWINDOW_H
