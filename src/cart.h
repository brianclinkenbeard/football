#ifndef CART_H
#define CART_H

#include "stadium.h"
#include <QVector>

class Cart{
public:

    void clear(){
        stadiumList.clear();
    }

    int size(){
        return stadiumList.size();
    }

    QVector<Stadium> getStadiumList(){
        return stadiumList;
    }

    void addStadium(QString stadium){
        Stadium temp;
        temp.setStadiumName(stadium);
        stadiumList.append(temp);
    }

    void addSouvenir(QString stadium, QString team, QString item, double price, int quantity){
        bool found = false;
        for(int i=0;i<stadiumList.size(); ++i){
            if(stadiumList[i].getName() == stadium){
                found = true;
                for(int j=0; j<quantity; ++j){
                    stadiumList[i].addSouvenir(team,item,price);
                }
            }
        }
        if(!found){
            this->addStadium(stadium);
            //too lazy to code so I recursively called it again
            this->addSouvenir(stadium, team, item, price, quantity);
        }
    }

    double getTotalAmount(){
        double cost = 0;
        for(int i=0; i<stadiumList.size(); ++i){
            cost += stadiumList[i].getTotalAmount();
        }
        qDebug() << cost;
        return cost;
    }

private:
    QVector<Stadium> stadiumList;
};


#endif // CART_H
