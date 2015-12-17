function [x, gray] = readFaceData()

% get file listings
faces = dir('faceSet/*.pgm');

% size of filesets
faceSize = size(faces, 1);
imageSize = 19^2;

% init cell arrays
x = zeros(faceSize,imageSize);
gray = cell(1);

% read images in filesets into a matrix
% first loop for faces (class 1)
for i=1:faceSize
    faceFile = sprintf('faceSet/%s', faces(i).name);
    faceGray = imread(faceFile);
    gray{i} = faceGray;
    faceImage = double(faceGray);
    x(i,:) = faceImage(:);
end
end