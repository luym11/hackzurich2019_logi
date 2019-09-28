#pragma once

#include <algorithm>
#include "opencv/cv.h"

using namespace cv;
using namespace std;

class Processing {
	public:
		static void processMovement(vector<Point>::iterator begin, vector<Point>::iterator end);
		static void processArea(vector<double>::iterator begin, vector<double>::iterator end);
		static void processFinger(vector<int>::iterator begin, vector<int>::iterator end);
};
