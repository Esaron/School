function [x,y,t] = readFaceData()

% read IRIS data

clear all

% get file listings
faces = dir('faces/train/face/face000*.pgm');
nonfaces = dir('faces/train/non-face/B1_000*.pgm');

% size of filesets
faceSize = size(faces, 1);
nonFaceSize = size(nonfaces, 1);
total = faceSize + nonFaceSize;
imageSize = 19^2;
numClasses = 2;

% init cell arrays
x = zeros(imageSize,faceSize);
y = zeros(imageSize,nonFaceSize);
t = zeros(numClasses,total);

% read images in filesets into a matrix
% first loop for faces (class 1)
for i=1:faceSize
    faceFile = sprintf('faces/train/face/%s', faces(i).name);
    faceImage = double(imread(faceFile));
    x(:,i) = faceImage(:);
    t(:,i) = [1; 0];
end
% second loop for nonfaces (class 0)
for j=1:nonFaceSize
    currentIndex = j+faceSize;
    nonfaceFile = sprintf('faces/train/non-face/%s', nonfaces(j).name);
    nonfaceImage = double(imread(nonfaceFile));
    y(:,j) = nonfaceImage(:);
    t(:,currentIndex) = [0; 1];
end
end