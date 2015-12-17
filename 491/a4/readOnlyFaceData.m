function [x,t] = readOnlyFaceData()

clear all

% get file listings (only allow 999 images for faces)
faces = dir('faces/train/face/face00*.pgm');

% size of filesets
faceSize = size(faces, 1);
imageSize = 19^2;
numClasses = 2;

% init cell arrays
x = zeros(faceSize,imageSize);
t = zeros(faceSize,numClasses);

% read images in filesets into a matrix
% first loop for faces (class 1)
faceImages = [];
for i=1:faceSize
    faceFile = sprintf('faces/train/face/%s', faces(i).name);
    faceImage = imread(faceFile);
    faceImDouble = double(faceImage);
    x(i,:) = faceImDouble(:);
    t(i,:) = [1; 0];
    if mod(i, 100) == 1
        faceImages = horzcat(faceImages, faceImage);
    end
end
imwrite(faceImages, 'faceImages.jpg');
end