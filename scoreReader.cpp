
#include "scoreReader.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


void ScoreReader :: saveReader( const char* path ) {
   /* Save the trained SVM to disk. */

   svm.save(path);

} 


void ScoreReader :: loadReader( const char* path ) {
   /* Load a trained SVM from disk. */

   svm.load(path);

}


cv::Mat ScoreReader :: readLabels( const int num, const char* path ) {
   /* Read in the label information in file at path. */

   std::ifstream readFile( path );
   cv::Mat data( num*6, 1, CV_32SC1 );

   std::string line;
   int i = 0;

   while ( readFile >> line ) {
      for (int j = 0; j < 6; j++) {

         data.at<int>(i+j) = std::stoi( line.substr(j, 1) );
      }

      i += 6;
   }

   return data;

}


std::vector<cv::Mat> ScoreReader :: getScoreDigits( const int figNum, const std::string dir ) {
   /* Parses a Tetris screenshot into six images of the digits of the score. */

   cv::Mat image;
   std::stringstream path;
   std::vector<cv::Mat> digits;

   char full[200];
   realpath(dir.c_str(), full);

   path << full << "/TETRIS-" << figNum << ".png";

   std::cout << path.str() << std::endl;

   image = cv::imread( path.str().c_str(), CV_LOAD_IMAGE_COLOR);
   cv::cvtColor(image, image, CV_BGR2GRAY);

   for ( int i = 0; i < 6; i++ ) {

      cv::Rect box( cv::Point(191 + i*8, 55), cv::Point(200 + i*8, 64) );
      cv::Mat cropped( image, box );

      cv::Mat copyCropped;
      cropped.copyTo(copyCropped);

      digits.push_back( copyCropped );
   }

   return digits;
}


cv::Mat ScoreReader :: genTrainingData( const int figMin, const int figMax, const std::string dir ) {
   /* Generates a matrix from all of the sample images for training SVM. */

   int numFigs = figMax - figMin + 1;
   int numRows = 6 * numFigs;

   cv::Mat trainingData( numRows, 81, CV_32F );

   for ( int i = 0; i < numFigs; i++ ) {

      std::vector<cv::Mat> digits = getScoreDigits( i + figMin, dir );

      for ( int j = 0; j < 6; j++ ) {

         digits[j].reshape(1,1).row(0).copyTo( trainingData.row( 6*i + j ) );
      }
   }

   return trainingData;

}


float ScoreReader :: matToScore( const cv::Mat &digits ) {
   /* Takes a 6x1 matrix of digits and calculates the score. */

   int tens[6] = { 100000, 10000, 1000, 100, 10, 1 };

   int score = 0;

   for ( int i = 0; i < 6; i++ ) {

      score += tens[i] * ( (int) digits.at<float>(i) );
   }

   return score;

}


void ScoreReader :: trainReader( const int figMin, const int figMax, const char* labelPath ) {
   /* Trains the SVM on the screen shots numbered figMin - figMax with labels given by
    * file at labelPath. */

   cv::Mat data = genTrainingData( figMin, figMax, "./scoreData" );
   cv::Mat labels = readLabels( figMax - figMin + 1, labelPath );

   CvSVMParams params;
   params.svm_type = CvSVM::C_SVC;
   params.kernel_type = CvSVM::LINEAR;

   svm.train( data, labels, cv::Mat(), cv::Mat(), params);

}


int ScoreReader :: operator()( const int figNum ) {
   /* Reads in the score from the screenshot labeled by figNum. */

   cv::Mat results;
   cv::Mat test = genTrainingData( figNum, figNum, "/home/baxter/.fceux/snaps" );
   svm.predict( test, results );

   return matToScore(results);

}

