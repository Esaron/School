function [FIND_NUM, eigenNum, foundFaceIndices] = ...
    findFaces(faces, averageFace, eigenfaces)

% Number of faces
numFaces = size(faces, 1);

% Number to find
FIND_NUM = 3;
eigenNum = size(eigenfaces, 1);

% Sum Squared Error placeholder
SSEs = zeros(numFaces, 1);
eigenSSEs = zeros(numFaces, eigenNum);

% Output
foundFaceIndices = zeros(FIND_NUM + eigenNum, 2);

% Find least squares difference
for i=1:numFaces
    differences = abs(averageFace - faces(i));
    sumSquaredError = sum(differences.^2);
    SSEs(i) = sumSquaredError;
    for j=1:eigenNum
        differences = abs(eigenfaces(j, :) - faces(i));
        sumSquaredError = sum(differences.^2);
        eigenSSEs(i, j) = sumSquaredError;
    end
end

% Find top FIND_NUM and eigenNum faces with least sum squared error
% It's a weird way of doing it, but I need indices intact for the faces, 
% so I cannot sort first and grab the first FIND_NUM values
for i=1:FIND_NUM
    [minimum index] = min(SSEs);
    SSEs(index) = Inf();
    foundFaceIndices(i, 1) = index;
    foundFaceIndices(i, 2) = minimum;
end
for i=FIND_NUM+1:FIND_NUM+eigenNum
    [minimum index] = min(eigenSSEs(:, i-FIND_NUM));
    foundFaceIndices(i, 1) = index;
    foundFaceIndices(i, 2) = minimum;
end

end