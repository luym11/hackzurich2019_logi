# Wavability

Wavability converts gestures to input by using computer vision to track users' hands and map these movements to various input commands. By just waving, the users have a easier and more intuitive way of issuing several commands. 

The implemented functions in HackZurich 2019 were: 
- Identify whether the user's fist is opened or closed. Wavability only responds when the user's fist is open, allowing the user to move their hand without inadvertently entering commands.
- Detect the user's hand moving laterally, which is interpreted as either "undo" or "redo", depending on the direction.

Example scene of Wavability
![](/wavability.png)

An example video can also be found [here](https://youtu.be/qDwLzXr8QXA). 

# Acknowledgements

Forked from [Handy](https://github.com/PierfrancescoSoffritti/handy). Handy is a hand detection software written in C++ using OpenCV v3.4.1. The software is capable of recognizing hands in an video and of counting the number of lifted fingers.

