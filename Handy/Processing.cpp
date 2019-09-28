#include "Processing.h"
#include <iostream>
#include <numeric>      // std::accumulate

Processing::AnalyzedMovement Processing::lastMovement;

void Processing::pressKey(CGKeyCode keycode, bool shift, bool ctrl, bool option, bool cmd) {
    ProcessSerialNumber psn;
    GetFrontProcess(&psn);
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    CGEventRef press = CGEventCreateKeyboardEvent(source, keycode, true);
    CGEventRef release = CGEventCreateKeyboardEvent(source, keycode, false);

    CGEventRef pcmd, rcmd, pshift, rshift, poption, roption, pctrl, rctrl;

    if (cmd) {
    	pcmd = CGEventCreateKeyboardEvent(source, COMMAND, true);
	rcmd = CGEventCreateKeyboardEvent(source, COMMAND, false);
	CGEventPost(kCGHIDEventTap, pcmd);
    }
    if (shift) {
    	pshift = CGEventCreateKeyboardEvent(source, SHIFT, true);
	rshift = CGEventCreateKeyboardEvent(source, SHIFT, false);
	CGEventPost(kCGHIDEventTap, pcmd);
    }
    if (option) {
    	poption = CGEventCreateKeyboardEvent(source, OPTION, true);
	roption = CGEventCreateKeyboardEvent(source, OPTION, false);
	CGEventPost(kCGHIDEventTap, pcmd);
    }
    if (ctrl) {
    	pctrl = CGEventCreateKeyboardEvent(source, CONTROL, true);
	rctrl = CGEventCreateKeyboardEvent(source, CONTROL, false);
	CGEventPost(kCGHIDEventTap, pcmd);
    }

    CGEventPost(kCGHIDEventTap, press);
    CGEventPost(kCGHIDEventTap, release);

    if (cmd) {
    	CGEventPost(kCGHIDEventTap, rcmd);
	CFRelease(pcmd);
	CFRelease(rcmd);
    }
    if (shift) {
    	CGEventPost(kCGHIDEventTap, rshift);
	CFRelease(pshift);
	CFRelease(rshift);
    }
    if (option) {
    	CGEventPost(kCGHIDEventTap, roption);
	CFRelease(poption);
	CFRelease(roption);
    }
    if (ctrl) {
    	CGEventPost(kCGHIDEventTap, rctrl);
	CFRelease(pctrl);
	CFRelease(rctrl);
    }

    CFRelease(press);
    CFRelease(release);
    CFRelease(source);
}

void Processing::performAction() {
	if (Processing::lastMovement.movType) {
		switch (Processing::lastMovement.movType) {
		case SWIPE_LEFT:
			pressKey(0x06, false, false, false, true);
			break;
		case SWIPE_RIGHT:
			pressKey(0x06, true, false, false, true);
			break;
		default:
			break;
		}
	}
}

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
	std::cout << "Average number of fingers detected: "<< std::accumulate(begin, end, 0) / 10 << std::endl;
}
