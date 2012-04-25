#include <QtGui>
#include "window.h"
#include "roguetext.h"
#include "closingIn.h"

/** QRogueWidget **
 *
 * Represents the buttons and layout of the game GUI and
 * is in charge of replacing each game with the new one
 * selected.
 *
 **/
class QRogueWidget : public QWidget
{
  public:
   QRogueWidget();
   Q_OBJECT
//   Q_PROPERTY(QString text READ text WRITE setText)

      protected:

  private:
   QHBoxLayout* layout;
   QWidget* rightSide;
   QVBoxLayout* rightLayout;
   rogueText* rogueDrawing;
   QPushButton* closingInButton;
   QPushButton* gauntletButton;
   QPushButton* dungeonButton;
   QWidget* currentWidget;
   Window* gauntlet;
   Window* dungeon;
   ClosingIn* closingIn;


   public slots:
   /**
    *
    * These functions change which game is showing
    * in response to mouse clicks
    *
    **/
   void showClosingIn();
   void showGauntlet();
   void showDungeon();
   void clearDisplay();
};
