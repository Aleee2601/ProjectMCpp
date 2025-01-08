#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include <QKeyEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include "ui_MainWindow.h"
#include "../../Server/include/GameSession.h"
#include "../../Server/include/Bomb.h"
#include "../../Server/include/Bullet.h"
#include <QTimer>
#include "../../Server/include/GameSession.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; }
QT_END_NAMESPACE

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
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError);
    void updatePlayerScore(int score);
    std::string directionToString(Direction direction);
    void shoot();
    void updateBullets();

private:
    QTcpSocket* socket; // Socket pentru conexiunea cu serverul
    Ui::MainWindowClass* ui;
    Map gameMap;
    GameSession gameSession;
     int currentPlayerScore;  
    std::vector<Bullet> activeBullets; // Gloanțe active
    QTimer* bulletTimer;
};
