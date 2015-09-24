#ifndef MESSAGETYPES_H
#define MESSAGETYPES_H
#include <QJsonObject>

enum ClientMessages
{
    TEXT_MSG,
    VOTE_MSG,
    HEALTH_MSG,
    SKIP_STEP_MSG,
    KILL_MSG,
    RECOGNIZE_MSG
};

enum ServerMessages
{
    RESPONSE_MSG,
    EVENT_MSG,
    NOTIFY_MSG
};


#endif // MESSAGETYPES_H
