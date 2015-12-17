% Read in face data
[faces faceImages] = readFaceData();

% Get user choices and scores
userFacesAndScores = rateFaces();

% Get principal components, weight the user's choices and get 
% the weighted average eigenface
[coeff, score, latent, weightedAvgEigenface, proportion] = ...
    faceComponents(userFacesAndScores);

% Get eigenfaces for weighted faces
% number of eigenvectors
numEigenVectors = size(score, 2);

% number of eigenfaces set equal to those that account for 90% of
% uniqueness
for i=1:size(proportion)
    if proportion(i) >= 0.9
        numEigenFaces = i;
        break;
    end
end

% size of one side of an image
imSize = 19;

top7Eigenfaces = [];

meanshifted = score/coeff;
eigenvectors = score(:,1:numEigenFaces);
eigenfaces = eigenvectors'*meanshifted;
for j=1:numEigenFaces
    output=[];
    eigenface = eigenfaces(j,:);
    for i=1:imSize
        output=vertcat(output, eigenface(362-(i)*imSize:361-(i-1)*imSize));
    end
    output = mat2gray(output);
    output = im2uint8(output);
    top7Eigenfaces = horzcat(top7Eigenfaces, output');
    imwrite(output', sprintf('eigenface%d%s', j, '.png'));
end
imwrite(top7Eigenfaces, 'eigenfaces.png');

% Find closest matching faces by minimizing sum of squared error
[avgMatches eigenMatches matchIndicesAndSSEs] = ...
    findFaces(faces, weightedAvgEigenface, eigenfaces);
numFaces = size(matchIndicesAndSSEs, 1);

% Find closest matching faces by using SIFT
%matchSIFTIndices = findSIFTFaces(faceImages, weightedAvgEigenface);
%numSIFTFaces = size(matchSIFTIndices, 1);

% Write average eigenface and matched faces to file

% Size of one side of an image
imSize = 19;

% Average eigenface
weightedAvgEigenfaceFormatted=[];
for i=1:imSize
    weightedAvgEigenfaceFormatted = ...
        vertcat(weightedAvgEigenfaceFormatted, ...
        weightedAvgEigenface(362-(i)*imSize:361-(i-1)*imSize));
end
weightedAvgEigenfaceFormatted = uint8(weightedAvgEigenfaceFormatted)';
imwrite(imresize(weightedAvgEigenfaceFormatted, [50 50]), ...
    'weightedAvgEigenface.png');

% Found SSE faces
for j=1:numFaces
    foundFaceIndex = matchIndicesAndSSEs(j, 1);
    foundFace = faces(foundFaceIndex,:);
    foundFaceFormatted = [];
    for i=1:imSize
        foundFaceFormatted = ...
            vertcat(foundFaceFormatted, ...
            foundFace(362-(i)*imSize:361-(i-1)*imSize));
    end
    foundFaceFormatted = uint8(foundFaceFormatted)';
    if j <= avgMatches
        file = sprintf('foundFace%d.png', j);
    else
        file = sprintf('foundFaceEigen%d.png', j-avgMatches);
    end
    imwrite(imresize(foundFaceFormatted, [50 50]), ...
        file);
end

% Found SIFT faces
%for j=1:numSIFTFaces
%    foundFaceIndex = matchSIFTIndices(j, 1);
%    foundFace = faces(foundFaceIndex,:);
%    foundFaceFormatted = [];
%    for i=1:imSize
%        foundFaceFormatted = ...
%            vertcat(foundFaceFormatted, ...
%            foundFace(362-(i)*imSize:361-(i-1)*imSize));
%    end
%    foundFaceFormatted = uint8(foundFaceFormatted)';
%    imwrite(imresize(foundFaceFormatted, [50 50]), ...
%        sprintf('foundSIFTFace%s.png', num2str(j)));
%end
