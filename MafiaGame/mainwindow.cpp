#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mafiaserver.h"
#include "mafiaclient.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MafiaServer server;
    server.StartServer();
    MafiaClient client(server.GetHost(), server.GetPort());
    //QThread worker(&client);
    //connect(&worker, SIGNAL(finished()),&worker, SLOT(deleteLater()));
    client.StartClient();
    //worker.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
