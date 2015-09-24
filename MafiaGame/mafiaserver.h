#ifndef MAFIASERVER_H
#define MAFIASERVER_H

#include <QNetworkSession>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class MafiaServer : QObject
{
    Q_OBJECT
public:
    MafiaServer();
    ~MafiaServer();

    QString GetHost();
    quint16 GetPort();

public slots:
    void StartServer();
    void StopServer();
    void OnClientConnect();
    void OnSessionOpened();
    void OnDisplayError(QAbstractSocket::SocketError socketError);
    void OnMsgReceive();
    void OnSendMsg();

private:
    QTcpServer*      mServer;
    QNetworkSession* mServerSession;
    QString          mIpAddress;
    quint16          mPort;
    bool             mServerStatus;
    QMap<qintptr, QTcpSocket*>      mClientsTcpSocketContainer;
};

#endif // MAFIASERVER_H
