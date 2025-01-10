#include "../../Client/include/MainWindow.h"
#include "ui_MainWindow.h"
#include <QHostAddress>
#include <QDebug>
#include <QKeyEvent>
#include <QCheckBox>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include "../../Server/include/Player.h"
#include "../../Server/include/Bullet.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass), currentPlayerScore(0) {
    ui->setupUi(this);

    // Initializare socket
    socket = new QTcpSocket(this);

    // Initializare butoane
    connect(ui->PushButton, &QPushButton::clicked, this, &MainWindow::detonateBomb);
    connect(ui->showActivePlayersButton, &QPushButton::clicked, this, &MainWindow::showActivePlayers);
    connect(ui->resetGameButton, &QPushButton::clicked, this, &MainWindow::resetGame);
    connect(ui->visualizeExplosionButton, &QPushButton::clicked, this, &MainWindow::visualizeExplosion);
    connect(ui->readyCheckBox, &QCheckBox::stateChanged, this, [this](int state) {
        QJsonObject obj;
        obj["type"] = "readyStatus";
        obj["ready"] = (state == Qt::Checked);
        socket->write(QJsonDocument(obj).toJson());
        });

    // Inițializare timer pentru mișcarea gloanțelor
    bulletTimer = new QTimer(this);
    connect(bulletTimer, &QTimer::timeout, this, &MainWindow::updateBullets);
    bulletTimer->start(50); // Actualizează gloanțele la fiecare 50ms

    // Conectare socket la semnale și sloturi
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::onError);

    // Inițializare conexiune la server
    qDebug() << "Connecting to server...";
    socket->connectToHost(QHostAddress("127.0.0.1"), 12345);

    if (!socket->waitForConnected(5000)) {
        qDebug() << "Connection failed:" << socket->errorString();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

// Slot pentru detonarea bombei
void MainWindow::detonateBomb() {
    Bomb bomb(4, 4);
    bomb.Detonate(gameMap);

    QString message = "Bomb detonated at position (4,4)\n";
    ui->statusTextEdit->append(message);
    qDebug() << message;
}

// Slot pentru afișarea jucătorilor activi
void MainWindow::showActivePlayers() {
    QString message = "Active Players:\n";
    for (const auto& player : gameSession.GetAllPlayers()) {
        message += QString("ID: %1, Name: %2\n").arg(player.GetId()).arg(player.GetName().c_str());
    }
    ui->statusTextEdit->setText(message);
    qDebug() << message;
}

// Slot pentru resetarea jocului
void MainWindow::resetGame() {
    gameSession.ResetSession();
    gameMap = Map(18, 9);

    QString message = "Game has been reset.\n";
    ui->statusTextEdit->setText(message);
    qDebug() << message;
}

// Slot apelat când conexiunea cu serverul este stabilită
void MainWindow::onConnected() {
    qDebug() << "Connected to server!";
    qDebug() << "Server IP:" << socket->peerAddress().toString();
}

// Slot apelat când conexiunea cu serverul este întreruptă
void MainWindow::onDisconnected() {
    qDebug() << "Disconnected from server.";
}

// Slot pentru procesarea mesajelor primite de la server
void MainWindow::onReadyRead() {
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if (obj.contains("type") && obj["type"] == "score_update") {
        int newScore = obj["score"].toInt();
        updatePlayerScore(newScore);
    }
}

// Slot pentru gestionarea erorilor de conexiune
void MainWindow::onError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socketError << socket->errorString();
}

// Slot pentru vizualizarea exploziei
void MainWindow::visualizeExplosion() {
    Bomb bomb(4, 4);
    bomb.CalculateExplosionEffects(gameMap, gameSession.GetAllPlayers());

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newX = 4 + i;
            int newY = 4 + j;

            if (gameMap.IsWithinBounds(newX, newY)) {
                QGraphicsRectItem* rect = new QGraphicsRectItem(newX * 20, newY * 20, 20, 20);
                rect->setBrush(Qt::red);
                ui->graphicsView->scene()->addItem(rect);
            }
        }
    }

    qDebug() << "Explosion visualized on the map.";
}

// Gestionarea evenimentelor de tastatură pentru mișcarea jucătorului
void MainWindow::keyPressEvent(QKeyEvent* event) {
    int playerId = 1;
    Player& player = gameSession.GetPlayerById(playerId);

    switch (event->key()) {
    case Qt::Key_Up:
        player.Move(Direction::UP, gameMap); // Utilizare complet calificată
        break;
    case Qt::Key_Down:
        player.Move(Direction::DOWN, gameMap);
        break;
    case Qt::Key_Left:
        player.Move(Direction::LEFT, gameMap);
        break;
    case Qt::Key_Right:
        player.Move(Direction::RIGHT, gameMap);
        break;
    case Qt::Key_Space:
        shoot();
        break;
    default:
        QMainWindow::keyPressEvent(event);
        return;
    }

    QString message = QString("Player %1 moved to (%2, %3)")
        .arg(player.GetId())
        .arg(player.GetX())
        .arg(player.GetY());
    ui->statusTextEdit->append(message);

    qDebug() << message;
}


// Convertirea direcției într-un șir de caractere pentru trimiterea comenzilor
std::string MainWindow::directionToString(Direction direction) {
    switch (direction) {
    case Direction::UP:
        return "UP";
    case Direction::DOWN:
        return "DOWN";
    case Direction::LEFT:
        return "LEFT";
    case Direction::RIGHT:
        return "RIGHT";
    default:
        return "UNKNOWN";
    }
}

// Slot pentru trimiterea unei comenzi de mișcare către server
void MainWindow::sendMoveCommand(Direction direction) {
    QJsonObject command;
    command["type"] = "move";
    command["direction"] = QString::fromStdString(directionToString(direction));
    socket->write(QJsonDocument(command).toJson());
}

// Slot pentru actualizarea scorului jucătorului
void MainWindow::updatePlayerScore(int score) {
    currentPlayerScore = score;
    ui->playerScoreLabel->setText(QString("Score: %1").arg(currentPlayerScore));
}

// Slot pentru gestionarea acțiunii de a trage
void MainWindow::shoot() {
    int playerId = 1;
    Player& player = gameSession.GetPlayerById(playerId);

    Bullet bullet(player.GetX(), player.GetY(), player.GetDirection());
    activeBullets.push_back(bullet);

    QString message = QString("Player %1 fired a bullet from (%2, %3)")
        .arg(player.GetId())
        .arg(player.GetX())
        .arg(player.GetY());
    ui->statusTextEdit->append(message);
    qDebug() << message;
}

// Slot pentru actualizarea poziției gloanțelor active
void MainWindow::updateBullets() {
    for (auto it = activeBullets.begin(); it != activeBullets.end(); ) {
        it->Move(gameMap);

        if (!gameMap.IsWithinBounds(it->GetX(), it->GetY()) ||
            gameMap.GetCellType(it->GetX(), it->GetY()) == CellType::EMPTY) {
            it = activeBullets.erase(it);
        }
        else {
            ++it;
        }
    }
}
