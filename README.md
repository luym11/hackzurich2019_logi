# Handy
Handy is a hands recognition software written in C++ using OpenCV. The software is capable of recognizing hands in an video and of counting the number of lifted fingers.

## Software assumptions
A few assumptions have been made:
1. The camera is supposed to be static.
2. The camera has no automatic regulations, such as auto-focus etc.
3. The user is not moving in the frame (eg: he sits in front of the camera).
4. There are no particular constraints on the color of the background but it should be approximately static (no moving objects/strong changes of illumination in the background).

We decided not to add invasive constraints such as forcing the user to wear gloves, in order to change the color of his hands, or to have a specic illumination in the scene.

___add gif___

## Hand segmentation
With segmentation we refer to the process of extracting objects of interest from an image. In our case, the object of interest is the hand of the user.

There are many possible approaches to solve this problem, each with different complexity and accuracy. Due to the lack of strong constraints on the scene's composition and illumination, we had to exclude segmentation techniques based on thresholding grayscale images. Even though this techniques are generally fast and reliable (with the right images), our typical histogram doesn't have any recognizable separation of modes, therefore grayscale analysis is not a viable option.

We decided that the best approach to solve our problem, in terms of complexity and reliability, is to segment the hand starting from the color of the user's skin. The idea is quite simple: find the color of the user's skin and use it as a threshold to binarize the image.

### How to find the color of the user's skin
In order to find the skin color we decided to designate two specic areas of the screen as "sample areas". The user has to move his hand in the frame so that it covers the two sample areas. When the "S" key is pressend on the keyboard, the program saves the images contained in the sample areas, makes an average of the colors and then uses those two averaged colors as lower and higher thresholds to find the user's skin.

This solution is extremely simple and could be impoved in many ways. For example accuracy could be improved by taking more samples over time, to account for noise and light variations.

The code responsible for this part can be found [here](https://github.com/PierfrancescoSoffritti/Handy/blob/master/Handy/SkinDetector.cpp).

___add sample areas image___

#### Threshold calculation
The image is first converted to the HSV color space. After some testing this color space resulted the most resisted to changes of shade and tonality of the color, but is not the only option, YCrCb was also giving good results. The most signicant advantage we had using HSV was the ability to ignore shadows on the hand, simply by ignoring the third channel.

The samples images are divided in their three channels: H, S and V. For each channel we calculate the mean value, from there low and high threshold values are computed, except for the value channel (V).

After some testing we've found that lowering the minimum thresholds and increasing the higher thresholds by a constant amount yields better results.

```
void SkinDetector::calculateThresholds(Mat sample1, Mat sample2) {
  int offsetLowThreshold = 80;
  int offsetHighThreshold = 30;

  Scalar hsvMeansSample1 = mean(sample1);
  Scalar hsvMeansSample2 = mean(sample2);

  hLowThreshold = min(hsvMeansSample1[0], hsvMeansSample2[0]) - offsetLowThreshold;
  hHighThreshold = max(hsvMeansSample1[0], hsvMeansSample2[0]) + offsetHighThreshold;

  sLowThreshold = min(hsvMeansSample1[1], hsvMeansSample2[1]) - offsetLowThreshold;
  sHighThreshold = max(hsvMeansSample1[1], hsvMeansSample2[1]) + offsetHighThreshold;

  vLowThreshold = 0;
  vHighThreshold = 255;
}
```

#### Binarization
For the binarization of the frame OpenCV's `inRange` function is used. The operator simply sets to 1 all the pixels contained between the low and high thresholds and to 0 all the other pixels.

`inRange(hsvInput, Scalar(hLowThreshold, sLowThreshold, vLowThreshold), Scalar(hHighThreshold, sHighThreshold, vHighThreshold), skinMask);`

After the binarization the image resulted a bit noisy, because of false positives. To clean the image and remove the false positives an opening operator is applied, with a 3x3 circular structuring element.

A dilation is also applied, just in case parts of the hands have been detached after the binarization (sometimes fingers are detached from the hand).

```
performOpening(skinMask, MORPH_ELLIPSE, { 3, 3 });
dilate(skinMask, skinMask, Mat(), Point(-1, -1), 3);

void SkinDetector::performOpening(Mat binaryImage, int kernelShape, Point kernelSize) {
  Mat structuringElement = getStructuringElement(kernelShape, kernelSize);
  morphologyEx(binaryImage, binaryImage, MORPH_OPEN, structuringElement);
}	
```

___add binarized image___

### Remove the user's face
The skin of the user is now the object of our binary image.
Along with the user's hands, the user's face is also part of our object. This is obviosly not desirable. 

To prevent our algorithm from picking up the face of the user, before the binarization of the image the user's face is detected and removed by drawing a black rectagle over it.
The problem of face detection is solved using one of the face classiers provided by OpenCV.

The code responsible for this part can be found [here](https://github.com/PierfrancescoSoffritti/Handy/blob/master/Handy/FaceDetector.cpp).

___add binary image without user face___

### Background removal
At this point the program is working but, due to the unpredictability of the scene conditions, it is not particularly reliable. A simple change of illumination or a background with a color too similar to the color of the user's skin may give a lot of false positives.
In order to solve this problem we decided to add an extra step to our process: background removal, before binarization.

In a first approach we tried to use dynamic background subtraction. Theproblem with this solution is that the hand has to always move, otherwise it is classied as background and then removed.

Considering that this application is supposed to be used indoors, ideally at a desk, we can assume our image to be static (it doesn't change signicantly over time). We decided to save a frame (converted to grayscale), and use it as a reference for background removal.

When the application starts the first frame is saved as reference. Then for each new frame we simply iterate over each pixel of the frame and compare it to the correspondig pixel in the reference frame. If the pixel in the current frame differs by a certain amount from the corresponding pixel in the reference frame it is not removed, otherwise the pixel is classified as background and removed.

More in detail:
* if `bg` is the reference frame and `input` is the current frame.
* If `bg[x,y] - offset <= input[x, y] <= bg[x, y] + offset`, input[x, y] is classified as background and set to 0. Every other pixel will be in the foreground.

```
void BackgroundRemover::removeBackground(Mat input, Mat background) {
  int thresholdOffset = 10;

  for (int i = 0; i < input.rows; i++) {
    for (int j = 0; j < input.cols; j++) {
      uchar framePixel = input.at<uchar>(i, j);
      uchar bgPixel = background.at<uchar>(i, j);
    
      if (framePixel >= bgPixel - thresholdOffset && framePixel <= bgPixel + thresholdOffset)
        input.at<uchar>(i, j) = 0;
      else
        input.at<uchar>(i, j) = 255;
    }
  }
}
```

The code responsible for this part can be found [here](https://github.com/PierfrancescoSoffritti/Handy/blob/master/Handy/BackgroundRemover.cpp).

Background removal, applied before the binarization of the image, gave us good results and high accuracy in most scenes.

Despite working well this approach has many problems and limitation, if the background isn't static or if the illumination of the scene changes, it doesn't work.

To increase the flexibility of our program we assigned a key to the keyboard (the "B" key) that the user can press to replace the reference frame with the current frame. Even if the initial background changes (invalidating the original reference frame), the user can easily take a new sample.

___add image without background___

## Hand and finger detection

### Hand contour
Now that we have the binary image, we use OpenCV's function `findContours` to get the contours of all objects in the image. From these we select the contour with the biggest area. If the binarization of the image has been done correctly, this contour should be the one of our hand.

At this point we look for the smallest convex set containing the hand contour, using the `convexHull` function. We then construct the bounding rectangle of the convex hull. The rectalge can be used to approximate the center of the hand and will be also used to do scale invariant computations.

The code responsible for this part can be found [here](https://github.com/PierfrancescoSoffritti/Handy/blob/master/Handy/FingerCount.cpp).

### Fingers identification
The points of intersection between the hand contour and convex hull are saved in an array, they will be used to locate the finger tips.

Using the `convexityDefects` function, we get all the defects of the contour and we save them in another array. These are the lowest points between one finger and the other. We can use the two arrays to make assumptions about the number of lifted fingers in the image.

`convexityDefects` usually returns more points than we need, therefore we have to filter them. We filter them based on their distance from the center of the bounding rectangle (which approximately corresponds to the center of the hand), so that we keep only the lowest points between each finger. In order to make this process scale invariant, we use the height of the bounding rectangle as reference.

Both arrays (farthest points from the convex hull and closest points to the convex hull) are then filtered again, we need exactly one point for each finger tip and one point between each finger. To do that all points are averaged using a chosen neighborhood. For each point, all the points within a certain distance are averaged to a single point.

Now we can analyze our arrays in order to locate fingers.

For each point in the finger tips array we look in the defects array for the two nearest points on the x axis. We now have three points: one is a fingertip candidate (we aren't sure at this point), the others are "local minimum" representing the concavities of the hand.

![finger tips and convex hull defects](https://raw.githubusercontent.com/PierfrancescoSoffritti/Handy/master/pictures/tips_and_defects.png)

To determine if the point is really a fingertip, we do the following operations:
1. Check that the angle between the three points is within specied limits (usually the angle between the tip of our finger and the two closest concavities is within a certain range).
2. Check that the finger tip is not below the two concavity points (our hand is supposed not to be upside down).
3. Check that the two concavity points are not below the center of the hand (in the case of the thumb and pinkie fingers one point is allowed to be under the center, but both would be anatomically incorrect).
4. Check that the distance between the center of the hand and the finger tip is greater than a chosen limit, scaled with the height of the bounding rectangle (fingers shouldn't be too small or big compared to the size of the hand).
5. To increase accuracy for the case in which we have zero lifted finger, we check that all the concavity points have a minimum distance from the center of the hand. This minimum distance is scaled using the height of the bounding rectangle.

To remove false positives, every point following the fifth fingertip is removed.

The code responsible for this part can be found [here](https://github.com/PierfrancescoSoffritti/Handy/blob/master/Handy/FingerCount.cpp).

___add contour and points image___

## Limitations and possible improvements
* The accuracy of this solution is not the always the best and the program may need to be tuned differently for different environments.
* This sofware has been written with limited knowledge of both OpenCV and C++, there's probably plenty of room for optimizations and performance improvements.
* As mentioned before, the samples of the user's skins are collocted in an exremely simplistic way. There's a lot of room for improvements here.
* Some part of the application relie on a few "magic numbers" (hardcoded constants), it would be ideal to find a way to remove them.
