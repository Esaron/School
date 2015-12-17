function [x,t] = readOnlyFaceData()

% read IRIS data

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
for i=1:faceSize
    faceFile = sprintf('faces/train/face/%s', faces(i).name);
    faceImage = double(imread(faceFile));
    x(i,:) = faceImage(:);
    t(i,:) = [1; 0];
end
end