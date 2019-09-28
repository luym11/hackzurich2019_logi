#include "Processing.h"
#include <iostream>

void Processing::processMovement(vector<Point>::iterator begin, vector<Point>::iterator end) {
	Point displacement = *begin - *(end - 1);
	double dispLen = norm(displacement);
	//std::cout << "displacement: " << dispLen << "\n";
}

void Processing::processArea(vector<double>::iterator begin, vector<double>::iterator end) {
	if (*(end - 1) < .8 * *begin) {
		std::cout << "pinch!\n";
	}
}
