#include "mainwindow.h"
#include <QMessageBox>
#include "setlevel.h" // Replace with actual header file name
#include "gifdisplay.h"
#include "about.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), bg(new QLabel("", this)), logo(new QLabel("", this)), start(nullptr),  demo(nullptr),
        about(nullptr),  exit(nullptr)
{
    setPalette(QPalette(QPalette::Window, QColor(QString("#d6d9df"))));
    setFixedSize(700, 600-40);


    setCentralWidget(bg);

    QString imgsPath = "../../resources/images/";

    QPixmap bg_img(imgsPath + "bg4.jpg");
    if (bg_img.isNull()) {
        qDebug() << "Failed to load image";
    } else {
        bg->setPixmap(bg_img.scaledToWidth(400, Qt::SmoothTransformation));
    }


    QPixmap logo_img(imgsPath + "icon.png");
    if (logo_img.isNull()) {
        qDebug() << "Failed to load image";
    } else {
        logo->setPixmap(logo_img.scaledToWidth(240, Qt::SmoothTransformation));
        logo->setGeometry(3 * width() / 5 , 50, 240, 230);
    }


    start = new QPushButton(this);
    start->setIcon(QIcon(imgsPath + "start2.png"));
    start->setStyleSheet("border: none; background: transparent;");
    start->setIconSize(QSize(280, 200));
    start->setFixedSize(120, 60);
    start->setCursor(Qt::PointingHandCursor);
    start->move(width() - 220, 320 - 40);
    connect(start, &QPushButton::clicked, this, [=]() {
        (new SetLevel(nullptr))->show();
        close();
    });

    demo = new QPushButton(this);
    demo->setIcon(QIcon(imgsPath + "demo.png"));
    demo->setStyleSheet("border: none; background: transparent;");
    demo->setIconSize(QSize(280, 200));
    demo->setFixedSize(120, 60);
    demo->setCursor(Qt::PointingHandCursor);
    demo->move(width() - 220, 320 - 40 + 60);
    connect(demo, &QPushButton::clicked, this, [=]() { // Replace with actual path

        GifDisplay *gifWindow = new GifDisplay(imgsPath + "demo4.gif", nullptr);
        gifWindow->setAttribute(Qt::WA_DeleteOnClose); // Delete on close to avoid memory leak

        gifWindow->exec(); // Display the window as modal
    });

    about = new QPushButton(this);
    about->setIcon(QIcon(imgsPath + "about.png"));
    about->setStyleSheet("border: none; background: transparent;");
    about->setIconSize(QSize(280, 200));
    about->setFixedSize(120, 60);
    about->setCursor(Qt::PointingHandCursor);
    about->move(width() - 220, 320 - 40 + 60 * 2);
    connect(about, &QPushButton::clicked, this, [=]() {
        (new About(nullptr))->show();
    });

    exit = new QPushButton(this);
    exit->setIcon(QIcon(imgsPath + "exit.png"));
    exit->setStyleSheet("border: none; background: transparent;");
    exit->setIconSize(QSize(280, 200));
    exit->setFixedSize(120, 60);
    exit->setCursor(Qt::PointingHandCursor);
    exit->move(width() - 220, 320 - 40 + 60 * 3);
    connect(exit, &QPushButton::clicked, this, [=]() {
        close();
    });


}

MainWindow::~MainWindow() {}
