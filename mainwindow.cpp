#include "mainwindow.h"
#include <QMessageBox>
#include "setlevel.h" // Replace with actual header file name
#include "gifdisplay.h"
#include "about.h"
#include <QStandardPaths>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), bg(new QLabel("", this)), logo(new QLabel("", this)), start(nullptr),  demo(nullptr),
        about(nullptr),  exit(nullptr)
{
    setPalette(QPalette(QPalette::Window, QColor(QString("#d6d9df"))));
    setFixedSize(700, 600-40);

    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString bestTimeDirPath = homeDir + "/best_time";

    // Ensure the directory exists
    QDir dir;
    if (!dir.exists(bestTimeDirPath)) {
        if (!dir.mkpath(bestTimeDirPath)) {
            qDebug() << "Failed to create directory:" << bestTimeDirPath;
        }
    }

    QString filePath = bestTimeDirPath + "/best_time.txt";
    QFile file(filePath);

    // Check if the file exists; if not, create it with default values
    if (!file.exists()) {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing:" << file.errorString();
        }

        QTextStream out(&file);
        out << "easy:30\n";
        out << "medium:60\n";
        out << "hard:90\n";
        out << "so hard:120\n";
        file.close();
    }
    setCentralWidget(bg);


    QPixmap bg_img(":/resources/images/bg4.jpg");
    if (bg_img.isNull()) {
        qDebug() << "Failed to load image";
    } else {
        bg->setPixmap(bg_img.scaledToWidth(400, Qt::SmoothTransformation));
    }


    QPixmap logo_img(":/resources/images/icon.png");
    if (logo_img.isNull()) {
        qDebug() << "Failed to load image";
    } else {
        logo->setPixmap(logo_img.scaledToWidth(240, Qt::SmoothTransformation));
        logo->setGeometry(3 * width() / 5 , 50, 240, 230);
    }


    start = new QPushButton(this);
    start->setIcon(QIcon(":/resources/images/start2.png"));
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
    demo->setIcon(QIcon(":/resources/images/demo.png"));
    demo->setStyleSheet("border: none; background: transparent;");
    demo->setIconSize(QSize(280, 200));
    demo->setFixedSize(120, 60);
    demo->setCursor(Qt::PointingHandCursor);
    demo->move(width() - 220, 320 - 40 + 60);
    connect(demo, &QPushButton::clicked, this, [=]() { // Replace with actual path

        GifDisplay *gifWindow = new GifDisplay(":/resources/images/demo4.gif", nullptr);
        gifWindow->setAttribute(Qt::WA_DeleteOnClose); // Delete on close to avoid memory leak

        gifWindow->exec(); // Display the window as modal
    });

    about = new QPushButton(this);
    about->setIcon(QIcon(":/resources/images/about.png"));
    about->setStyleSheet("border: none; background: transparent;");
    about->setIconSize(QSize(280, 200));
    about->setFixedSize(120, 60);
    about->setCursor(Qt::PointingHandCursor);
    about->move(width() - 220, 320 - 40 + 60 * 2);
    connect(about, &QPushButton::clicked, this, [=]() {
        (new About(nullptr))->show();
    });

    exit = new QPushButton(this);
    exit->setIcon(QIcon(":/resources/images/exit.png"));
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
