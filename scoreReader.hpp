
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

#ifndef SCORE_READER_HPP
#define SCORE_READER_HPP

class ScoreReader {

   public:
      //Public functions:
      void trainReader( const int, const int, const char*);
      void saveReader( const char* );
      void loadReader( const char* );
      int operator()( const int = 0 );

   private:
      //The SVM:
      CvSVM svm;

      //Private functions:
      cv::Mat readLabels( const int, const char* );
      std::vector<cv::Mat> getScoreDigits( const int );
      cv::Mat genTrainingData( const int, const int );
      float matToScore( const cv::Mat& );
};

#endif

