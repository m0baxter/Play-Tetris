
#include "X11Interactions.hpp" 
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <typeinfo>


Window* listWindows( Display* display, unsigned long* len) {
   /* Returns a list of all open windows on the display. */

   Atom prop = XInternAtom( display, "_NET_CLIENT_LIST", False ), type;
   int form;
   unsigned long remain;
   unsigned char *list;

   if ( XGetWindowProperty( display, XDefaultRootWindow(display), prop, 0, 1024, False,
                            XA_WINDOW, &type, &form, len, &remain, &list ) != Success ) {

      std::cerr << "winlist() -- GetWinProp" << std::endl;
      return 0;
   }

   return (Window*) list;
}


char* windowName( Display *display, Window window ) {
   /* Returns the name of the window. */

   Atom prop = XInternAtom( display, "WM_NAME", False ), type;
   int form;
   unsigned long remain, len;
   unsigned char *list;

   if ( XGetWindowProperty( display, window, prop, 0, 1024, False, XA_STRING,
                            &type, &form, &len, &remain, &list ) != Success ) {

      std::cerr << "winlist() -- GetWinProp" << std::endl;
      return NULL;
   }

   return (char*) list;
}


Window findEmulatorWindow( Display *display, Window *list, unsigned long &len ) {
   /* Returns the NES emulator window. */

   char* name;
   Window window;

   if (!display) {
      std::cout << "no display!" << std::endl;
      return -1;
   }

   list = (Window*) listWindows( display, &len );

   /* Find gedit window: */
   for ( int i = 0; i < (int) len; i++ ) {

      name = windowName( display, list[i] );
      std::string strName( name );
      std::free( name );

      if ( strName.find("FCEUX") != std::string::npos) {
         window = list[i];
      }
   }

   return window;
}


void focusOnEmulator( Display *display ) {
   /* Set keyboard focus to the FCEUX window. */

   unsigned long len;

   Window *list = (Window*) listWindows( display, &len );
   Window emulator = findEmulatorWindow( display, list, len );

   XSetInputFocus(display, emulator, RevertToNone, CurrentTime);
   XSync(display, False);

   return;
}

