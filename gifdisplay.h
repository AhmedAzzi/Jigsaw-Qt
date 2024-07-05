#ifndef GIFDISPLAY_H
#define GIFDISPLAY_H

#include <QDialog>
#include <QLabel>
#include <QMovie>

class GifDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit GifDisplay(const QString &gifFilePath, QWidget *parent = nullptr);
    ~GifDisplay();

private:
    QLabel *gifLabel;
    QMovie *movie;
};

#endif // GIFDISPLAY_H
