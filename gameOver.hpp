
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

#ifndef GAME_OVER_HPP
#define GAME_OVER_HPP

class GameOver {

   public:
      //Public functions:
      void train( const int, const int, const char*);
      void save( const char* );
      void load( const char* );
      int operator()( const int = 0 );

   private:
      //The SVM:
      CvSVM svm;

      //Private functions:
      cv::Mat readLabels( const int, const char* );
      cv::Mat genTrainingData( const int, const int, const std::string );
      cv::Mat getExample( const int, const std::string );
      float matToScore( const cv::Mat& );
};

#endif

