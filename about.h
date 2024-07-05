#ifndef ABOUT_H
#define ABOUT_H

#include <QFrame>

class QLabel;
class QPushButton;
class QStackedWidget;
class QWidget;

class About : public QFrame
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = nullptr);

private slots:
    void showInstructionPanel();
    void showAboutPanel();

private:
    void setupAboutPanel();
    void setupInstructionPanel();
    QLabel* createStyledLabel(const QString& text);
    QPushButton* createStyledButton(const QString& text);
    QString getAboutText();
    QString getInstructionText();

    QStackedWidget *m_stackedWidget;
    QWidget *m_aboutPanel;
    QWidget *m_instructionPanel;
};

#endif // ABOUT_H
