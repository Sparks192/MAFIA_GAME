#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QCommandLineParser parser;
    QCommandLineOption server_option("s");
    QCommandLineOption client_option("c");
    parser.setApplicationDescription("MAFIA GAME");;
    parser.addOption(server_option);
    parser.addOption(client_option);
    QStringList arg = a.arguments();
    parser.process(arg);
    if(parser.isSet(server_option))
    {
        qDebug()<<"server option active"<<endl;
    }
    return a.exec();
}
