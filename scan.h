#ifndef SCAN_H
#define SCAN_H
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <map>
#include <unistd.h> //time
#include <thread>

using namespace std;
using namespace cv;


class Scan
{
public:
    Scan();
    vector<Mat> listFrames;

private:


};

#endif // SCAN_H
