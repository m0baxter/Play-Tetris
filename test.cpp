
#include "scoreReader.hpp"
#include "tetrisPlayer.hpp"
#include <iostream>
#include <random>

#include <exception>

void testReader() {

   ScoreReader reader1;
   reader1.trainReader( 0, 213, "./scoreData/scores.txt" );

   for ( int i = 214; i <= 217; i++ ) {
      
      std::cout << "Score: " << reader1(i) << std::endl;

   }

   reader1.saveReader( "test.txt" );

   std::cout << std::endl;

   ScoreReader reader2;
   reader2.loadReader( "test.txt" );

   for ( int i = 214; i <= 217; i++ ) {
      
      std::cout << "Score: " << reader2(i) << std::endl;

   }

   return;

}


void testPlayer() {

   TetrisPlayer player;

   std::cout << "here" << std::endl;

   std::mt19937 rnd;
   rnd.seed( std::random_device()() );
   std::uniform_int_distribution<std::mt19937::result_type> dist(1,6);

   player.newGame();

   while (true) {
      player.pressButton(dist(rnd));
      //std::cout << player.getScore() << std::endl;
      sleep(3);
   }

   return;

}


int main() {

   try {
      //testReader();
      testPlayer();
   }
   catch (std::exception &except) {
      //std::cout << "Exception caught: " << except.what() << std::endl;
      return 1;
   }

   return 0;

}

