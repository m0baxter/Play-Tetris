
#include "scoreReader.hpp"
#include <X11/Xlib.h>
#include <thread>

extern "C" {
   #include <xdo.h>
}

#ifndef TETRIS_PLAYER_HPP
#define TETRIS_PLAYER_HPP

class TetrisPlayer {

   public:
      TetrisPlayer();
      ~TetrisPlayer();
      void pressButton( const int );
      int getScore();
      void newGame();

   private:
      Display* display;
      xdo_t *xdo;
      ScoreReader scoreReader;
      std::thread emulator;

};

#endif

