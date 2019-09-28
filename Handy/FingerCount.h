#pragma once

#include <algorithm>
#include "opencv/cv.h"

#include "Processing.h"

/*
 Author: Nicol� Castellazzi https://github.com/nicast
*/

using namespace std;

class FingerCount {
	public:
		FingerCount(void);
		cv::Mat findFingersCount(cv::Mat input_image, cv::Mat frame, bool rotateImage=false);
	
		void clear();
		void toggleHands();
	private:
		cv::Scalar color_blue;
		cv::Scalar color_green;
		cv::Scalar color_red;
		cv::Scalar color_black;
		cv::Scalar color_white;
		cv::Scalar color_yellow;
		cv::Scalar color_purple;

		vector<cv::Point> centerEvolution;
		vector<double> areaEvolution;
		vector<int> filtered_finger_points_sizes;
		int hands;

		double findPointsDistance(cv::Point a, cv::Point b);
		vector<cv::Point> compactOnNeighborhoodMedian(vector<cv::Point> points, double max_neighbor_distance);
		double findAngle(cv::Point a, cv::Point b, cv::Point c);
		bool isFinger(cv::Point a, cv::Point b, cv::Point c, double limit_angle_inf, double limit_angle_sup, cv::Point palm_center, double distance_from_palm_tollerance);
		vector<cv::Point> findClosestOnX(vector<cv::Point> points, cv::Point pivot);
		double findPointsDistanceOnX(cv::Point a, cv::Point b);
		void drawVectorPoints(cv::Mat image, vector<cv::Point> points, cv::Scalar color, bool with_numbers);
};
