/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QTimer>
#include <iostream>
using namespace std;

#include "window.h"
#include "p_movementtransition.h"
#include "game_object.h"
#include <sstream>


//![0]
Window::Window(int width, int height, int type)
{
//   cout << width << endl;
//   cout << height << endl;


    pX = 5;
    pY = 5;
//![0]
    QFontDatabase database;
    QFont font;
    if (database.families().contains("Monospace"))
        font = QFont("Monospace", 12);
    else {
        foreach (QString family, database.families()) {
            if (database.isFixedPitch(family)) {
                font = QFont(family, 12);
                break;
            }
        }
    }
    setFont(font);

//![1]
    qRegisterMetaType<Qt::Key>("Qt::Key");
    connect(this, SIGNAL(change_player(Qt::Key)),  
	    SLOT(move_and_redraw(Qt::Key)), 
	    Qt::QueuedConnection);
    buildMachine();
    board = NULL;

    if( board != NULL )
       delete board;
    

    board = new game_board(this, type);
    this->width = board->get_width();
    this->height = board->get_height();
}
//![1]
/*
Window::~Window()
{
   delete game_board;
   delete machine;
}
*/
void Window::update()
{
   board->update();
   repaint();
}

QColor gray(139, 137, 137);

void Window::setStatus(const QString &status)
{
    myStatus = status;
    repaint();
}

void Window::setFSize(int x, int y)
{
   QFontMetrics metrics(font());
   this->setFixedSize( QSize(metrics.width('X') * x, metrics.height() * (y + 1)));
}

QString Window::status() const
{
    return myStatus;
}

void Window::paintEvent(QPaintEvent * /* event */)
{

    QFontMetrics metrics(font());
    QPainter painter(this);
    int fontHeight = metrics.height();
    int fontWidth = metrics.width('X');
    int yPos = fontHeight;
    int xPos;

    painter.fillRect(rect(), Qt::black);
    painter.setPen(Qt::white);

    painter.drawText(QPoint(0, yPos), status());
    
    for (int y = 0; y < board->get_height(); ++y) {
        yPos += fontHeight;
        xPos = 0;
	
        for (int x = 0; x < board->get_width(); ++x) {
	   QChar newChar = board->get_char(x,y);
	   if( newChar == '@' || newChar == 'S' || newChar == 'E')
	      painter.setPen(Qt::white);
	   else if( newChar == 'B' || newChar == 'Z')
	      painter.setPen(Qt::green);
	   else if( newChar == 'D' || newChar == '+')
	      painter.setPen(Qt::red);
	   else if( newChar == '#' )
	      painter.setPen(gray);
	   else if( newChar == '.' )
	      painter.setPen(Qt::black);
	   else if( newChar == 'W' )
	      painter.setPen(Qt::blue);
	   else
	      painter.setPen(Qt::white);
	   painter.drawText(QPoint(xPos, yPos), newChar);
	   xPos += fontWidth;
        }
    }

    string lives = "Lives: ";
    std::stringstream ss;
    ss << board->get_lives();
    lives += ss.str();
    xPos = 0;
    yPos = 2*fontHeight;
    for(int i = 0; i < lives.length(); i++)
    {
       QChar newChar = lives[i];
       painter.fillRect(xPos, fontHeight, fontWidth, fontHeight, Qt::black);
       painter.drawText(QPoint(xPos, yPos), newChar);
       xPos += fontWidth;
    }
}

QSize Window::sizeHint() const
{
    QFontMetrics metrics(font());

    return QSize(metrics.width('X') * width, metrics.height() * (height + 1));
}

void Window::run_new_game( int time_interval, int width, int height )
{

//     board->setup_gauntlet();
}

//![2]
void Window::buildMachine()
{
    machine = new QStateMachine;

    QState *inputState = new QState(machine);
    inputState->assignProperty(this, "status", "Move the rogue with 2, 4, 6, and 8");

    P_Movementtransition *transition = new P_Movementtransition(this);
    inputState->addTransition(transition);
//![2]

//![3]
    QState *quitState = new QState(machine);
    quitState->assignProperty(this, "status", "Really quit(y/n)?"); 

    QKeyEventTransition *yesTransition = new
        QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Y);
    yesTransition->setTargetState(new QFinalState(machine));
    quitState->addTransition(yesTransition);

    QKeyEventTransition *noTransition =
        new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_N);
    noTransition->setTargetState(inputState);
    quitState->addTransition(noTransition);
//![3]

//![4]
    QKeyEventTransition *quitTransition =
        new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Q);
    quitTransition->setTargetState(quitState);
    inputState->addTransition(quitTransition);
//![4]

//![5]
    machine->setInitialState(inputState);

    connect(machine, SIGNAL(finished()), qApp, SLOT(quit()));

    machine->start();
}
//![5]

void Window::movePlayer(Qt::Key key)
{
   emit change_player(key);
//   board->move_player(key);
//   repaint();
}

void Window::move_and_redraw(Qt::Key key)
{
   board->move_player(key);
   repaint();
}


