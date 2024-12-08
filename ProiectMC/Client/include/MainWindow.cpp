#include "../include/MainWindow.h"
#include "../include/ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Initialize the scene for the game
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // Create the bomb object (position x=100, y=100)
    bomb = new Bomb(100, 100);

    // Add the bomb to the map (draw a circle at the bomb's position)
    bombItem = scene->addEllipse(bomb->getX(), bomb->getY(), 20, 20, QPen(Qt::black), QBrush(Qt::red));

    // Connect the "Detonate Bomb" button to the onDetonateBomb function
    connect(ui->detonateButton, &QPushButton::clicked, this, &MainWindow::onDetonateBomb);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Slot for bomb detonation
void MainWindow::onDetonateBomb() {
    // Simulate the bomb detonation
    qDebug() << "The bomb has detonated at (" << bomb->getX() << ", " << bomb->getY() << ")!";

    // Change the bomb's color to indicate detonation
    bombItem->setBrush(QBrush(Qt::gray));

    // Display a message in the status bar
    ui->statusbar->showMessage("The bomb has exploded!");
}
