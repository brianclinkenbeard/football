#ifndef STADIUM_H
#define STADIUM_H
#include <QVector>
#include <QString>
struct Souvenir{
    QString team;
    QString item;
    double price;
};

class Stadium{
public:
    /*!
     * \brief Stadium
     * \fn default constructor
     */
    Stadium()
    {
        stadiumName = "DEFAULT";
    }

    /*!
     * \brief setStadiumName
     * \param input
     * \fn constructor
     */
    void setStadiumName(QString input)
    {
        stadiumName = input;
    }

    /*!
     * \brief addSouvenir
     * \param team
     * \param item
     * \param price
     * \fn adds a new souvenir to the team
     */
    void addSouvenir(QString team, QString item, double price)
    {
        Souvenir temp;
        temp.team = team;
        temp.item = item;
        temp.price = price;
        souvenirList.append(temp);
    }

    /*!
     * \brief getTotalAmount
     * \return double total amount spent
     */
    double getTotalAmount()
    {
        double cost = 0;
        for(int i=0; i<souvenirList.size(); ++i)
            cost += souvenirList[i].price;

        qDebug() << cost;
        return cost;
    }

    /*!
     * \brief getSouvenirQuantity
     * \return int quantity of souvenirs bought
     */
    int getSouvenirQuantity()
    {
        qDebug() << souvenirList.size();
        return souvenirList.size();
    }

    /*!
     * \brief getName
     * \return QString name of stadium
     */
    QString getName()
    {
        return stadiumName;
    }

private:
    QVector<Souvenir> souvenirList; /*! < List of souvenirs */
    QString stadiumName;            /*! < Name of stadium   */
};

#endif // STADIUM_H
