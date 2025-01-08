#include "../../Server/include/Server.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

Server::Server(QObject* parent)
    : QTcpServer(parent), gameSession(), gameMap(10, 10) { // Initializează GameSession și Map
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

    // Adaugă clientul în lista de clienți activi
    clients.append(clientSocket);

    // Gestionează mesajele primite de la client
    connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket]() {
        QByteArray data = clientSocket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        if (obj["type"] == "move") {
            int playerId = obj["playerId"].toInt();
            QString direction = obj["direction"].toString();
            handleMoveCommand(playerId, direction);
        }
        });

    // Elimină clientul la deconectare
    connect(clientSocket, &QTcpSocket::disconnected, this, [this, clientSocket]() {
        clients.removeAll(clientSocket);
        clientSocket->deleteLater();
        });

    clientSocket->write("Welcome to the server!");
    clientSocket->flush();
}

void Server::handleMoveCommand(int playerId, const QString& direction) {
    try {
        Player& player = gameSession.GetPlayerById(playerId);

        if (direction == "UP") {
            player.Move(Direction::UP, gameMap);
        }
        else if (direction == "DOWN") {
            player.Move(Direction::DOWN, gameMap);
        }
        else if (direction == "LEFT") {
            player.Move(Direction::LEFT, gameMap);
        }
        else if (direction == "RIGHT") {
            player.Move(Direction::RIGHT, gameMap);
        }

        qDebug() << "Player" << playerId << "moved to position (" << player.GetX() << "," << player.GetY() << ")";
        broadcastPlayerPosition(playerId); // Notifică toți clienții despre poziția actualizată
    }
    catch (std::exception& e) {
        qDebug() << "Error handling move command:" << e.what();
    }
}

void Server::broadcastPlayerPosition(int playerId) {
    try {
        const Player& player = gameSession.GetPlayerById(playerId);
        QJsonObject updateMessage;
        updateMessage["type"] = "update_position";
        updateMessage["playerId"] = playerId;
        updateMessage["x"] = player.GetX();
        updateMessage["y"] = player.GetY();

        QJsonDocument doc(updateMessage);
        QByteArray message = doc.toJson();

        for (QTcpSocket* client : clients) {
            client->write(message);
            client->flush();
        }
    }
    catch (std::exception& e) {
        qDebug() << "Error broadcasting position:" << e.what();
    }
}
void Server::sendScoreUpdate(QTcpSocket* clientSocket, int score) {
    QJsonObject message;
    message["type"] = "score_update";
    message["score"] = score;

    QJsonDocument doc(message);
    clientSocket->write(doc.toJson());
    clientSocket->flush();
}

