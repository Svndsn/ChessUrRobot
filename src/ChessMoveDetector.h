#pragma once
#include<iostream>
#include<vector>
#include <opencv2/opencv.hpp>
using namespace cv;

class ChessMoveDetector
{
private:
    Mat baseImage;
    int x, y, width, height;
    Rect roi;
    Mat takePicture();
    Mat fen2Matrix(std::string fen);
    bool fen2turn(std::string fen);

public:
    ChessMoveDetector(int _x, int _y, int _width, int _height);
    void takePictureAfterRobot();
    std::string detectMove(std::string fen);
};



