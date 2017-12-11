#include "stadiummap.h"

#include "stadiummap.h"

StadiumMap::StadiumMap()
{
    MapStadium AVAILABLE = {0, "EMPTY", true};
    for (int i = 0; i < 29; i++)
        buckets.push_back(AVAILABLE);
}

/* Double hash to the bucket array */
void StadiumMap::doubleHash(MapStadium inStadium)
{
    int single = hash(inStadium.key);
    int prime = hashprime(inStadium.key);
    std::vector<MapStadium>::const_iterator it = buckets.begin();

    if (buckets.at(single).available || buckets.at(single).key == inStadium.key)
    {
        std::advance(it, single);
        buckets.insert(it, inStadium);
    }
    else if (buckets.at(prime).available || buckets.at(prime).key == inStadium.key)
    {
        std::advance(it, prime);
        buckets.insert(it, inStadium);
    }
    else
    {
        for (int j = 0; j < 29; j++)
        {
            if (buckets.at((single + (j * prime)) % 29).available
                    || buckets.at((single + (j * prime)) % 29).key == inStadium.key)
            {
                advance(it, (single + (j * prime)) % 29);
                buckets.insert(it, inStadium);
                return;
            }
        }
    }
}

void StadiumMap::remove(int key)
{
    std::vector<MapStadium>::const_iterator it = buckets.begin();
    /* Used to delete data with shortening vector size */
    MapStadium AVAILABLE = {0, "EMPTY", true};
    for (int i = 0; i < 29; i++)
    {
        if (buckets.at(i).key == key)
        {
            buckets.insert(it, AVAILABLE);
            return;
        }
        it++;
    }
}

void StadiumMap::quadraticHash(MapStadium inStadium)
{
    int single = hash(inStadium.key);
    std::vector<MapStadium>::const_iterator it = buckets.begin();

    if (buckets.at(single).available || buckets.at(single).key == inStadium.key)
    {
        std::advance(it, single);
        buckets.insert(it, inStadium);
    }
    else
    {
        for (int j = 1; j < 29; j++)
        {
            if (buckets.at((single + (j * j)) % 29).available ||
                    buckets.at((single + (j * j)) % 29).key == inStadium.key)
            {
                std::advance(it, (single + (j * j)) % 29);
                buckets.insert(it, inStadium);
                return;
            }
        }
    }
}


/* Helper functions */
int StadiumMap::hash(int inKey) const
{
    return inKey % 29;
}

int StadiumMap::hashprime(int inKey) const
{
    return 17 - (inKey % 17);
}
