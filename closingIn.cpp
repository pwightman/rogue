/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

#include "closingIn.h"
#include "movementtransition.h"
#include <iostream>

using namespace std;

ClosingIn::ClosingIn()
{
    //![0]
    pX = WIDTH / 2;
    pY = HEIGHT / 2;
    population = 20; // The higher the number, the less populated.
    tightenEvery = 10;
    pointsAvailable = 20;
    moves = 0;
    //![0]


    QFontDatabase database;
    QFont font;
    if (database.families().contains("Monospace"))
        font = QFont("Monospace", 12);
    else 
    {
        foreach (QString family, database.families()) 
        {
            if (database.isFixedPitch(family)) 
            {
                font = QFont(family, 12);
                break;
            }
        }
    }

    setFont(font);


    //![1]
    setupMap();
    buildMachine();

    //![1]
}

void ClosingIn::setStatus(const QString &status)
{
    myStatus = status;
    repaint();
}

QString ClosingIn::status() const
{
    return myStatus;
}

void ClosingIn::paintEvent(QPaintEvent * /* event */)
{
    QFontMetrics metrics(font());
    QPainter painter(this);
    int fontHeight = metrics.height();
    int fontWidth = metrics.width('X');
    int yPos = fontHeight;
    int xPos;

    this->setFixedSize(WIDTH * fontWidth, (HEIGHT + 1) * fontHeight);
    painter.fillRect(rect(), Qt::black);
    painter.setPen(Qt::white);
    
    painter.drawText(QPoint(0, yPos), status());
    
    for (int y = 0; y < HEIGHT; ++y) 
    {
        yPos += fontHeight;
        xPos = 0;
	
        for (int x = 0; x < WIDTH; ++x) 
        {
            if (y == pY && x == pX) 
            {
                xPos += fontWidth;
                continue;
            }
            if(displayMap[x][y] == '.')
            {
                painter.setPen(Qt::black);
                painter.drawText(QPoint(xPos, yPos), displayMap[x][y]);
            }
            else if(displayMap[x][y] == '#')
            {
                painter.setPen(QColor(255, 0, 10, 255));
                painter.drawText(QPoint(xPos, yPos), displayMap[x][y]);
            }
            else if(displayMap[x][y] == '$')
            {
                painter.setPen(Qt::green);
                painter.drawText(QPoint(xPos, yPos), displayMap[x][y]);
            }
            else
            {
                painter.setPen(Qt::white);
                painter.drawText(QPoint(xPos, yPos), displayMap[x][y]);
            }
            xPos += fontWidth;
        }
    }

    painter.setPen(Qt::white);
    painter.drawText(QPoint(pX * fontWidth, (pY + 2) * fontHeight), QChar('@'));
}

QSize ClosingIn::sizeHint() const
{
    QFontMetrics metrics(font());

    return QSize(metrics.width('X') * WIDTH, metrics.height() * (HEIGHT + 1));
}

//![2]
void ClosingIn::buildMachine()
{
    machine = new QStateMachine;
    QState *inputState = new QState(machine);
    inputState->assignProperty(this, "status", "Move the rogue with the arrow keys");

    MovementTransition *transition = new MovementTransition(this);
    inputState->addTransition(transition);
    //![2]

    //![3]
    QState *quitState = new QState(machine);
    quitState->assignProperty(this, "status", "Really quit(y/n)?"); 

    QKeyEventTransition *yesTransition = new
                                         QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Y);
    yesTransition->setTargetState(new QFinalState(machine));
    quitState->addTransition(yesTransition);

    QKeyEventTransition *noTransition = new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_N);
    noTransition->setTargetState(inputState);
    quitState->addTransition(noTransition);
    //![3]

    //![4]
    QKeyEventTransition *quitTransition = new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Q);
    quitTransition->setTargetState(quitState);
    inputState->addTransition(quitTransition);
    //![4]

    // This portion of code still needs work. I cannot get it to draw a new board.
    QState* redrawState = new QState(machine);
    redrawState->assignProperty(this, "status", "Redraw the board(y/n)?");

    QKeyEventTransition* yesRedraw = new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Y);
    yesRedraw->setTargetState(inputState);
    redrawState->addTransition(yesRedraw);

    setupMap();

    QKeyEventTransition* noRedraw = new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_N);
    noRedraw->setTargetState(inputState);
    redrawState->addTransition(noRedraw);

    QKeyEventTransition* redrawTransition = new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_R);
    redrawTransition->setTargetState(redrawState);
    inputState->addTransition(redrawTransition);

    //![5]
    machine->setInitialState(inputState);

    connect(machine, SIGNAL(finished()), qApp, SLOT(quit()));

    machine->start();
}
//![5]

void ClosingIn::tightenMap()
{
    int closeInBy = (moves / tightenEvery);

    for(int x = 0; x < WIDTH; x++)
    {
        for(int y = 0; y < HEIGHT; y++)
        {
            if(x == closeInBy || y == closeInBy)
            {
                displayMap[x][y] = '#';
                displayMap[WIDTH - closeInBy][y] = '#';
                displayMap[x][HEIGHT - closeInBy] = '#';
            }
        }
    }

    if(displayMap[pX - 1][pY] == '#' &&
       displayMap[pX + 1][pY] == '#' &&
       displayMap[pX][pY - 1] == '#' &&
       displayMap[pX][pY + 1] == '#')
    {
        // Then we are surrounded by blocks and the game needs to end.
        // How to end?

        machine->stop();
        exit(0);
    }
}

void ClosingIn::loosenMap()
{
    int looseBy = (moves / tightenEvery) - 3;

    if(looseBy < 0)
    {
        looseBy = 0;
        moves = 0;
    }

    else
        moves = moves - 30;

    for(int x = looseBy; x < (WIDTH - looseBy); x++)
    {
        for(int y = looseBy; y < (HEIGHT - looseBy); y++)
        {
            displayMap[x][y] = originalMap[x][y];
        }
    }
}

void ClosingIn::movePlayer(Direction direction)
{
    switch (direction)
    {
    case Left:
        {
            if (displayMap[pX - 1][pY] != '#')
            {
                --pX;
            }
            break;
        }

    case Right:
        {
            if (displayMap[pX + 1][pY] != '#')
            {
                ++pX;
            }
            break;
        }

    case Up:
        {
            if (displayMap[pX][pY - 1] != '#')
            {
                --pY;
            }
            break;
        }

    case Down:
        {
            if (displayMap[pX][pY + 1] != '#')
            {
                ++pY;
            }
            break;
        }
    }

    moves++;

    if(displayMap[pX][pY] == '$')
    {
        // This is where we need to take the walls back a few steps. Maybe 3?
        score++;
        displayMap[pX][pY] = '.';
        originalMap[pX][pY] = '.';
        pointsAvailable--;

        cout << pointsAvailable << endl;

        if(pointsAvailable == 0)
        {
            // Then the player has hit all the gems, and we need to say game over.

            machine->stop();
            exit(0);
        }

        loosenMap();
    }

    else if(moves % tightenEvery == 0)
    {
        tightenMap();
    }

    repaint();
}

void ClosingIn::setupMap()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < HEIGHT; ++y)
        {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1 || qrand() % population == 0)
            {
                displayMap[x][y] = '#';
                originalMap[x][y] = '#';
            }
            else
            {
                displayMap[x][y] = '.';
                originalMap[x][y] = '.';
            }
        }
    }

    srand( time(NULL) );
    for(int i = 0; i < pointsAvailable; i++)
    {
        int tempX = rand() % WIDTH;
        int tempY = rand() % HEIGHT;

        while(tempX == 0 || tempY == 0 || tempX >= WIDTH - 1 || tempY >= HEIGHT - 1)
        {
            tempX = rand() % WIDTH;
            tempY = rand() % HEIGHT;
        }

        displayMap[tempX][tempY] = '$';
        originalMap[tempX][tempY] = '$';
    }
}
