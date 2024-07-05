#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *bg, *logo;
    QPushButton *start, *demo, *about, *exit;

    void onButtonClick();

};
#endif // MAINWINDOW_H



