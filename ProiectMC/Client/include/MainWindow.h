#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include "../include/Bomb.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onDetonateBomb(); // Slot pentru butonul de detonare

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;  // Scena pentru desenare
    QGraphicsEllipseItem* bombItem;  // Reprezentarea grafica a bombei
    Bomb* bomb;  // Obiectul bomba
};
