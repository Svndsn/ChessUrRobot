
#include "ChessMoveDetector.h"
#include <vector>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;





ChessMoveDetector::ChessMoveDetector(int _x, int _y, int _width, int _height) : x(_x), y(_y), width(_width), height(_height){

    baseImage = takePicture();
    std::cout << baseImage.size() << std::endl;
    roi = Rect(x, y, width, height); 
    baseImage = baseImage(roi); 
    rotate(baseImage, baseImage, ROTATE_90_COUNTERCLOCKWISE); //ÆNDR ROTATION HER
    //showGrid(baseImage);
    //imshow("baseImage", baseImage);
    //waitKey(0);
    //destroyAllWindows();
}


void ChessMoveDetector::showGrid(Mat image1){ 

        int grid_size = int(image1.size().width/8);
        for(int i = 0; i < 8; i++){
            for(int j= 0; j < 8; j++){
                cv::rectangle(image1, cv::Rect(i * grid_size, j * grid_size, grid_size, grid_size), (0, 255, 0), 2);
            }
        }
            
}

Mat ChessMoveDetector::fen2Matrix(std::string fen){

    Mat matrix = cv::Mat::zeros(8, 8, CV_8UC1);
    std::string pieces = fen.substr(0, fen.find(' '));
    int row = 0;
    int col = 0;

    for(int i = 0; i < pieces.size(); i++){

        if(pieces[i] == '/'){
            row++;
            col = 0;
        }

        else if(pieces[i]<64){ //64 -> int(8) = 56
            col+= ((int)pieces[i]-48); //48 -> int(1) =
        }

        else if((int(pieces[i]) < 97)){
            matrix.at<uchar>(col,row) = 1;
            col++;
        }

        else{
           matrix.at<uchar>(col,row) = 2;
           col++; 
        }
    }
    
    flip(matrix, matrix,1);
    return matrix;
}

bool ChessMoveDetector::fen2turn(std::string fen){
    std::string toMove = fen.substr(fen.find(' ')+1,1);
    if(toMove == "w"){
        return true;
    }
    else{
        return false;
    }
}

Mat ChessMoveDetector::takePicture(){
    VideoCapture cam(4, CAP_V4L2);
    
    Mat image;
    bool ret = cam.read(image);
    cam.release(); 

    return image;
}

void ChessMoveDetector::takePictureAfterRobot() {
    baseImage = takePicture();
    Mat croppedBaseImage = baseImage(roi);
    rotate(croppedBaseImage, baseImage, ROTATE_90_COUNTERCLOCKWISE);
}

std::string ChessMoveDetector::detectMove(std::string fen) {
    
    
    Mat board_mat = fen2Matrix(fen);
    bool isWhite = fen2turn(fen);

    //Running
    Mat image1 = baseImage;
    Mat image2 = takePicture();
    Mat croppedImage2 = image2(roi); 
    image2 = croppedImage2;  

    rotate(image2, image2, ROTATE_90_COUNTERCLOCKWISE);


    //showGrid(image1.size(), image1);
    //imshow("test",image1);
    //waitKey(0);

    if (image1.empty() || image2.empty()) {
        return "Error: could not load one or both images\n";
    }

    
    Mat diffImage;

    absdiff(image1, image2, diffImage);

    cvtColor(diffImage, diffImage, COLOR_BGR2GRAY);

    threshold(diffImage, diffImage, 30, 255, THRESH_BINARY);

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));

    erode(diffImage, diffImage, kernel);

    std::vector<std::vector<Point>> contours;
    findContours(diffImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    double largestArea1 = 0, largestArea2 = 0;
    int largestContourIndex1 = -1, largestContourIndex2 = -1;
    Rect largestContourRect1, largestContourRect2; 
    for (int i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area > largestArea1) {
            largestArea2 = largestArea1;
            largestContourIndex2 = largestContourIndex1;
            largestArea1 = area;
            largestContourIndex1 = i;
        } else if (area > largestArea2) {
            largestArea2 = area;
            largestContourIndex2 = i;
        }
    }

    if (largestContourIndex1 != -1) {
        largestContourRect1 = boundingRect(contours[largestContourIndex1]);
        rectangle(image1, largestContourRect1, Scalar(0, 0, 255), 2);
    }
    if (largestContourIndex2 != -1) {
        largestContourRect2 = boundingRect(contours[largestContourIndex2]);
        rectangle(image1, largestContourRect2, Scalar(0, 0, 255), 2);
    }

    //imshow("Differences", image1);
    //waitKey(0);

    cv::Size2f dimensions = image1.size();    

    cv::Point diff1pos, diff2pos;
    diff1pos.x = ((largestContourRect1.x + (largestContourRect1.width / 2)) / dimensions.width) * 8;
    diff1pos.y = ((largestContourRect1.y + (largestContourRect1.height / 2)) / dimensions.width) * 8;
    diff2pos.x = ((largestContourRect2.x + (largestContourRect2.width / 2)) / dimensions.width) * 8;
    diff2pos.y = ((largestContourRect2.y + (largestContourRect2.height / 2)) / dimensions.width) * 8;


    showGrid(image1);
    cv::rectangle(image1, cv::Rect(diff1pos.x * (dimensions.height / 8), diff1pos.y * (dimensions.height / 8), dimensions.height / 8, dimensions.height / 8), cv::Scalar(255, 0, 0), 2);
    cv::rectangle(image1, cv::Rect(diff2pos.x * (dimensions.height / 8), diff2pos.y * (dimensions.height / 8), dimensions.height / 8, dimensions.height / 8), cv::Scalar(255, 0, 0), 2);
    cv::rectangle(image1, cv::Rect(0, 0, dimensions.height / 8, dimensions.height / 8), cv::Scalar(255, 255, 255), 2);
    cv::rectangle(image1, cv::Rect(0, dimensions.width, dimensions.height / 8, dimensions.height / 8), cv::Scalar(0, 255, 255), 2);

    std::vector<Point> positions = {diff1pos, diff2pos};
    
/*     imshow("Result", image1);
    waitKey(0);
    destroyAllWindows();  */
    
    std::map<int, std::string> chessConversions_back = {{0, "a"}, {1, "b"}, {2, "c"}, {3, "d"}, {4, "e"}, {5, "f"}, {6, "g"}, {7, "h"}};
    std::string from_square = "";
    std::string to_square = "";

    for (auto pos : positions) {
        uchar currentColor = isWhite ? 1 : 2;        
        if ((int(board_mat.at<uchar>(pos.y, pos.x))) == (int)currentColor) {
            from_square = chessConversions_back[pos.y] + std::to_string(pos.x + 1);

        }
        else {
            to_square = chessConversions_back[pos.y] + std::to_string(pos.x +1);
            
        } 
        
    }
    baseImage = image2;
    //if((from_square + to_square).size() < 4){ return "None";} //Error handling?
    return from_square + to_square;
}
