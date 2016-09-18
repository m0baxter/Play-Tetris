
#include "tetrisPlayer.hpp"
#include "scoreReader.hpp"
#include "X11Interactions.hpp"
#include <X11/Xlib.h>
#include <iostream>
#include <sys/stat.h>

extern "C" {
   #include <xdo.h> 
}


TetrisPlayer :: TetrisPlayer() {
   /* Constructor for TetrisPlayer object. */

   display = XOpenDisplay( NULL );
   xdo = xdo_new(NULL);

   struct stat buf;

   if ( stat( "svm.txt", &buf ) == 0 ) {
      scoreReader.loadReader( "svm.txt" );
   }

   else {
      scoreReader.trainReader( 0, 213, "./scoreData/scores.txt" );
      scoreReader.saveReader( "svm.txt" );
   }

}


void TetrisPlayer :: pressButton( const int button ) {
   /* Press the button associated to int button. */

   focusOnEmulator( display );

   switch ( button ) {
      case 1:
         xdo_send_keysequence_window(xdo, CURRENTWINDOW, "a", 10000);
         break;

      case 2:
         xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Down", 10000);
         break;

      case 3:
         xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Left", 10000);
         break;

      case 4:
         xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Right", 10000);
         break;

      case 5:
         xdo_send_keysequence_window(xdo, CURRENTWINDOW, "s", 10000);
         break;

      case 6:
         xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Up", 10000);
         break;

      default:
         std::cout << "That is not a button" << std::endl;
         return;
   }

   return;

}


int TetrisPlayer :: getScore() {

   focusOnEmulator( display );

   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Pause", 0);
   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "F12", 0);

   struct stat buf;

   while ( stat( "/home/baxter/.fceux/snaps/TETRIS-0.png", &buf ) != 0 ) {}

   int score = scoreReader();

   while ( stat( "/home/baxter/.fceux/snaps/TETRIS-0.png", &buf ) == 0 ) {
      system("rm -f /home/baxter/.fceux/snaps/TETRIS-0.png");
   }

   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Pause", 0);

   return score;
}
