#ifndef STADIUMMAP_H
#define STADIUMMAP_H

#include <QString>

struct MapStadium
{
    int key;
    QString name;
    bool available;
};

class StadiumMap
{
public:
    StadiumMap();
    void doubleHash(MapStadium inStadium);
    void remove(int key);
    void quadraticHash(MapStadium inStadium);
private:
    std::vector<MapStadium> buckets;
    int hash(int inKey) const;
    int hashprime(int inKey) const;
};

#endif // STADIUMMAP_H
