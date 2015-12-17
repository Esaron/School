function[COEFF, SCORE, latent, weightedAvgEigenface, cumProportion] = ...
    faceComponents(facesScores)

% User rated faces row size
scoredFaceSize = size(facesScores, 2);

% And column size
numFaces = size(facesScores, 1);

% Separate faces and scores
faces = facesScores(:, 1:scoredFaceSize-1);
scores = facesScores(:, scoredFaceSize);

% Init weighted face matrix
weightedFaces = [];

% Init score tracker
ctr = 0;

% Create weighted face matrix
for i = 1:numFaces
    for j = 1:scores(i)
        weightedFaces(ctr + j, :) = faces(i, :);
    end
    ctr = ctr + scores(i);
end

weightedAvgEigenface = mean(weightedFaces);

% Get principal components
[COEFF, SCORE, latent] = princomp(weightedFaces);

proportion = latent./sum(latent);
cumProportion = proportion;
for i = 1:size(proportion)
    curProp = proportion(i);
    for j = i+1:size(proportion)
        cumProportion(j) = cumProportion(j) + curProp;
    end
end

end