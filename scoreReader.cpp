
//  g++-5 -std=c++11 readScore.cpp -o readScore.exe `pkg-config --cflags --libs opencv`

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


std::string GetMatType(const cv::Mat& mat) {

   const int mtype = mat.type();

   switch (mtype) {

      case CV_8UC1:  return "CV_8UC1";
      case CV_8UC2:  return "CV_8UC2";
      case CV_8UC3:  return "CV_8UC3";
      case CV_8UC4:  return "CV_8UC4";

      case CV_8SC1:  return "CV_8SC1";
      case CV_8SC2:  return "CV_8SC2";
      case CV_8SC3:  return "CV_8SC3";
      case CV_8SC4:  return "CV_8SC4";

      case CV_16UC1: return "CV_16UC1";
      case CV_16UC2: return "CV_16UC2";
      case CV_16UC3: return "CV_16UC3";
      case CV_16UC4: return "CV_16UC4";

      case CV_16SC1: return "CV_16SC1";
      case CV_16SC2: return "CV_16SC2";
      case CV_16SC3: return "CV_16SC3";
      case CV_16SC4: return "CV_16SC4";

      case CV_32SC1: return "CV_32SC1";
      case CV_32SC2: return "CV_32SC2";
      case CV_32SC3: return "CV_32SC3";
      case CV_32SC4: return "CV_32SC4";

      case CV_32FC1: return "CV_32FC1";
      case CV_32FC2: return "CV_32FC2";
      case CV_32FC3: return "CV_32FC3";
      case CV_32FC4: return "CV_32FC4";

      case CV_64FC1: return "CV_64FC1";
      case CV_64FC2: return "CV_64FC2";
      case CV_64FC3: return "CV_64FC3";
      case CV_64FC4: return "CV_64FC4";

      default:
         return "Invalid type of matrix!";
   }
}






cv::Mat readLabels( const int num, const char* path ) {
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


std::vector<cv::Mat> getScoreDigits( const int figNum ) {
   /* Parses a Tetris screenshot into six images of the digits of the score. */

   cv::Mat image;
   std::stringstream path;
   std::vector<cv::Mat> digits;

   path << "/home/baxter/.fceux/snaps/TETRIS-" << figNum << ".png";

   std::cout << path.str().c_str() << std::endl;

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


cv::Mat genTrainingData( const int figMin, const int figMax ) {
   /* Generates a matrix from all of the sample images for training SVM. */

   int numFigs = figMax - figMin + 1;
   int numRows = 6 * numFigs;

   cv::Mat trainingData( numRows, 81, CV_32F );

   for ( int i = 0; i < numFigs; i++ ) {

      std::vector<cv::Mat> digits = getScoreDigits( i + figMin );

      for ( int j = 0; j < 6; j++ ) {

         digits[j].reshape(1,1).row(0).copyTo( trainingData.row( 6*i + j ) );
      }
   }

   return trainingData;

}


int main() {

   cv::Mat data = genTrainingData( 0, 213 );
   cv::Mat labels = readLabels( 214, "/home/baxter/.fceux/snaps/scores.txt" );

   //Select SVM parameters:
   CvSVMParams params;
   params.svm_type = CvSVM::C_SVC;
   params.kernel_type = CvSVM::LINEAR;
   //params.term_crit = cvTermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, FLT_EPSILON );

   //Create SVM:
   CvSVM SVM;
   std::cout << SVM.train( data, labels, cv::Mat(), cv::Mat(), params) << std::endl;
   std::cout << "true:  " << true << std::endl;
   std::cout << "false: " << false << std::endl;

   cv::Mat results;
   cv::Mat test = genTrainingData( 214, 214 );

   SVM.predict( test, results );

   std::cout << results.rows << "  " << results.cols << std::endl;

   for ( int i = 0; i < 6; i++ ) {

      std::cout << results.at<float>(i) << std::endl;
   
   }

   std::cout << results << std::endl;
   std::cout << "Type: " << GetMatType( results ) << std::endl;

   return 0;

}

