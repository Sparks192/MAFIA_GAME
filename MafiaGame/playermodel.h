#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H


#include "gamemodel.h"
#include <QString>

enum eFraction
{
    MAFIA,
    CITIZEN
};

class PlayerModel
{
public:
    PlayerModel();
    virtual void Vote();
    virtual void WriteMessage();
    size_t GetUID();

private:
    QString mName;
    bool mSkipMove;
    bool mHealthing;
    bool mAlive;
    size_t mUID;
    eFraction mFraction;
};

class Maffia : public PlayerModel
{

};

class Citizen : public PlayerModel
{

};

class Policeman : public PlayerModel
{

};

class Doctor : public PlayerModel
{

};

class TownsWoman : public PlayerModel
{

};
#endif // PLAYERMODEL_H
