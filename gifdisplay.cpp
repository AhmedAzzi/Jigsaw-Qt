#include "gifdisplay.h"
#include <QVBoxLayout>

GifDisplay::GifDisplay(const QString &gifFilePath, QWidget *parent)
    : QDialog(parent), gifLabel(nullptr), movie(nullptr)
{
    setWindowTitle("GIF Display Window");

    gifLabel = new QLabel(this);
    movie = new QMovie(gifFilePath);

    gifLabel->setMovie(movie);
    movie->start();

    setFixedSize(movie->frameRect().size()); // Set window size to match GIF size

}

GifDisplay::~GifDisplay()
{
    delete movie;
}
