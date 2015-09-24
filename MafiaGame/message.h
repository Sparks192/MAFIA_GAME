#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QJsonObject>
//
//
// Types declaration
//
typedef QString tMessage;
typedef qint32 tMessageType;


class MsgBuilderInterface
{
public:
    MsgBuilderInterface();
    virtual ~MsgBuilderInterface();

    virtual void     AddProperty(std::string prop, std::string value);
    virtual tMessage ConsructMsg();
};

class MessageFactoryInterace
{
public:
    MessageFactoryInterace();
    virtual ~MessageFactoryInterace();

    virtual tMessage CreateMsg(MsgBuilderInterface* msgBuilder);
};

class JsonMsgBuilder : public MsgBuilderInterface
{
public:
    JsonMsgBuilder();
    virtual ~JsonMsgBuilder();

    virtual void AddProperty(std::string prop, tMessage value) override;
    virtual tMessage ConsructMsg() override;
    void     LoadFromString(tMessage msg);
    tMessage GetPropertyValue(std::string prop);

private:
    QJsonObject mMsgObject;
};

class MessageFactory : public MessageFactoryInterace
{
public:
    MessageFactory();
    tMessage CreateMsg(MsgBuilderInterface* msgBuilder) override;
};

class Message
{
public:
    Message(tMessage msg);

    tMessageType GetMsgType();
    void SetMsgType(tMessageType msgtype);
    tMessage toString();

private:
    tMessage mMessage;
    MessageFactoryInterace* mMsgFactory;
    MsgBuilderInterface*    mMsgBuilder;
};

#endif // MESSAGE_H
