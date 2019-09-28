#pragma once

#include <algorithm>
#include "opencv/cv.h"

#include <ApplicationServices/ApplicationServices.h>

#include <stdarg.h>

using namespace cv;
using namespace std;

class Processing {
		const CGKeyCode COMMAND = 0x37;
		const CGKeyCode SHIFT = 0x38;
		const CGKeyCode OPTION = 0x3A;
		const CGKeyCode CONTROL = 0x3B;

		void pressKey(CGKeyCode keycode, bool shift=false, bool ctrl=false, bool option=false, bool cmd=false);
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

		static void performAction();
};
