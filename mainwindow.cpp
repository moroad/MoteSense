//! Implementation of the MainWindow class
/*! \file mainwindow.cpp
 *  \author Jharrod LaFon
 *  \date Spring 2010
 */
#include "mainwindow.h"
#include "graphwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Instantiate packetHandler
    server = new packetHandler();

    // Connect signal
    connect(server, SIGNAL(nodesChanged()), this, SLOT(nodeChange()));

}


MainWindow::~MainWindow()
{
    delete ui;
    delete server;

}

void MainWindow::nodeChange()
{
    // Get new list of nodes
    nodeList = server->getNodeList();

    // Update user list
    for(int i = 0; i < nodeList.count(); i++)
    {
        ui->nodeList->addItem(QString("%1").arg(nodeList[i]));
    }
}

// Qt stuff
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::on_plotButton_clicked()
{

    if(ui->nodeList->selectedItems().count() < 1)
        return;
    //int id = ui->nodeList->selectedItems().at(0)->text().toInt();

    // Create new graphWindow
    GraphWindow * graphWindow = new GraphWindow();
    graphWindow->setSource(this->server);
    graphWindow->resize(800,600);
    graphWindow->show();

}
