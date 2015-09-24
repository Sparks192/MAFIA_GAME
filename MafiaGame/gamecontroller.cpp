#include "gamecontroller.h"
#include "gamemodel.h"
#include <QStateMachine>
#include <QState>
#include <QTimer>

GameController::GameController()
{

}

void GameController::startGame()
{
    QStateMachine stateMachine;
    QState dayState;
    QState nightState;
    //stateMachine.addState(dayState);
    //stateMachine.addState(nightState);
    //stateMachine.setInitialState(dayState);
    //QTimer::singleShot(dayDuration,&stateMachine,SLOT(start()));
}

void GameController::stopGame()
{

}
