function FAST_detector
%FAST_DETECTOR  feature detector demo
%
% Before start, addpath('/path/to/mexopencv');
%

im = imread(fullfile(mexopencv.root(),'test','img001.jpg'));
im = cv.cvtColor(im,'RGB2GRAY');

%{
detector = cv.FeatureDetector('FAST');
keypoints = detector.detect(im);
%}

keypoints = cv.FAST(im, 'Threshold', 40);

im_keypoints = cv.drawKeypoints(im,keypoints);
imshow(im_keypoints);

end
