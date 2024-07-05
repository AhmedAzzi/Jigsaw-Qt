#include "about.h"
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextBrowser>
#include <QPalette>

About::About(QWidget *parent)
: QFrame(parent)
, m_stackedWidget(new QStackedWidget(this))
{
    setFixedSize(500, 500);
    setWindowTitle("About");


    setupAboutPanel();
    setupInstructionPanel();

    m_stackedWidget->addWidget(m_aboutPanel);
    m_stackedWidget->addWidget(m_instructionPanel);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
    mainLayout->setSpacing(0); // Remove spacing
    mainLayout->addWidget(m_stackedWidget);
    setLayout(mainLayout);

    m_stackedWidget->setCurrentWidget(m_aboutPanel);

    // Remove frame border
    setFrameStyle(QFrame::NoFrame);
}

void About::setupAboutPanel()
{
    m_aboutPanel = new QWidget(this);
    m_aboutPanel->setStyleSheet("background-color: #c5935e;");

    QLabel *titleLabel = createStyledLabel(getAboutText());
    QPushButton *helpButton = createStyledButton("Help?");

    connect(helpButton, &QPushButton::clicked, this, &About::showInstructionPanel);

    QVBoxLayout *layout = new QVBoxLayout(m_aboutPanel);
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);
    layout->addWidget(helpButton, 0, Qt::AlignCenter);
    m_aboutPanel->setLayout(layout);
}

void About::setupInstructionPanel()
{
    m_instructionPanel = new QWidget(this);
    m_instructionPanel->setStyleSheet("background-color: #c5935e;");

    QLabel *instructionLabel = createStyledLabel(getInstructionText());
    QPushButton *backButton = createStyledButton("Back");

    connect(backButton, &QPushButton::clicked, this, &About::showAboutPanel);

    QVBoxLayout *layout = new QVBoxLayout(m_instructionPanel);
    layout->addWidget(instructionLabel, 0, Qt::AlignCenter);
    layout->addWidget(backButton, 0, Qt::AlignCenter);
    m_instructionPanel->setLayout(layout);
}

QLabel* About::createStyledLabel(const QString& text)
{
    QLabel *label = new QLabel(this);
    label->setText(text);
    label->setStyleSheet("font-family:  Z003, sans-serif; font-size: 20px; color: black;");
    label->setAlignment(Qt::AlignCenter);
    label->setFixedSize(360, 400);
    return label;
}

QPushButton* About::createStyledButton(const QString& text)
{
    QPushButton *button = new QPushButton(text, this);
    button->setStyleSheet(" color: black; font-family:  Z003, sans-serif; font-size: 18px;");
    button->setFixedSize(100, 30);
    return button;
}

void About::showInstructionPanel()
{
    m_stackedWidget->setCurrentWidget(m_instructionPanel);
}

void About::showAboutPanel()
{
    m_stackedWidget->setCurrentWidget(m_aboutPanel);
}

QString About::getAboutText()
{
    return "𝕵𝖎𝖌𝖘𝖆𝖜 𝕳𝕾𝕽\n"
           "𝖁𝖊𝖗𝖘𝖎𝖔𝖓 : 1.0\n"
           "𝕮𝖗𝖊𝖆𝖙𝖎𝖔𝖓 𝕯𝖆𝖙𝖊: May 29, 2023\n"
           "This puzzle game was created using Qt C++.\n"
           "Développeur : Ahmed Azzi\n"
           "Date de création : 5 Jul 2024\n"
           "Ce jeu de puzzle a été créé avec Qt C++.\n"
           "Amusez-vous bien !";
}

QString About::getInstructionText()
{
    return "𝕵𝖎𝖌𝖘𝖆𝖜 𝕳𝕾𝕽 - 𝕲𝖆𝖒𝖊 𝕴𝖓𝖘𝖙𝖗𝖚𝖈𝖙𝖎𝖔𝖓𝖘\n\n"
           "• Move puzzle pieces by clicking on them with the mouse.\n"
           "• The goal is to reassemble the image by swapping the pieces in the correct order.\n"
           "• Use the 'Back' button to go back to the levels interface.\n"
           "• Pause the game by clicking the pause button.\n"
           "• Use the reset button to restart the game at any time.\n"
           "• Finish the game by placing all the pieces in the correct order.\n"
           "• The game also ends if you close the game window.";
}
