#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QHostAddress>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass) {
    ui->setupUi(this);

    // Creeaz? socket-ul în thread-ul principal
    socket = new QTcpSocket(this);

    // Conecteaz? semnalele ?i sloturile
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::onError);

    // Ini?iaz? conexiunea la server
    qDebug() << "Connecting to server...";
    socket->connectToHost(QHostAddress("127.0.0.1"), 12345); // Adresa ?i portul serverului

    if (!socket->waitForConnected(5000)) {  // Timeout de 5 secunde
        qDebug() << "Connection failed:" << socket->errorString();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

// Sloturi pentru gestionarea socket-ului
void MainWindow::onConnected() {
    qDebug() << "Connected to server!";
    qDebug() << "Server IP:" << socket->peerAddress().toString();  // Afi?eaz? IP-ul serverului
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


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow window; 
    window.show();

    return app.exec();
}
