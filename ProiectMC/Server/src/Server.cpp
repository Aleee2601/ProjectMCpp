#include "Server.h"
#include <QDebug>

Server::Server(QObject* parent)
    : QTcpServer(parent) {
    // Constructor gol sau adaugă inițializări necesare
}


void Server::startServer(quint16 port) {
    if (listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port:" << port;
    }
    else {
        qDebug() << "Error starting server:" << errorString();
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* clientSocket = new QTcpSocket();
    clientSocket->setSocketDescriptor(socketDescriptor);
    qDebug() << "New client connected!";
    clientSocket->write("Hello from the server!");
    clientSocket->flush();
    clientSocket->disconnectFromHost();
}



// Include generated moc file
//#include "moc_Server.cpp"
