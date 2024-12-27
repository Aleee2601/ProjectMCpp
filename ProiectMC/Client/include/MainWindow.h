#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include <QKeyEvent>
#include <QJsonObject>
#include <QJsonDocument>

enum class Direction { UP, DOWN, LEFT, RIGHT };

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override; // Suprascrierea evenimentului de tastatură

private slots:
    void sendMoveCommand(Direction direction);
    std::string directionToString(Direction direction);

private:
    QTcpSocket* socket; // Socket pentru conexiunea cu serverul
};
