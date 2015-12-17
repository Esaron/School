function [foundFaceIndices] = findSIFTFaces(faces, averageFace)

% Number of faces
numFaces = size(faces, 1);

% Number to find
FIND_NUM = 10;

% Output
foundFaceIndices = zeros(FIND_NUM, 1);

% Find least squares difference
for i=1:numFaces
    sift = compareSIFT(faces{i}, averageFace);
    SSEs(i) = nearestNeighbor(sift);
end

% Find top FIND_NUM faces with least sum squared error
% It's a weird way of doing it, but I need indices intact for the faces, 
% so I cannot sort first and grab the first FIND_NUM values
for i=1:FIND_NUM
    [minimum index] = min(SSEs);
    SSEs(index) = Inf();
    foundFaceIndices(i, 1) = index;
    foundFaceIndices(i, 2) = minimum;
end

end