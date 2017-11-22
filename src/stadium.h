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
    Stadium(){
        stadiumName = "DEFAULT";
    }

    void setStadiumName(QString input){
        stadiumName = input;
    }

    void addSouvenir(QString team, QString item, double price){
        Souvenir temp;
        temp.team = team;
        temp.item = item;
        temp.price = price;
        souvenirList.append(temp);
    }

    double getTotalAmount(){
        double cost = 0;
        for(int i=0; i<souvenirList.size(); ++i){
            cost += souvenirList[i].price;
        }
        qDebug() << cost;
        return cost;
    }

    int getSouvenirQuantity(){
        qDebug() << souvenirList.size();
        return souvenirList.size();
    }

    QString getName(){
        return stadiumName;
    }

private:
    QVector<Souvenir> souvenirList;
    QString stadiumName;
};

#endif // STADIUM_H
