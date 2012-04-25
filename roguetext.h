#ifndef ROGUETEXT_H
#define ROGUETEXT_H

#include <QWidget>
#include <string>
#include <iostream>

QT_BEGIN_NAMESPACE
class QState;
class QStateMachine;
class QTransition;
QT_END_NAMESPACE

class rogueText : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status WRITE setStatus)

public:
            rogueText();

    void setStatus(const QString &status);

    QSize sizeHint() const;
    QString status() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    void setupBanner();

    QString banner[8];

    QStateMachine *machine;
    QString myStatus;
};

#endif // ROGUETEXT_H
