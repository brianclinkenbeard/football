#ifndef CART_H
#define CART_H

#include "stadium.h"
#include <QVector>

class Cart {
public:
    /*!
     * \brief clear
     * \fn clears stadium list
     */
    void clear()
    {
        stadiumList.clear();
    }

    /*!
     * \brief size
     * \return int size of stadium list
     */
    int size()
    {
        return stadiumList.size();
    }

    /*!
     * \brief getStadiumList
     * \return stadium list
     */
    QVector<Stadium> getStadiumList()
    {
        return stadiumList;
    }

    /*!
     * \brief addStadium
     * \param stadium
     * \fn adds a stadium to the stadium list
     */
    void addStadium(QString stadium)
    {
        Stadium temp;
        temp.setStadiumName(stadium);
        stadiumList.append(temp);
    }

    /*!
     * \brief addSouvenir
     * \param stadium
     * \param team
     * \param item
     * \param price
     * \param quantity
     * \fn adds a souvenir to a stadium/team
     */
    void addSouvenir(QString stadium, QString team, QString item, double price, int quantity)
    {
        bool found = false;
        for (int i=0;i<stadiumList.size(); ++i) {
            if (stadiumList[i].getName() == stadium) {
                found = true;
                for (int j=0; j<quantity; ++j) {
                    stadiumList[i].addSouvenir(team,item,price);
                }
            }
        }
        if (!found) {
            this->addStadium(stadium);
            //too lazy to code so I recursively called it again
            this->addSouvenir(stadium, team, item, price, quantity);
        }
    }

    /*!
     * \brief getTotalAmount
     * \return double total amount spent
     */
    double getTotalAmount()
    {
        double cost = 0;
        for (int i=0; i<stadiumList.size(); ++i)
            cost += stadiumList[i].getTotalAmount();

        qDebug() << cost;
        return cost;
    }

private:
    QVector<Stadium> stadiumList;   /*!<    List of Stadiums */
};


#endif // CART_H
