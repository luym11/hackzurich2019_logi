#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "BackgroundRemover.h"
#include "SkinDetector.h"
#include "FaceDetector.h"
#include "FingerCount.h"

using namespace cv;
using namespace std;

int rotateImage = 0;

int main(int argc, char**) {
	VideoCapture videoCapture(0);
	videoCapture.set(CV_CAP_PROP_SETTINGS, 1);

	if (!videoCapture.isOpened()) {
		cout << "Can't find camera!" << endl;
		return -1;
	}
	videoCapture.set(CAP_PROP_FRAME_WIDTH, 640);
	videoCapture.set(CAP_PROP_FRAME_HEIGHT, 480);

	Processing::init();

	Mat frame, frameOut, handMask, foreground, fingerCountDebug;

	BackgroundRemover backgroundRemover;
	SkinDetector skinDetector;
	FaceDetector faceDetector;
	FingerCount fingerCount;

	while (true) {
		videoCapture >> frame;
		frameOut = frame.clone();

		skinDetector.drawSkinColorSampler(frameOut);

		foreground = backgroundRemover.getForeground(frame);
		
		faceDetector.removeFaces(frame, foreground);
		handMask = skinDetector.getSkinMask(foreground);
		fingerCountDebug = fingerCount.findFingersCount(handMask, frameOut, rotateImage);

		if (rotateImage) {
			rotate(frameOut, frameOut, ROTATE_180);
			rotate(foreground, foreground, ROTATE_180);
			rotate(handMask, handMask, ROTATE_180);
			rotate(fingerCountDebug, fingerCountDebug, ROTATE_180);
		}

		imshow("output", frameOut);
		imshow("foreground", foreground);
		imshow("handMask", handMask);
		imshow("handDetection", fingerCountDebug);
		
		int key = waitKey(1);

		if (key == 27) // esc
			break;
		else if (key == 98) // b
			backgroundRemover.calibrate(frame);
		else if (key == 115) // s
			skinDetector.calibrate(frame);
		else if (key == 99) // c
			fingerCount.clear();
		else if (key == 114) // r
			rotateImage = !rotateImage;
		else if (key == 104) // h
			fingerCount.toggleHands();

		Processing::performAction();
	}

	return 0;
}
