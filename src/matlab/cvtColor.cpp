/**
 * @file cvtColor.cpp
 * @brief mex interface for cvtColor
 * @author Kota Yamaguchi
 * @date 2012
 */
#include "mexopencv.hpp"
using namespace std;
using namespace cv;

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
	if (nrhs<2 || ((nrhs%2)!=0) || nlhs>1)
        mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    
	// Argument vector
	vector<MxArray> rhs(prhs,prhs+nrhs);
	
	// Option processing
	int dstCn = 0;
	for (int i=2; i<nrhs; i+=2) {
		string key = rhs[i].toString();
		if (key=="DstCn")
			dstCn = rhs[i+1].toInt();
		else
			mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
	}
	
	// Process
	Mat src(rhs[0].toMat()), dst;
	int code = ColorConv[rhs[1].toString()];
	cvtColor(src, dst, code, dstCn);
	plhs[0] = MxArray(dst);
}
