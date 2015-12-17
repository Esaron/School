% matrix for output
avgAndTop7 = [];

% get face components
[coeff, score, latent, prop, x] = faceComponents;

% number of eigenvectors
numEigenVectors = size(score, 2);

% number of eigenfaces
numEigenFaces = 7;

% size of one side of an image
imSize = 19;

% average face
avgIm = mean(x, 1);
output=[];
for i=1:imSize
    output=vertcat(output, avgIm(362-(i)*imSize:361-(i-1)*imSize));
end

avgAndTop7 = horzcat(avgAndTop7, uint8(output)');
imwrite(uint8(output)', 'eigenfaceAvg.png');

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
    avgAndTop7 = horzcat(avgAndTop7, output');
    imwrite(output', sprintf('eigenface%d%s', j, '.png'));
end
imwrite(avgAndTop7, 'eigenfaces.png');