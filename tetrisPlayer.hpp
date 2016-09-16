
#include "scoreReader.hpp"
#include <X11/Xlib.h>

extern "C" {
   #include <xdo.h>
}

#ifndef TETRIS_PLAYER_HPP
#define TETRIS_PLAYER_HPP

class TetrisPlayer {

   public:
      //public things:
      TetrisPlayer();
      void pressButton( const int );

   private:
      //private things:
      Display* display;
      xdo_t *xdo;
      ScoreReader scoreReader;

};

#endif

