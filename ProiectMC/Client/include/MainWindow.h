#pragma once

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
//#include "../include/Bomb.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onDetonateBomb(); // Slot for the detonation button

private:
    Ui::MainWindowClass* ui;                // Pointer to the UI class
    QGraphicsScene* scene;             // Scene for rendering
    QGraphicsEllipseItem* bombItem;    // Graphical representation of the bomb
   // Bomb* bomb;                        // Bomb object
};
