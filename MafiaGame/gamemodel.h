#ifndef GAMEMODEL_H
#include <inttypes.h>

enum eDayTime
{
    DAY,
    NIGHT
};

const size_t dayDuration = 60;
const size_t nightDuration = 60;

class GameModel
{
public:
    GameModel();
};

#endif // GAMEMODEL_H
