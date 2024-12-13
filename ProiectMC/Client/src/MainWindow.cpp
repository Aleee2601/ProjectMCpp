#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass) { 
    ui->setupUi(this);

    // Initialize the scene for the game
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onDetonateBomb() {
    
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv); // Initialize the Qt application

    MainWindow window;            // Create the main window
    window.show();                // Display the main window

    return app.exec();            // Start the event loop
}