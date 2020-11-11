# Wavability

Wavability converts gestures to input by using computer vision to track users' hands and map these movements to various input commands. By just waving, the users have a easier and more intuitive way of issuing several commands. 

The implemented functions in HackZurich 2019 were: 
- Identify the back of a hand, and use this as an "undo" command. 
- Identify a fist, and use this as a "redo" command. 

Example scene of Wavability
![](/wavability.png)

An example video can also be found [here](https://youtu.be/qDwLzXr8QXA). 

# Acknowledgements

Forked from [Handy](https://github.com/PierfrancescoSoffritti/handy). Handy is a hand detection software written in C++ using OpenCV v3.4.1. The software is capable of recognizing hands in an video and of counting the number of lifted fingers.

