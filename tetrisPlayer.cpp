
#include "tetrisPlayer.hpp"
#include "scoreReader.hpp"
#include "X11Interactions.hpp"
#include <X11/Xlib.h>
#include <iostream>
#include <sys/stat.h>

#include <thread>

#include <exception>

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

   emulator = std::thread( &system, "nes /home/baxter/Downloads/tet/TETRIS.NES &" );
   sleep(2);

}


TetrisPlayer :: ~TetrisPlayer()  {
   /* Destructor, cleans up by making sure that the second thread dies. */

   focusOnEmulator( display );
   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Ctrl+q", 0);

   emulator.join();

}


void TetrisPlayer :: newGame() {
   /* Starts a new game of tetris. */

   focusOnEmulator( display );
   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "F7", 0);
   sleep(1);
   std::cout << "1" << std::endl;
   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Return", 10000);
   sleep(1);
   std::cout << "2" << std::endl;
   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Return", 10000);
   sleep(1);
   std::cout << "3" << std::endl;
   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Return", 10000);

   return;

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

   int score = -1;

   focusOnEmulator( display );

   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Pause", 0);
   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "F12", 0);

   struct stat buf;

   while ( stat( "/home/baxter/.fceux/snaps/TETRIS-0.png", &buf ) != 0 ) {}

   while (score == -1) {
      
      try {
         score = scoreReader();
      }
      catch (std::exception &except) {
         //std::cout << "Exception caught: " << except.what() << std::endl;
         continue;
      }
   }

   while ( stat( "/home/baxter/.fceux/snaps/TETRIS-0.png", &buf ) == 0 ) {
      system("rm /home/baxter/.fceux/snaps/TETRIS-0.png");
   }

   xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Pause", 0);

   usleep(1);

   return score;
}

