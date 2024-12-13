#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QThread> 

class ClientHandler : public QThread {
    Q_OBJECT
public:
    explicit ClientHandler(QTcpSocket* clientSocket, QObject* parent = nullptr);
    void run() override;

private:
    QTcpSocket* socket;
};

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr);
    void startServer(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};
