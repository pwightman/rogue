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

#ifndef WINDOW_H
#define WINDOW_H

#include <QFrame>

#include "dragon.h"
#include "game_board.h"

QT_BEGIN_NAMESPACE
class QState;
class QStateMachine;
class QTransition;
QT_END_NAMESPACE

/** Window **
 *
 * This is the class provided by the Nokia demo.  I will not comment it since
 * I did not make it but it sufficeth to say it holds and draws the game_board
 * to the screen and passes keyPress events to the game_board.
 *
 **/

//![0]
class Window : public QFrame
{
   Q_OBJECT
      Q_PROPERTY(QString status READ status WRITE setStatus)
      
      public:
    enum Direction { Up, Down, Left, Right };
    
    Window(int width, int height, int type);
//    ~Window();
    
    void setStatus(const QString &status);
    QString status() const;
    static const int GAUNTLET = 0;
    static const int DUNGEON = 1;
    
    QSize sizeHint() const;
    void run_new_game( int time_interval, int width, int height);
    void setFSize(int x, int y);
   
    
    void movePlayer(Qt::Key key);
  signals:
    void change_player(Qt::Key key);
  protected:
    void paintEvent(QPaintEvent *event);
    public slots:
    void move_and_redraw(Qt::Key key);
//![0]
    
//![1]
  private:
    void buildMachine();
    int type;
    game_board* board;
    int width, height;
    
    int pX, pY;
    
    QStateMachine *machine;
    QString myStatus;
    
    public slots:
    void update();
};
//![1]

#endif

