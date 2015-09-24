#ifndef MAFIACLIENT_H
#define MAFIACLIENT_H

#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QTcpSocket>
#include <QObject>

class MafiaClient : public QObject
{
    Q_OBJECT
public:
    MafiaClient(QString, quint16);
    ~MafiaClient();

public slots:
    void StartClient();

private slots:
    void OnOpenSession();
    void OnMsgReceive();
    void OnMsgSend();
    void OnDisplayError(QAbstractSocket::SocketError socketError);
    void OnConnect();

private:
    QTcpSocket*      mClientTcpSocket;
    QNetworkSession* mClientSession;
    QString          mDestIp;
    quint16          mDestPort;
    QNetworkConfigurationManager mNetworkConfigMngr;
    QNetworkConfiguration        mNetConfig;
};

#endif // MAFIACLIENT_H
