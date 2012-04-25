#include "QRogueWidget.h"


#include <iostream>
using namespace std;

/** QRogueWidget **
 *
 * See header file for comments
 *
 **/
QRogueWidget::QRogueWidget()
{
   layout = new QHBoxLayout();
   rightSide = new QWidget();
   rightSide->setMinimumSize(370,300);
   rightLayout = new QVBoxLayout();
   
   
   QPalette pal = rightSide->palette();
   pal.setColor(rightSide->backgroundRole(), Qt::black);
   rightSide->setPalette(pal);
   rogueDrawing = new rogueText();
   rogueDrawing->setFixedSize(370, 160);
   rightLayout->addWidget(rogueDrawing);
   
   closingInButton = new QPushButton("Closing In");
   closingInButton->setMaximumSize(150, 30);
   closingInButton->setFocusPolicy(Qt::NoFocus);
   connect(closingInButton, SIGNAL(clicked()), this, SLOT(showClosingIn()));
   gauntletButton = new QPushButton("Gauntlet");
   gauntletButton->setFocusPolicy(Qt::NoFocus);
   gauntletButton->setMaximumSize(150, 30);
   connect(gauntletButton, SIGNAL(clicked()), this, SLOT(showGauntlet()));
   dungeonButton = new QPushButton("Dungeon");
   dungeonButton->setMaximumSize(150, 30);
   dungeonButton->setFocusPolicy(Qt::NoFocus);
   connect(dungeonButton, SIGNAL(clicked()), this, SLOT(showDungeon()));
   rightLayout->addWidget(closingInButton);
   rightLayout->addWidget(gauntletButton);
   rightLayout->addWidget(dungeonButton);
   
   rightLayout->setAlignment(closingInButton, Qt::AlignHCenter);
   rightLayout->setAlignment(gauntletButton, Qt::AlignHCenter);
   rightLayout->setAlignment(dungeonButton, Qt::AlignHCenter);
  
   rightSide->setLayout(rightLayout);
   
   layout->addWidget(rightSide);
   this->setLayout(layout);
   
   closingIn = NULL;
   gauntlet = NULL;
   

}


void QRogueWidget::showClosingIn()
{
   clearDisplay();
   closingIn = new ClosingIn();
   closingIn->grabKeyboard();
   layout->addWidget(closingIn);
}

void QRogueWidget::showGauntlet()
{
   clearDisplay();
   gauntlet = new Window(40, 40, Window::GAUNTLET);
   gauntlet->grabKeyboard();
   layout->addWidget(gauntlet);
}

void QRogueWidget::showDungeon()
{
   clearDisplay();
   dungeon = new Window(40, 40, Window::DUNGEON);
   dungeon->grabKeyboard();
   layout->addWidget(dungeon);
}

void QRogueWidget::clearDisplay()
{
   if(closingIn != NULL)
   {
      layout->removeWidget(closingIn);
      delete closingIn;
      closingIn = NULL;
   }
   else if(gauntlet != NULL)
   {
      layout->removeWidget(gauntlet);
      delete gauntlet;
      gauntlet = NULL;
   }
   else if(dungeon != NULL)
   {
      layout->removeWidget(dungeon);
      delete dungeon;
      dungeon = NULL;
   }
}
