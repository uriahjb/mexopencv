function FREAK_descriptor
%FREAK_FAST_DETECTOR  feature detector demo
%
% Before start, addpath('/path/to/mexopencv');
%
%{
im1 = imread(fullfile(mexopencv.root(),'test','img001.jpg'));
im1 = cv.cvtColor(im1,'RGB2GRAY');
im2 = fliplr(im1);
%}
im1 = imread('/home/ujb/Class/ESE650/final_project/data/rgbd_dataset_freiburg1_xyz/rgb/1305031102.175304.png');
im2 = imread('/home/ujb/Class/ESE650/final_project/data/rgbd_dataset_freiburg1_xyz/rgb/1305031102.211214.png');
im1 = cv.cvtColor(im1, 'RGB2GRAY');
im2 = cv.cvtColor(im2, 'RGB2GRAY');

threshold = 40;
NOctaves = 4;
patternScale = 22.0;
scaleNormalized = false;
orientationNormalized = false;

tic
[keypoints1, descriptors1] = cv.FREAK_FAST(im1, 'Threshold', threshold, ...
                             'NOctaves', NOctaves, 'patternScale', patternScale, ...
                             'scaleNormalized', scaleNormalized, 'orientationNormalized', orientationNormalized );
[keypoints2, descriptors2] = cv.FREAK_FAST(im2, 'Threshold', threshold, ...
                             'NOctaves', NOctaves, 'patternScale', patternScale, ...
                             'scaleNormalized', scaleNormalized, 'orientationNormalized', orientationNormalized );
toc
size(descriptors1)
size(descriptors2)

% Need to add the SSSE3 matcher but want to test teh brute force one 
% anyways 
% BruteForce matcher is fairly cheap so long as there aren't too many
% descriptors
tic
matcher = cv.DescriptorMatcher('BruteForce-Hamming');
matches = matcher.match(descriptors1,descriptors2);
size(matches)
toc

figure(1)
im_matches = cv.drawMatches(im1, keypoints1, im2, keypoints2, matches);
imshow(im_matches);

figure(2)
imshow(im1);
pts = cat(1,keypoints1.pt);
hold on;
plot( pts(:,1), pts(:,2), '.' );

end
