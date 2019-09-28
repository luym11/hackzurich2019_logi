#include "Processing.h"
#include <iostream>
#include <numeric>      // std::accumulate

Processing::AnalyzedMovement Processing::lastMovement;

void Processing::processMovement(vector<Point>::iterator begin, vector<Point>::iterator end) {
	Point displacement = *begin - *(end - 1);
	Processing::lastMovement.displacement = norm(displacement);
	if (Processing::lastMovement.displacement > 90) {
		double threshold = .5 * Processing::lastMovement.displacement;
		if (displacement.ddot(Point(1,0)) > threshold) {
			lastMovement.movType = SWIPE_RIGHT;
		} else if (displacement.ddot(Point(0,1)) > threshold) {
			lastMovement.movType = SWIPE_UP;
		} else if (displacement.ddot(Point(-1,0)) > threshold) {
			lastMovement.movType = SWIPE_LEFT;
		} else {
			lastMovement.movType = SWIPE_DOWN;
		}
	} else {
		lastMovement.movType = NO_SWIPE;
	}
}

void Processing::processArea(vector<double>::iterator begin, vector<double>::iterator end) {
	if (Processing::lastMovement.displacement < 80) {
		if (*(end - 1) < .8 * *begin) {
			std::cout << "pinch!\n";
			lastMovement.pinType = PINCH;
		} else if (*(end - 1) > 1.2 * *begin) {
			std::cout << "expand!\n";
			lastMovement.pinType = EXPAND;
		}
	}
	lastMovement.pinType = NO_PINCH;
}

void Processing::processFinger(vector<int>::iterator begin, vector<int>::iterator end) {
	if (std::accumulate(begin, end, 0) / 5 > 1) {
		std::cout << "Fingers! " << std::endl; 
	} else {
		std::cout << "NO finger! " << std::endl; 
	}
}