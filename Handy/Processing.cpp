#include "Processing.h"
#include <iostream>

Processing::MovementType Processing::processMovement(vector<Point>::iterator begin, vector<Point>::iterator end, double& distance) {
	Point displacement = *begin - *(end - 1);
	distance = norm(displacement);
	if (distance > 90) {
		double threshold = .5 * distance;
		if (displacement.ddot(Point(1,0)) > threshold) {
			return SWIPE_RIGHT;
		} else if (displacement.ddot(Point(0,1)) > threshold) {
			return SWIPE_UP;
		} else if (displacement.ddot(Point(-1,0)) > threshold) {
			return SWIPE_LEFT;
		}
		return SWIPE_DOWN;
	}
	return NO_SWIPE;
}

Processing::PinchType Processing::processArea(vector<double>::iterator begin, vector<double>::iterator end, double displacement) {
	if (displacement < 80) {
		if (*(end - 1) < .8 * *begin) {
			std::cout << "pinch!\n";
			return PINCH;
		} else if (*(end - 1) > 1.2 * *begin) {
			std::cout << "expand!\n";
			return EXPAND;
		}
	}
	return NO_PINCH;
}
