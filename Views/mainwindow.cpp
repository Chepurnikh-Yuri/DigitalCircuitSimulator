#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene = new Scene();
    m_view = new QGraphicsView(m_scene);
    m_view->setScene(m_scene);

    setCentralWidget(m_view);
}

MainWindow::~MainWindow()
{
    delete ui;
}
