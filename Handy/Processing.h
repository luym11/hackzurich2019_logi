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

		struct AnalyzedMovement {
			MovementType movType;
			PinchType pinType;
			int visibleFingers;
			double displacement;
		};

		static AnalyzedMovement lastMovement;

		static void processMovement(vector<Point>::iterator begin, vector<Point>::iterator end);
		static void processArea(vector<double>::iterator begin, vector<double>::iterator end);
		static void processFinger(vector<int>::iterator begin, vector<int>::iterator end);
};
