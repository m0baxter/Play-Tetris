
#include "scoreReader.hpp"
#include <iostream>


int main() {

   ScoreReader reader1;
   reader1.trainReader( 0, 213, "/home/baxter/.fceux/snaps/scores.txt" );

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

   return 0;

}

