function[features1, features2, keys1, keys2, img1, img2] = plotSIFT()

img1 = imread('frame1.jpg');

% Detect the SIFT features:
fprintf(1,'Computing the SIFT features for frame1.jpg...\n')
[features1,pyr1,imp1,keys1] = detect_features(img1);

figure(1);
showfeatures(features1,img1);
title('SIFT features of image frame1.jpg');
drawnow;

img2 = imread('frame2.jpg');

% Detect the SIFT features:
fprintf(1,'Computing the SIFT features for frame2.jpg...\n')
[features2,pyr2,imp2,keys2] = detect_features(img2);

figure(2);
showfeatures(features2,img2);
title('SIFT features of image frame2.jpg');
drawnow;

end