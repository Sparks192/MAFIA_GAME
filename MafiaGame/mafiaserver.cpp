#include "mafiaserver.h"
#include <QNetworkConfigurationManager>
#include <QSettings>
#include <QNetworkSession>
#include <QMessageBox>
#include <QTcpSocket>
#include <QApplication>
#include <QJsonObject>

MafiaServer::MafiaServer()
    : mServerSession(NULL),
      mServer(NULL),
      mServerStatus(false)
{
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired)
    {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        mServerSession = new QNetworkSession(config, this);
        QObject::connect(mServerSession, SIGNAL(opened()), this, SLOT(OnSessionOpened()));
        mServerSession->open();
    }else if (mServerSession)
    {
        // Save the used configuration
        QNetworkConfiguration config = manager.defaultConfiguration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice) {
            id = mServerSession->sessionProperty(
                   QLatin1String("UserChoiceConfiguration")).toString();
        } else {
            id = config.identifier();
        }
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }
}

MafiaServer::~MafiaServer()
{
    mServerStatus = false;
    if(mServer)
    {
        delete mServer;
    }
    if(mServerSession)
    {
        delete mServerSession;
    }
}

QString MafiaServer::GetHost()
{
    return mIpAddress;
}

quint16 MafiaServer::GetPort()
{
    return mPort;
}

void MafiaServer::StartServer()
{
    mServer = new QTcpServer(this);
    connect(mServer, SIGNAL(newConnection()), this, SLOT(OnClientConnect()));
    connect(mServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(OnDisplayError(QAbstractSocket::SocketError)));
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    if(!mServer->listen() && !mServerStatus)
    {
        QString errMsg = QString("Server\nUnable to start the server %1.").arg(mServer->errorString());
        qDebug()<<errMsg<<endl;
        mServer->close();
        return;
    }else
    {
        mIpAddress = ipAddress;
        mPort = mServer->serverPort();
        mServerStatus= true;
        if(mServer->isListening())
        {
            qDebug()<<"Listening..."<<endl;
        }
        QString statusMsg = QString("The server is running on IP: %1 port: %2"\
                                   "\nRun the MAFIA_GAME Client  now.")
                                .arg(ipAddress).arg(mServer->serverPort());
        qDebug()<<statusMsg.simplified()<<endl;

        if(mServer->hasPendingConnections())
        {
            qDebug()<<"Pending connection wait"<<endl;

        }
    }
}

void MafiaServer::StopServer()
{
    if(mServerStatus == true)
    {
        foreach (qintptr clientId, mClientsTcpSocketContainer.keys())
        {
            mClientsTcpSocketContainer[clientId]->close();
            mClientsTcpSocketContainer.remove(clientId);
        }
        mServer->close();
        mServerStatus =false;
        delete mServer;
        qDebug()<<"Server stoped"<<endl;
    }else
    {
        qDebug()<<"Server already stoped"<<endl;
    }
}

void MafiaServer::OnClientConnect()
{
    qDebug()<<"Client connected"<<endl;
    QTcpSocket* clientTcpSocket = mServer->nextPendingConnection();
    qintptr clientId = clientTcpSocket->socketDescriptor();
    mClientsTcpSocketContainer[clientId] = clientTcpSocket;
    connect(mClientsTcpSocketContainer[clientId], SIGNAL(readyRead()), this, SLOT(OnMsgReceive()));
    connect(mClientsTcpSocketContainer[clientId], SIGNAL(disconnected()),
            mClientsTcpSocketContainer[clientId], SLOT(deleteLater()));
}

void MafiaServer::OnSessionOpened()
{
    qDebug()<<"Server session opened"<<endl;
}

void MafiaServer::OnDisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(NULL, tr("Server"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(NULL, tr("Server"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(NULL, tr("Server"),
                                 tr("The following error occurred: %1.")
                                 .arg(mServer->errorString()));
    }
}

void MafiaServer::OnMsgReceive()
{
    qDebug()<<"Server: received msg:"<<endl;
    QTcpSocket* clientTcpSocket = (QTcpSocket*)sender();
    QDataStream in(clientTcpSocket);
    QByteArray block;
    clientTcpSocket->seek(0);
    block = clientTcpSocket->readAll();
    qDebug()<<QString(block)<<endl;
}

void MafiaServer::OnSendMsg()
{
    QJsonObject msg;
    QByteArray block;
    QString dataToSend("hello");
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << dataToSend;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    foreach (qintptr clientId, mClientsTcpSocketContainer.keys())
    {
        mClientsTcpSocketContainer[clientId]->write(block);
        mClientsTcpSocketContainer.remove(clientId);
    }
}
