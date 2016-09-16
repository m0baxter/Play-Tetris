
#include "tetrisPlayer.hpp"
#include "X11Interactions.hpp"
#include <X11/Xlib.h>
#include <iostream>

extern "C" {
   #include <xdo.h> 
}


TetrisPlayer :: TetrisPlayer() {
   /* Constructor for TetrisPlayer object. */

   display = XOpenDisplay( NULL );
   xdo = xdo_new(NULL);

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

