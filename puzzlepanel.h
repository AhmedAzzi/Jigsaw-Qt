#ifndef PUZZLEPANEL_H
#define PUZZLEPANEL_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QList>
#include <QPoint>
#include <QSoundEffect>

class PuzzlePanel : public QWidget {
    Q_OBJECT

public:
    PuzzlePanel(int dim, const QString& name, int second, QWidget *parent = nullptr);

private:
    QGridLayout *layout;
    QImage source;
    QPixmap resized;
    QList<QPushButton*> buttons;
    QList<QPoint> solution;
    bool firstClick;
    int lastClicked;
    QString name;
    QLabel *img_preview;
    QPushButton *back;
    QRadioButton *pause;
    QPushButton *reset;
    QLabel *timerLabel;
    QLabel *lastTime;
    QLabel *one, *two, *three;
    QLabel *test;
    QTimer *timer;
    int remainingTime;
    int startingTime;
    int dim;
    QSoundEffect beep; // Declare QMediaPlayer object
    QSoundEffect winner;

    QString formatTime(int totalSeconds);
    void updateTimer();
    void handleClick();
    void checkWin();
    void moveAndScaleImage(QLabel *icon, int startX, int startY, int endX, int endY, double scaleFactor, int duration);
};

#endif // PUZZLEPANEL_H
