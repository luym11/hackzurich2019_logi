#pragma once

#include <algorithm>
#include "opencv/cv.h"

#include "Keyhandler.h"

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

		enum WaitAction {
			UNDO, REDO, ESC, ZOOM_IN, ZOOM_OUT, NULL_ACTION
		};

		struct AnalyzedMovement {
			MovementType movType;
			PinchType pinType;
			int visibleFingers;
			double displacement;
			bool isLocked;
		};

		static AnalyzedMovement lastMovement;
		static int framesSinceAction, waitFrames;
		static WaitAction requestedAction;

		static void init();

		static void processMovement(vector<Point>::iterator begin, vector<Point>::iterator end);
		static void processArea(vector<double>::iterator begin, vector<double>::iterator end);
		static void processFinger(vector<int>::iterator begin, vector<int>::iterator end);
		static void reverseMovement();
		static void clearAction(WaitAction action = NULL_ACTION);

		static void performAction(); 
};
