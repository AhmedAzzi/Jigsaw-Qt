#include "puzzlepanel.h"
#include "setlevel.h"
#include <QApplication>
#include <QScreen>
#include <QIcon>
#include <QFileInfo>
#include <algorithm>
#include <QTimer>
#include <random>
#include <chrono>
#include <QMovie>
#include <QMessageBox>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>

PuzzlePanel::PuzzlePanel(int dim, const QString& name, int second, QWidget *parent)
    : QWidget(parent), firstClick(false), lastClicked(0), name(name), test(nullptr), remainingTime(second), startingTime(second), dim(dim), beep(nullptr), winner(nullptr)
{
    setFixedSize(QGuiApplication::primaryScreen()->geometry().width(), QGuiApplication::primaryScreen()->geometry().height());

    QString imgsPath = "../../resources/images/";


    // player->setSource(QUrl(audioPath + "qrc:/sounds/click.wav")); // Adjust path to your sound file




    QPixmap image(name);
    img_preview = new QLabel(this);
    img_preview->setGeometry(QGuiApplication::primaryScreen()->geometry().width() - 310, 60, 280, 180);
    img_preview->setPixmap(image.scaled(280, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    back = new QPushButton(this);
    back->setIcon(QIcon(imgsPath + "back.png"));
    back->setStyleSheet("border: none; background: transparent;");
    back->setIconSize(QSize(250, 180));
    back->setFixedSize(100, 60);
    back->setCursor(Qt::PointingHandCursor);
    back->move(0, 0);
    connect(back, &QPushButton::clicked, this, [=]() {
        (new SetLevel(nullptr))->show();
        close();
    });

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PuzzlePanel::updateTimer);
    timer->start(1000);  // Update every second

    timerLabel = new QLabel(formatTime(second), this);
    timerLabel->setStyleSheet("font-size: 30px; font-family: Z003; color: black;background: transparent;");
    timerLabel->setGeometry(width() / 3 - 200, 0, 400, 50);
    timerLabel->setAlignment(Qt::AlignCenter);


    QMap<int, QString> difficultyMap = {
        {3, "easy"},
        {4, "medium"},
        {5, "hard"},
        {6, "so hard"}
    };

    QFile file("../../best_time.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
    }

    int number = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed(); // Read and trim whitespace
        QStringList parts = line.split(':'); // Split line by ':'

        if (parts.size() == 2) {
            QString currentDifficulty = parts[0].trimmed();
            if (currentDifficulty == difficultyMap.value(dim)) {
                number = parts[1].trimmed().toInt();
                break; // Stop searching once found
            }
        }
    }

    file.close();


    lastTime = new QLabel("Less Elapsed Time: " + QString::number(number), this);
    lastTime->setStyleSheet("font-size: 30px; font-family: Z003; color: black;background: transparent;");
    lastTime->setGeometry(2 * width() / 3 - 200, 0, 400, 50);
    lastTime->setAlignment(Qt::AlignCenter);

    this->setStyleSheet(
        "QRadioButton::indicator {"
        "    width: 60px;"
        "    height: 60px;"
        "}"
        "QRadioButton::indicator::unchecked {"
        "    image: url(" + imgsPath + "pause.png);"
                     "}"
                     "QRadioButton::indicator::checked {"
                     "    image: url(" + imgsPath + "play.png);"
                     "}"
        "* {"
             "background-color: #d3c3b6;"
        "}"
        );

    pause = new QRadioButton("", this);
    pause->setGeometry(width() - pause->width() , 5, 40, 40);
    pause->setCursor(Qt::PointingHandCursor);
    pause->setStyleSheet("border: none; margin-left: -10px;");
    connect(pause, &QRadioButton::toggled, this, [this](bool checked) {
        if (checked) {
            timer->stop();
            for (auto btn : buttons) {
                btn->setEnabled(false);
            }
        } else {
            timer->start(1000);
            for (auto btn : buttons) {
                btn->setEnabled(true);
            }
        }
    });

    reset = new QPushButton(this);
    reset->setIcon(QIcon(imgsPath + "reset.png"));
    reset->setStyleSheet("border: none;");
    reset->setIconSize(QSize(60, 60));
    reset->setGeometry(width() - reset->width() * 1.5, 5, 40, 40);
    reset->setCursor(Qt::PointingHandCursor);
    connect(reset, &QPushButton::clicked, this, [=]() {
        remainingTime = second;  // Or whatever initial time you want
        timerLabel->setText(formatTime(remainingTime));
        timer->start(1000);
        for (auto btn : buttons) {
            btn->setEnabled(true);
        }

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(buttons.begin(), buttons.end(), std::default_random_engine(seed));

        for (int r = 0; r < dim; ++r) {
            for (int c = 0; c < dim; ++c) {
                int index = r * dim + c;
                if (index < buttons.size()) {
                    layout->addWidget(buttons[index], r, c);
                }
            }
        }

    });

    one = new QLabel("", this);
    one->setGeometry(QGuiApplication::primaryScreen()->geometry().width() - 250, 260, 280, 200);
    QPixmap one_img(imgsPath + "01.png");
    one->setAttribute(Qt::WA_TranslucentBackground);
    one->setPixmap(one_img.scaled(280, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    two = new QLabel("", this);
    two->setGeometry(QGuiApplication::primaryScreen()->geometry().width() - 320, 470, 280, 200);
    QPixmap two_img(imgsPath + "02.png");
    two->setAttribute(Qt::WA_TranslucentBackground);
    two->setPixmap(two_img.scaled(280, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    three = new QLabel("", this);
    three->setGeometry(QGuiApplication::primaryScreen()->geometry().width() - 160, 470, 280, 200);
    QPixmap three_img(imgsPath + "03.png");
    three->setAttribute(Qt::WA_TranslucentBackground);
    three->setPixmap(three_img.scaled(280, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    source.load(name);
    resized = QPixmap::fromImage(source.scaled(860, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    layout = new QGridLayout();

    for (int i = 0; i < dim * dim; ++i) {
        int w = i / dim;
        int col = i % dim;
        solution.append(QPoint(w, col));
    }

    int pieceWidth = resized.width() / dim;
    int pieceHeight = resized.height() / dim;

    for (int i = 0; i < dim * dim; ++i) {
        int x = i % dim;
        int y = i / dim;
        QPixmap image = resized.copy(x * pieceWidth, y * pieceHeight, pieceWidth, pieceHeight);

        QPushButton *button = new QPushButton(this);
        button->setIcon(QIcon(image));
        button->setIconSize(QSize(pieceWidth, pieceHeight));
        button->setFixedSize(pieceWidth, pieceHeight);
        button->setStyleSheet("QPushButton { border: none; padding: 0px; }");
        button->setProperty("position", QVariant(QPoint(y, x)));
        button->setCursor(Qt::PointingHandCursor);
        connect(button, &QPushButton::clicked, this, &PuzzlePanel::handleClick);
        buttons.append(button);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(buttons.begin(), buttons.end(), std::default_random_engine(seed));

    for (int r = 0; r < dim; ++r) {
        for (int c = 0; c < dim; ++c) {
            int index = r * dim + c;
            if (index < buttons.size()) {
                layout->addWidget(buttons[index], r, c);
            }
        }
    }

    QWidget *container = new QWidget(this);
    container->setLayout(layout);
    container->setStyleSheet("background: transparent;");
    container->setContentsMargins(0,0,0,0);
    container->setFixedSize(dim * 1.2 * pieceWidth, dim * pieceHeight * 1.2);
    container->move(  (QGuiApplication::primaryScreen()->geometry().width()) / 100, (QGuiApplication::primaryScreen()->geometry().height()) / 10);
}

void PuzzlePanel::handleClick()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    int buttonIndex = buttons.indexOf(button);

    QString audioPath = "../../resources/audio/";
    beep.setSource(QUrl::fromLocalFile(audioPath + "beep2.wav"));
    beep.play();

    if (firstClick) {



        std::swap(buttons[buttonIndex], buttons[lastClicked]);
        layout->removeWidget(buttons[buttonIndex]);
        layout->removeWidget(buttons[lastClicked]);
        for (auto btn : buttons) {
            btn->setStyleSheet("");
            layout->addWidget(btn);
        }
        layout->update();
    } else {
        button->setStyleSheet("border: 3px solid red;");
    }

    firstClick = !firstClick;
    lastClicked = buttonIndex;

    // Check if puzzle is solved after each move
    checkWin();
}


void PuzzlePanel::checkWin()
{
    bool solved = true;
    QString imgsPath_ = "../../resources/images/";
    for (int i = 0; i < buttons.size(); ++i) {
        QPoint currentPos = buttons[i]->property("position").toPoint();
        if (currentPos != solution[i]) {
            solved = false;
            break;
        }
    }
    if (solved) {
        timer->stop();


        QString audioPath = "../../resources/audio/";
        winner.setSource(QUrl::fromLocalFile(audioPath + "winner.wav"));
        winner.play();
        winner.setLoopCount(3);


        int newValue = startingTime - remainingTime;  // Assuming initial time is 90 seconds

        QMap<int, QString> difficultyMap = {
            {3, "easy"},
            {4, "medium"},
            {5, "hard"},
            {6, "so hard"}
        };

        QString targetDifficulty = difficultyMap.value(dim);
        QFile file("../../best_time.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for reading:" << file.errorString();
        }

        QList<QString> lines;
        QTextStream in(&file);
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }
        file.close();

        for (int i = 0; i < lines.size(); ++i) {
            QString line = lines[i];
            if (line.startsWith(targetDifficulty + ":")) {
                QStringList parts = line.split(':');
                if (parts.size() == 2) {
                    int currentValue = parts[1].trimmed().toInt();
                    if (newValue < currentValue) {
                        lines[i] = targetDifficulty + ":" + QString::number(newValue);
                    }
                    break;
                }
            }
        }

        // Write the modified list back to the file
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing:" << file.errorString();
        }

        QTextStream out(&file);
        for (const QString &line : lines) {
            out << line << "\n";
        }
        file.close();


        QWidget *winWidget = new QWidget(this);
        winWidget->setGeometry((QGuiApplication::primaryScreen()->geometry().width()  - 850) / 2 - 200, (QGuiApplication::primaryScreen()->geometry().height()  - 580) / 2 - 20 , 950, 580);
        winWidget->setStyleSheet("background: transparent");

        // Display the original image
        QLabel *imageLabel = new QLabel(winWidget);
        imageLabel->setPixmap(QPixmap::fromImage(source).scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setFixedSize(950, 580);
        imageLabel->setStyleSheet("background: transparent");
        imageLabel->setGeometry(0, 0, 950, 580);

        // Display the GIF
        QLabel *gifLabel = new QLabel(winWidget);
        QMovie *gifMovie = new QMovie(imgsPath_ + "bg.gif");
        gifLabel->setMovie(gifMovie);
        gifLabel->setAlignment(Qt::AlignCenter);
        gifLabel->setStyleSheet("background: transparent");
        gifLabel->setGeometry(0,0 , 950, 580);
        gifMovie->start();

        // Show the win display
        winWidget->show();

        // // Optionally, disable further interactions with the puzzle
        for (auto btn : buttons) {
            btn->hide();
        }
        reset->setEnabled(false);
        pause->setEnabled(false);

        if ((dim == 3 && remainingTime >= 20) || (dim == 4 && remainingTime >= 40) || (dim == 5 && remainingTime >= 60)
            || (dim == 6 && remainingTime >= 80)) {
            moveAndScaleImage(one, QGuiApplication::primaryScreen()->geometry().width() - 250, 260, 400, 300 - 20, 1.75, 9000); // Example animation parameters

        } else if ((dim == 3 && remainingTime >= 10) || (dim == 4 && remainingTime >= 20) || (dim == 5 && remainingTime >= 30)
                   || (dim == 6 && remainingTime >= 40)) {
            moveAndScaleImage(two, QGuiApplication::primaryScreen()->geometry().width() - 320, 470, 400, 300 - 20, 1.75, 9000); // Example animation parameters
        } else if ((dim == 3 && remainingTime >= 0) || (dim == 4 && remainingTime >= 0) || (dim == 5 && remainingTime >= 0)
                   || (dim == 6 && remainingTime >= 0)) {
            moveAndScaleImage(three, QGuiApplication::primaryScreen()->geometry().width() - 160, 470, 400, 300 - 20, 1.75, 9000); // Example animation parameters
        } else {
            return;
        }



        QTimer::singleShot(3000, this, [=]() {
            QMessageBox msgBox;
            msgBox.setText("This is a message box displayed after 3 seconds.");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();

            // Handle user's response
            if (ret == QMessageBox::Yes) {
                close();
                if (dim < 6)
                    (new PuzzlePanel(dim + 1, name, remainingTime + 30))->show();
            } else if (ret == QMessageBox::No) {
                qDebug() << "User clicked No";
            }
        });

    }
}
void PuzzlePanel::moveAndScaleImage(QLabel *icon, int startX, int startY, int endX, int endY, double scaleFactor, int duration) {
    int frames = duration; // Adjust this value to control the animation speed
    int startWidth = icon->width();
    int startHeight = icon->height();
    QImage originalImage = icon->pixmap().toImage();
    icon->raise();

    for (int i = 0; i <= frames; ++i) {
        double scale = 1.0 + (scaleFactor - 1.0) * i / frames;
        int width = startWidth * scale;
        int height = startHeight * scale;
        QImage scaledImage = originalImage.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap pixmap = QPixmap::fromImage(scaledImage);
        icon->setPixmap(pixmap);
        icon->resize(width, height);

        int x = startX + (endX - startX) * i / frames;
        int y = startY + (endY - startY) * i / frames;
        icon->move(x, y);

        if (i < frames) {
            QApplication::processEvents(QEventLoop::AllEvents, 10); // Adjust the delay to control the animation speed
        }
    }


}


QString PuzzlePanel::formatTime(int totalSeconds)
{
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

void PuzzlePanel::updateTimer()
{
    remainingTime--;
    if (remainingTime >= 0) {
        timerLabel->setText(formatTime(remainingTime));
    } else {
        timer->stop();
        QMessageBox::information(this, "Time's Up", "You've run out of time!");
    }
}
