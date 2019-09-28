#pragma once

#include <algorithm>
#include "opencv/cv.h"

using namespace cv;
using namespace std;

class Processing {
	public:
		enum MovementType {
			SWIPE_LEFT  = 0b1100,
			SWIPE_RIGHT = 0b0011,
			SWIPE_UP    = 0b1010,
			SWIPE_DOWN  = 0b0101,
			NO_SWIPE = 0
		};

		enum PinchType {
			EXPAND, PINCH, NO_PINCH
		};

		static MovementType processMovement(vector<Point>::iterator begin, vector<Point>::iterator end, double& distance);
		static PinchType processArea(vector<double>::iterator begin, vector<double>::iterator end, double displacement);
};
