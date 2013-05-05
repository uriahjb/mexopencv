/**
 * @file FREAK_SURF.cpp
 * @brief mex interface for FREAK using SURF keypoint detectors
 * @author Uriah Baalke
 * @date 2013
 */
#include "mexopencv.hpp"
#include <opencv2/nonfree/nonfree.hpp>

//#define TIMING 

#ifdef TIMING
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#endif

using namespace std;
using namespace cv;

namespace {

/// Initialization flag
bool initialized = false;

}


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

    if (!initialized) {
        initModule_nonfree();
        initialized = true;
    }

    // Argument vector
    vector<MxArray> rhs(prhs,prhs+nrhs);

    // return the descriptor size (64/128)?
    if (nrhs==1 && rhs[0].isChar() && rhs[0].toString()=="DescriptorSize") {
        plhs[0] = MxArray(FREAK().descriptorSize());
        return;
    }

    // Option processing


    // SURF options 
    int minHessian  = 1000;

    /*
    // FAST options
    int threshold = 10;
    bool nonmaxSuppression = true;
    */

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
        else if (key=="minHessian")
            minHessian = rhs[i+1].toInt();
        else
            mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
    }

    #ifdef TIMING
    struct timespec tic, toc;
    long seconds, nseconds;
    double mtime;
    #endif

    #ifdef TIMING
    clock_gettime( CLOCK_REALTIME, &tic );
    #endif 
    // Convert uint8 matlab image to cv::Mat 
    Mat image(rhs[0].toMat());
    #ifdef TIMING
    clock_gettime( CLOCK_REALTIME, &toc );
    seconds = toc.tv_sec - tic.tv_sec;
    nseconds = toc.tv_nsec - tic.tv_nsec;
    mtime = (double(seconds)+double(nseconds)/1000000000);
    printf("mxMat to cv::Mat: Elapsed time: %2.10f seconds \n", mtime);
    #endif


    #ifdef TIMING
    clock_gettime( CLOCK_REALTIME, &tic );
    #endif 
    // Calculate keypoints
    vector<KeyPoint> keypoints;
    //FAST( image, keypoints, threshold, nonmaxSuppression );
    SurfFeatureDetector detector( minHessian );
    detector.detect( image, keypoints );

    #ifdef TIMING
    clock_gettime( CLOCK_REALTIME, &toc );
    seconds = toc.tv_sec - tic.tv_sec;
    nseconds = toc.tv_nsec - tic.tv_nsec;
    mtime = (double(seconds)+double(nseconds)/1000000000);
    printf("FAST: Elapsed time: %2.10f seconds \n", mtime);
    #endif
     

    // Process FREAK descriptors
    FREAK freak( orientationNormalized, scaleNormalized, patternScale, nOctaves );
    
    #ifdef TIMING
    clock_gettime( CLOCK_REALTIME, &tic );
    #endif 
    Mat descriptors;
    // Compute FREAK descriptors
    freak.compute( image, keypoints, descriptors );
    
    #ifdef TIMING
    clock_gettime( CLOCK_REALTIME, &toc );
    seconds = toc.tv_sec - tic.tv_sec;
    nseconds = toc.tv_nsec - tic.tv_nsec;
    mtime = (double(seconds)+double(nseconds)/1000000000);
    printf("FREAK: Elapsed time: %2.10f seconds \n", mtime);
    #endif


    if (nlhs>1) {
	Mat m(descriptors);
        plhs[1] = MxArray(m.reshape(0, keypoints.size()));
    }
    plhs[0] = MxArray(keypoints);
}
