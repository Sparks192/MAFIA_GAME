#include "message.h"

#include <QJsonDocument>

MsgBuilderInterface::MsgBuilderInterface()
{
}

MsgBuilderInterface::~MsgBuilderInterface()
{
}

void MsgBuilderInterface::AddProperty(std::string prop, std::string value)
{
}

tMessage MsgBuilderInterface::ConsructMsg()
{
}

MessageFactoryInterace::MessageFactoryInterace()
{
}

MessageFactoryInterace::~MessageFactoryInterace()
{
}

tMessage MessageFactoryInterace::CreateMsg(MsgBuilderInterface *msgBuilder)
{
}

JsonMsgBuilder::JsonMsgBuilder()
{
}

JsonMsgBuilder::~JsonMsgBuilder()
{
}

JsonMsgBuilder::AddProperty(std::string prop, tMessage value)
{
    mMsgObject[prop] = value;
}

tMessage JsonMsgBuilder::ConsructMsg()
{
    QJsonDocument doc(mMsgObject);
    tMessage msg(doc.toJson(QJsonDocument::Indented));
    mMsgObject = QJsonObject();
    return msg;
}

void JsonMsgBuilder::LoadFromString(tMessage msg)
{
    mMsgObject = QJsonDocument::fromJson(msg.toUtf8()).object();
}

tMessage JsonMsgBuilder::GetPropertyValue(std::string prop)
{
    return mMsgObject[prop];
}

MessageFactory::MessageFactory()
{
}

MessageFactory::CreateMsg(MsgBuilderInterface *msgBuilder)
{
}

Message::Message(tMessage msg)
    :mMessage(msg)
{
    mMsgFactory = new MessageFactory();
    mMsgBuilder = new JsonMsgBuilder();
}

tMessageType Message::GetMsgType()
{
    return 0;
}

void Message::SetMsgType(tMessageType type)
{
}

tMessage Message::toString()
{
    return mMsgFactory->CreateMsg(mMsgBuilder);
}
