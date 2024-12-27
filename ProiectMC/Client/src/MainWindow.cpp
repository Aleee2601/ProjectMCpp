#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QHostAddress>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass), gameMap(10, 10) {
    ui->setupUi(this);

    // Initializare socket
    socket = new QTcpSocket(this);

    // Initializare butoane
    connect(ui->PushButton, &QPushButton::clicked, this, &MainWindow::detonateBomb);
    connect(ui->showActivePlayersButton, &QPushButton::clicked, this, &MainWindow::showActivePlayers);
    connect(ui->resetGameButton, &QPushButton::clicked, this, &MainWindow::resetGame);
    connect(ui->visualizeExplosionButton, &QPushButton::clicked, this, &MainWindow::visualizeExplosion);

    // Conectare socket la semnale si sloturi
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::onError);

    // Initiaz? conexiunea la server
    qDebug() << "Connecting to server...";
    socket->connectToHost(QHostAddress("127.0.0.1"), 12345); // Server IP ?i port

    if (!socket->waitForConnected(5000)) {
        qDebug() << "Connection failed:" << socket->errorString();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

// Slot pentru detonarea bombei
void MainWindow::detonateBomb() {
    Bomb bomb(4, 4);  // Pozi?ia bombei
    bomb.Detonate(gameMap);

    QString message = "Bomb detonated at position (4,4)\n";
    ui->statusTextEdit->append(message);
    qDebug() << message;
}

// Slot pentru afi?area jucatorilor activi
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
    gameMap = Map(10, 10); // Resetare harta

    QString message = "Game has been reset.\n";
    ui->statusTextEdit->setText(message);
    qDebug() << message;
}

// Sloturi pentru gestionarea conexiunii cu serverul
void MainWindow::onConnected() {
    qDebug() << "Connected to server!";
    qDebug() << "Server IP:" << socket->peerAddress().toString();
}

void MainWindow::onDisconnected() {
    qDebug() << "Disconnected from server.";
}

void MainWindow::onReadyRead() {
    QByteArray data = socket->readAll();
    qDebug() << "Data received from server:" << data;
}

void MainWindow::onError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socketError << socket->errorString();
}

void MainWindow::visualizeExplosion() {
    // Poziția bombei pentru explozie (exemplu: poziția fixă)
    Bomb bomb(4, 4);

    // Efectele exploziei
    bomb.CalculateExplosionEffects(gameMap, gameSession.GetAllPlayers());

    // Afișarea efectelor exploziei
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newX = 4 + i;
            int newY = 4 + j;

            if (gameMap.IsWithinBounds(newX, newY)) {
                // Simulăm explozia în QGraphicsView
                QGraphicsRectItem* rect = new QGraphicsRectItem(newX * 20, newY * 20, 20, 20);
                rect->setBrush(Qt::red); // Explozie evidențiată în roșu
                ui->graphicsView->scene()->addItem(rect);
            }
        }
    }

    qDebug() << "Explosion visualized on the map.";
}
