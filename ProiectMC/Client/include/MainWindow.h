#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include <QKeyEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include "ui_MainWindow.h"
#include "GameSession.h"
#include "Bomb.h"
enum class Direction { UP, DOWN, LEFT, RIGHT };

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override; // Suprascrierea evenimentului de tastatură
    void detonateBomb();
    void showActivePlayers();
    void resetGame();
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);
    void visualizeExplosion();
private slots:
    void sendMoveCommand(Direction direction);
    std::string directionToString(Direction direction);

private:
    QTcpSocket* socket; // Socket pentru conexiunea cu serverul
    Ui::MainWindowClass* ui;
    Map gameMap;
    GameSession gameSession;
};
