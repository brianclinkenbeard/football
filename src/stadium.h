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

private:
    QVector<Souvenir> souvenirList;
};

#endif // STADIUM_H
