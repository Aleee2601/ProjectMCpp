#pragma once

#include <QMainWindow>
#include <QTcpSocket>

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

private:
    Ui::MainWindowClass* ui;
    QTcpSocket* socket;   // Client socket
};
