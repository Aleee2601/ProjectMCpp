#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include "GameSession.h"
#include "map.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onConnected();       // Slot pentru semnalul connected
    void onDisconnected();    // Slot pentru semnalul disconnected
    void onReadyRead();       // Slot pentru semnalul readyRead
    void onError(QAbstractSocket::SocketError socketError);
    void detonateBomb();          // Detoneaza bomba
    void showActivePlayers();     // Afiseaza jucatorii activi
    void resetGame();             // Reseteaza jocul
    void visualizeExplosion(); // Afi?eaz? explozia pe hart?

private:
    Ui::MainWindowClass* ui;
    QTcpSocket* socket;           // Client socket
    GameSession gameSession;      // Obiect pentru gestionarea sesiunii de joc
    Map gameMap;                  // Harta jocului
};
