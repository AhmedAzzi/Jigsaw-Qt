#include "setlevel.h"
#include "mainwindow.h"
#include "puzzlepanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>



SetLevel::SetLevel(QWidget *parent)
    : QWidget(parent), intro(nullptr), easy(nullptr), medium(nullptr),
    hard(nullptr), so_hard(nullptr), play(nullptr), back(nullptr),
    imageLabel(nullptr), prevButton(nullptr), nextButton(nullptr),
    imagePanel(nullptr), dim(3), second(30), currentImage(0)
{

    QString imgsPath ="../../resources/images/";
    setFixedSize(QGuiApplication::primaryScreen()->geometry().width(), QGuiApplication::primaryScreen()->geometry().height());
    setPalette(QPalette(QPalette::Window, QColor(QString("#d3c3b6"))));

    intro = new QLabel("Choose another Image Here", this);
    intro->setStyleSheet("font-size: 30px; font-family: Z003; color: black;");
    intro->setGeometry(width() / 2 - 200, 20, 400, 50); // Absolute positioning
    intro->setAlignment(Qt::AlignCenter);

    QPushButton *fileChooserButton = new QPushButton(this);
    fileChooserButton->setIcon(QIcon(imgsPath + "choose.png"));
    fileChooserButton->setStyleSheet("border: none; background: transparent;");
    fileChooserButton->setIconSize(QSize(60, 60));
    fileChooserButton->setGeometry(width() / 2 + 150, 10, 100, 60); // Absolute positioning
    fileChooserButton->setCursor(Qt::PointingHandCursor);
    connect(fileChooserButton, &QPushButton::clicked, this, [=](){
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Image"), QDir::currentPath(), tr("Image Files (*.jpg *.jpeg *.png *.gif)"));

        if (!fileName.isEmpty()) {
            // Add the selected image file to the list
            if (QFileInfo(fileName).size() < 1000000) {
                imageFiles.append(fileName);
                currentImage = imageFiles.size() - 1;
                QPixmap image(fileName);
                imageLabel->setPixmap(image.scaled(680, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                QMessageBox::warning(this, "Image too Large", "Choose another one");
            }
        }
    });

    imageLabel = new QLabel(this);
    imageLabel->setGeometry(QGuiApplication::primaryScreen()->geometry().width()  / 2 - 340, 60, 680, 480); // Relative to imagePanel
    imageLabel->setAlignment(Qt::AlignCenter);

    prevButton = new QPushButton(this);
    prevButton->setIcon(QIcon(imgsPath + "prev.png"));
    prevButton->setStyleSheet("border: none; background: transparent;");
    prevButton->setIconSize(QSize(80, 100));
    prevButton->setFixedSize(100, 100);
    prevButton->setCursor(Qt::PointingHandCursor);
    prevButton->move(width() / 2 - 500, 280); // Absolute positioning
    connect(prevButton, &QPushButton::clicked, this, [=](){
        currentImage--;
        if (currentImage < 0)
            currentImage = imageFiles.size() - 1;

        QPixmap image(imageFiles[currentImage]);
        imageLabel->setPixmap(image.scaled(680, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    nextButton = new QPushButton(this);
    nextButton->setIcon(QIcon(imgsPath + "next.png"));
    nextButton->setStyleSheet("border: none; background: transparent;");
    nextButton->setIconSize(QSize(80, 100));
    nextButton->setFixedSize(100, 100);
    nextButton->setCursor(Qt::PointingHandCursor);
    nextButton->move(width() / 2 + 400, 280); // Absolute positioning
    connect(nextButton, &QPushButton::clicked, this, [=](){
        currentImage++;
        if (currentImage >= imageFiles.size())
            currentImage = 0;

        QPixmap image(imageFiles[currentImage]);
        imageLabel->setPixmap(image.scaled(680, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });

    setStyleSheet(
        "QRadioButton {"
        "    font-size: 20px; font-family: Z003; color: black;"
        "    padding-left: 40px;"
        "}"
        "QRadioButton::indicator {"
        "    width: 30px;"
        "    height: 30px;"
        "}"
        "QRadioButton::indicator::unchecked {"
        "    image: url(" + imgsPath + "r1_c.png);"
                     "}"
                     "QRadioButton::indicator::checked {"
                     "    image: url(" + imgsPath + "r1_o.png);"
                     "}"
        );

    easy = new QRadioButton("Easy\n3x3", this);
    easy->setChecked(true);
    easy->move(width() / 2 - 500, 550); // Absolute positioning
    connect(easy, &QPushButton::clicked, this, [=]() {
        dim = 3;
        second = 30;
    });


    medium = new QRadioButton("Medium\n4x4", this);
    medium->setStyleSheet("font-size: 20px; font-family: Z003; color:black");
    medium->move(width() / 2 - 200, 550); // Absolute positioning
    connect(medium, &QPushButton::clicked, this, [=]() {
        dim = 4;
        second = 60;
    });

    hard = new QRadioButton("Hard\n5x5", this);
    hard->setStyleSheet("font-size: 20px; font-family: Z003; color:black");
    hard->move(width() / 2 + 100, 550); // Absolute positioning
    connect(hard, &QPushButton::clicked, this, [=]() {
        dim = 5;
        second = 90;
    });

    so_hard = new QRadioButton("So Hard\n6x6", this);
    so_hard->setStyleSheet("font-size: 20px; font-family: Z003; color:black");
    so_hard->move(width() / 2 + 400, 550); // Absolute positioning
    connect(so_hard, &QPushButton::clicked, this, [=]() {
        dim = 6;
        second = 120;
    });


    play = new QPushButton(this);
    play->setIcon(QIcon(imgsPath + "play1.png"));
    play->setStyleSheet("border: none; background: transparent;");
    play->setIconSize(QSize(250, 180));
    play->setFixedSize(250, 60);
    play->setCursor(Qt::PointingHandCursor);
    play->move(3 * width() / 4 - 160, 620); // Absolute positioning
    connect(play, &QPushButton::clicked, this, [=]() {
        (new PuzzlePanel(dim, imageFiles[currentImage], second))->show();
        close();
    });

    back = new QPushButton(this);
    back->setIcon(QIcon(imgsPath + "back.png"));
    back->setStyleSheet("border: none; background: transparent;");
    back->setIconSize(QSize(250, 180));
    back->setFixedSize(250, 60);
    back->setCursor(Qt::PointingHandCursor);
    back->move(width() / 4 - 100, 620); // Absolute positioning
    connect(back, &QPushButton::clicked, this, [=]() {
        (new MainWindow(nullptr))->show();
        close();
    });

    imageFiles <<  imgsPath + "cow.jpg"
               <<  imgsPath + "dolphin.jpg"
               <<  imgsPath + "Nasa.jpg";


    QPixmap image(imageFiles[currentImage]);
    imageLabel->setPixmap(image.scaled(680, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

SetLevel::~SetLevel(){}
