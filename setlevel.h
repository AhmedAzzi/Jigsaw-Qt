#ifndef SETLEVEL_H
#define SETLEVEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>

class SetLevel : public QWidget
{
    Q_OBJECT

public:
    SetLevel(QWidget *parent = nullptr);
    ~SetLevel();



private:
    QLabel *intro;
    QRadioButton *easy;
    QRadioButton *medium;
    QRadioButton *hard;
    QRadioButton *so_hard;
    QPushButton *play;
    QPushButton *back;
    QLabel *imageLabel;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QWidget *imagePanel;
    int dim, second;
    int currentImage;
    QStringList imageFiles;

    void createWidgets();
    void groupButtonLevel();
    void levRadioButtonSelected();
    void fileChooser();
};

#endif // SETLEVEL_H
