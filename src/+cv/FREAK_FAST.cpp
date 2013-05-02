/**
 * @file FREAK_FAST.cpp
 * @brief mex interface for FREAK using FAST keypoint detectors
 * @author Uriah Baalke
 * @date 2013
 */
#include "mexopencv.hpp"

using namespace std;
using namespace cv;

/*
namespace {

/// Initialization flag
bool initialized = false;

}
*/


/**
 * Main entry called from Matlab
 * @param nlhs number of left-hand-side arguments
 * @param plhs pointers to mxArrays in the left-hand-side
 * @param nrhs number of right-hand-side arguments
 * @param prhs pointers to mxArrays in the right-hand-side
 */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
    // Check the number of arguments
    if (nrhs<1 || ((nrhs%2)!=1) || nlhs>2)
        mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");

    /*
    if (!initialized) {
        initModule_nonfree();
        initialized = true;
    }
    */

    // Argument vector
    vector<MxArray> rhs(prhs,prhs+nrhs);

    // return the descriptor size (64/128)?
    if (nrhs==1 && rhs[0].isChar() && rhs[0].toString()=="DescriptorSize") {
        plhs[0] = MxArray(FREAK().descriptorSize());
        return;
    }

    // Option processing

    // FAST options
    int threshold = 10;
    bool nonmaxSuppression = true;

    // FREAK options
    bool orientationNormalized = true;
    bool scaleNormalized = true;
    float patternScale = 22.0;
    int nOctaves=4;

    Mat mask;
    for (int i=1; i<nrhs; i+=2) {
        string key = rhs[i].toString();
        if (key=="orientationNormalized")
            orientationNormalized = rhs[i+1].toBool();
	else if (key=="scaleNormalized")
            scaleNormalized = rhs[i+1].toBool();
	else if (key=="patternScale")
	    patternScale = rhs[i+1].toFloat();
        else if (key=="NOctaves")
            nOctaves = rhs[i+1].toInt();
	else if (key=="Threshold")
	    threshold = rhs[i+1].toInt();
        else if (key=="NonMaxSuppression")
            nonmaxSuppression = rhs[i+1].toBool();
        else
            mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
    }

    // Convert uint8 matlab image to cv::Mat 
    Mat image(rhs[0].toMat());

    // Calculate keypoints
    vector<KeyPoint> keypoints;
    FAST( image, keypoints, threshold, nonmaxSuppression );

    // Process FREAK features
    FREAK freak( orientationNormalized, scaleNormalized, patternScale, nOctaves );

    Mat descriptors;
    freak.compute( image, keypoints, descriptors );

    if (nlhs>1) {
	Mat m(descriptors);
        plhs[1] = MxArray(m.reshape(0, keypoints.size()));
    }
    plhs[0] = MxArray(keypoints);
}
