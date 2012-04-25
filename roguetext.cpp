#include <QtGui>
#include <iostream>
#include <string>

#include "roguetext.h"

using namespace std;

rogueText::rogueText()
{
    QFontDatabase database;
    QFont font;
    if (database.families().contains("Monospace"))
        font = QFont("Monospace", 10);
    else
    {
        foreach (QString family, database.families())
        {
            if (database.isFixedPitch(family))
            {
                font = QFont(family, 10);
                break;
            }
        }
    }
    setFont(font);

    banner[0] = " _______  _______  _______           _______";
    banner[1] = "(  ____ )(  ___  )(  ____ \\|\\     /|(  ____ \\";
    banner[2] = "| (    )|| (   ) || (    \\/| )   ( || (    \\/";
    banner[3] = "| (____)|| |   | || |      | |   | || (__";
    banner[4] = "|     __)| |   | || | ____ | |   | ||  __)";
    banner[5] = "| (\\ (   | |   | || | \\_  )| |   | || (";
    banner[6] = "| ) \\ \\__| (___) || (___) || (___) || (____/\\";
    banner[7] = "|/   \\__/(_______)(_______)(_______)(_______/";
}

QSize rogueText::sizeHint() const
{
    QFontMetrics metrics(font());

    return QSize(metrics.width('X'), metrics.height());
}

void rogueText::paintEvent(QPaintEvent * /* event */)
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

    for (int y = 0; y < 8; ++y)
    {
       yPos += fontHeight;
       xPos = 0;
       QString stringTemp = banner[y];
       for (int x = 0; x < banner[y].length(); ++x)
       {
	  QChar charTemp = stringTemp[x];
	  if(banner[y][x] == ' ')
	  {
	     painter.setPen(Qt::black);
	     painter.drawText(QPoint(xPos, yPos), charTemp);
	  }
            else
            {
	       painter.setPen(Qt::white);
	       painter.drawText(QPoint(xPos, yPos), charTemp);
            }
	  xPos += fontWidth;
       }
    }
}

void rogueText::setStatus(const QString &status)
{
    myStatus = status;
    repaint();
}

QString rogueText::status() const
{
    return myStatus;
}
