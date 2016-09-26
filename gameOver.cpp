
#include "gameOver.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


void GameOver :: save( const char* path ) {
   /* Save the trained SVM to disk. */

   svm.save(path);

} 


void GameOver :: load( const char* path ) {
   /* Load a trained SVM from disk. */

   svm.load(path);

}


cv::Mat ScoreReader :: readLabels( const int num, const char* path ) {
   /* Read in the label information in file at path. */

   std::ifstream readFile( path );
   cv::Mat data( num, 1, CV_32SC1 );

   std::string line;
   int i = 0;

   while ( readFile >> line ) {
      
      data.at<int>(i) = std::stoi( line );
      ++i;

   }

   return data;

}


cv::Mat GameOver :: getExample( const int figNum, const std::string dir ) {
   /* Load an prepare a single example screen cap. */

   std::stringstream path;

   char full[200];
   realpath(dir.c_str(), full);

   path << full << "/TETRIS-" << figNum << ".png";

   std::cout << path.str() << std::endl;

   cv::Mat image = cv::imread( path.str().c_str(), CV_LOAD_IMAGE_COLOR );
   cv::cvtColor( image, image, CV_BGR2GRAY );

   return image;

}


cv::Mat GameOver :: genTrainingData( const int figMin, const int figMax, const std::string dir ) {
   /* Generates a matrix from all of the sample images for training SVM. */

   int numRows = figMax - figMin + 1;

   cv::Mat trainingData( numRows, 57344, CV_32F ); //256 * 224 = 57344

   for ( int i = 0; i < numFigs; i++ ) {
      
      cv::Mat image = getExample( i, dir );
      image.reshape(1,1).row(0).copyTo( trainingData.row( i ) );

   }

   return trainingData;

}

