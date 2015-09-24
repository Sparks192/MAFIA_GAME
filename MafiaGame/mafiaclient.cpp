#include "mafiaclient.h"
#include <QSettings>

#include <QTcpSocket>
#include <QDataStream>
#include <QMessageBox>
#include <QApplication>

MafiaClient::MafiaClient(QString host, quint16 port)
    : mDestIp(host),
      mDestPort(port),
      mClientSession(NULL),
      mClientTcpSocket(NULL)
{
    mNetConfig = mNetworkConfigMngr.defaultConfiguration();
}

MafiaClient::~MafiaClient()
{
    if(mClientSession != NULL)
    {
        delete mClientSession;
    }
}

void MafiaClient::StartClient()
{
    if(mClientSession != NULL)
    {
        delete mClientSession;
    }
    mClientSession = new QNetworkSession(mNetConfig, this);
    connect(mClientSession, SIGNAL(opened()), this, SLOT(OnOpenSession()));

    if(mClientTcpSocket != NULL)
    {
       qDebug()<<"Closing old connection..."<<endl;
       mClientTcpSocket->close();
       delete mClientTcpSocket;
    }

    mClientTcpSocket = new QTcpSocket(this);

    connect(mClientTcpSocket, SIGNAL(connected()), this, SLOT(OnConnect()));
    connect(mClientTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnDisplayError(QAbstractSocket::SocketError)));
    connect(mClientTcpSocket, SIGNAL(readyRead()), this, SLOT(OnMsgReceive()));
    qDebug()<<"Client connectiong:"<<endl;
    qDebug()<<"host:"<<mDestIp<<endl;
    qDebug()<<"port:"<<mDestPort<<endl;
    mClientSession->open();
}

void MafiaClient::OnOpenSession()
{
  qDebug()<<"Client session opened"<<endl;
  mClientTcpSocket->connectToHost(QHostAddress::LocalHost, mDestPort);
  if(!mClientTcpSocket->waitForConnected(1000))
  {
      qDebug()<<"error connecting"<<endl;
  }
}

void MafiaClient::OnMsgReceive()
{
    QDataStream in(mClientTcpSocket);
    QByteArray block;
    mClientTcpSocket->seek(0);
    block = mClientTcpSocket->readAll();
    qDebug()<<"received msg = "<<QString(block)<<endl;
}

void MafiaClient::OnMsgSend()
{
    qDebug()<<"Client send "<<endl;
    QByteArray block;
    QString dataToSend("hello");
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << dataToSend;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    mClientTcpSocket->write(block);
}

void MafiaClient::OnDisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(NULL, tr("Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(NULL, tr("Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(NULL, tr("Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(mClientTcpSocket->errorString()));
    }
}

void MafiaClient::OnConnect()
{
    qDebug()<<"Connected success"<<endl;
    OnMsgSend();
}
