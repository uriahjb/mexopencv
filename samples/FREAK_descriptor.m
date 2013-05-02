function SURF_descriptor
%FREAK_FAST_DETECTOR  feature detector demo
%
% Before start, addpath('/path/to/mexopencv');
%

im1 = imread(fullfile(mexopencv.root(),'test','img001.jpg'));
im1 = cv.cvtColor(im1,'RGB2GRAY');
im2 = fliplr(im1);

threshold = 40;

[keypoints1, descriptors1] = cv.FREAK_FAST(im1, 'Threshold', threshold);
[keypoints2, descriptors2] = cv.FREAK_FAST(im2, 'Threshold', threshold);

% Need to add the SSSE3 matcher 
%matcher = cv.DescriptorMatcher('BruteForce');
%matches = matcher.match(descriptors1,descriptors2);

%im_matches = cv.drawMatches(im1, keypoints1, im2, keypoints2, matches);
%imshow(im_matches);

end
