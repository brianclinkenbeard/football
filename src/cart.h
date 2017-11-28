#ifndef CART_H
#define CART_H

#include "stadium.h"
#include <QVector>

class Cart{
public:
    void addStadium(QString stadium){
        Stadium temp;
        temp.setStadiumName(stadium);
        stadiumList.append(temp);
    }

    void addSouvenir(QString stadium, QString team, QString item, double price){
        for(int i=0;i<stadiumList.size(); ++i){
            if(stadiumList[i].getName() == stadium){
                stadiumList[i].addSouvenir(team,item,price);
            }
        }
    }

    double getTotalAmount(){
        double cost = 0;
        for(int i=0; i<stadiumList.size(); ++i){
            cost += stadiumList[i].getTotalAmount();
        }
        return cost;
    }

private:
    QVector<Stadium> stadiumList;
};


#endif // CART_H
