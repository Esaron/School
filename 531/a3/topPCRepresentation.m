function[reducedFaces] = topPCRepresentation(components1, components2)

% original image side
imSize = 19;

% get face components
[coeff, score, latent, prop, x] = faceComponents;

% total components
numComponents = size(latent, 1);

% average face
avgIm = mean(x, 1);

% use first and second images
im1 = x(1,:);
im2 = x(20,:);
score1 = score(1,:);
score2 = score(20,:);
coeff1 = coeff(1,:);
coeff2 = coeff(20,:);

% eliminate last totalPCs-components1 components for im1
for i = components1+1:numComponents
    score1(1,i) = 0;
end
% eliminate last totalPCs-components2 components for im2
for i = components2+1:numComponents
    score2(1,i) = 0;
end

% reduced PCs
reduce1 = score1/coeff1 + avgIm;
reduce2 = score2/coeff2 + avgIm;
reducedim1 = im1 - reduce1;
reducedim2 = im2 - reduce2;

% roll up
output1 = [];
output2 = [];
reducedoutput1 = [];
reducedoutput2 = [];
for i=1:imSize
    output1=vertcat(output1, im1(362-(i)*imSize:361-(i-1)*imSize));
    output2=vertcat(output2, im2(362-(i)*imSize:361-(i-1)*imSize));
    reducedoutput1=vertcat(reducedoutput1, ...
        reducedim1(362-(i)*imSize:361-(i-1)*imSize));
    reducedoutput2=vertcat(reducedoutput2, ...
        reducedim2(362-(i)*imSize:361-(i-1)*imSize));
end
output1 = mat2gray(output1);
output1 = im2uint16(output1);
output2 = mat2gray(output2);
output2 = im2uint16(output2);
reducedoutput1 = mat2gray(reducedoutput1);
reducedoutput1 = im2uint16(reducedoutput1);
reducedoutput2 = mat2gray(reducedoutput2);
reducedoutput2 = im2uint16(reducedoutput2);
reducedFaces = horzcat(output1', output2', reducedoutput1', reducedoutput2');
imwrite(reducedFaces, sprintf('reducedFaces.png'));

end