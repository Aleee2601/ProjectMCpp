#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "GameSession.h"
#include "map.h"

class Server : public QTcpServer {
    Q_OBJECT

public:
    explicit Server(QObject* parent = nullptr);
    void startServer(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    void handleMoveCommand(int playerId, const QString& direction);
    void broadcastPlayerPosition(int playerId);

    QList<QTcpSocket*> clients; // Lista de clienți conectați
    GameSession gameSession;    // Obiect pentru gestionarea sesiunii de joc
    Map gameMap;                // Obiect pentru harta jocului
};
