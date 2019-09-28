#include "Processing.h"
#include <iostream>
#include <numeric>      // std::accumulate

Processing::AnalyzedMovement Processing::lastMovement;
Processing::WaitAction Processing::requestedAction = Processing::NULL_ACTION;
int Processing::framesSinceAction = 0, Processing::waitFrames = 7;

void Processing::init() {
	Processing::lastMovement = (AnalyzedMovement) {
		NO_SWIPE, NO_PINCH, 0, 0., false
	};
	Processing::waitFrames = 7;
}

void Processing::clearAction(Processing::WaitAction action) {
	Processing::requestedAction = action;
	Processing::framesSinceAction = 0;
	Processing::waitFrames = 7;
	Processing::lastMovement.movType = NO_SWIPE;
}

void Processing::performAction() {
	WaitAction frameAction = NULL_ACTION;
	if (Processing::lastMovement.movType) {
		switch (Processing::lastMovement.movType) {
		case SWIPE_LEFT:
			frameAction = UNDO;
			break;
		case SWIPE_RIGHT:
			frameAction = REDO;
			break;
		default:
			break;
		}
	}
	if (frameAction == Processing::requestedAction && frameAction != NULL_ACTION) {
		Processing::framesSinceAction++;
	} else {
		Processing::clearAction(frameAction);
		return;
	}
	if (Processing::lastMovement.isLocked) return;
	if (Processing::framesSinceAction >= Processing::waitFrames &&
		Processing::requestedAction != NULL_ACTION) {
		// std:cout << framesSinceAction << std::endl; 
		switch (Processing::requestedAction) {
			case UNDO:
				Keyhandler::pressKey(0x06, false, false, false, true);
				break;
			case REDO:
				Keyhandler::pressKey(0x06, true, false, false, true);
				break;
			default:
				break;
		}
		Processing::framesSinceAction = 0;
		if (Processing::waitFrames > 3) Processing::waitFrames--;
	}
}

void Processing::reverseMovement() {
	Processing::lastMovement.movType = (MovementType)(~Processing::lastMovement.movType & 0b1111);
}

void Processing::processMovement(vector<Point>::iterator begin, vector<Point>::iterator end) {
	if (Processing::lastMovement.isLocked) return;
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
		if (*(end - 1) < .9 * *begin) {
			// std::cout << "pinch!\n";
			lastMovement.pinType = PINCH;
			Processing::lastMovement.isLocked = true;
		} else if (*(end - 1) > 1.1 * *begin) {
			// std::cout << "expand!\n";
			lastMovement.pinType = EXPAND;
			Processing::lastMovement.isLocked = false;
		}
	}
	lastMovement.pinType = NO_PINCH;
}

void Processing::processFinger(vector<int>::iterator begin, vector<int>::iterator end) {
	// std::cout << "Average number of fingers detected: "<< std::accumulate(begin, end, 0) / 10 << std::endl;
}
