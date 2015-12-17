function[topX] = calcEigenfaces(x)
% matrix for output
topX = [];

% get face components
[coeff, score] = faceComponents;

% size of one side of an image
imSize = 19;

% average face
avgIm = mean(x, 1);
output=[];
for i=1:imSize
    output=vertcat(output, avgIm(362-(i)*imSize:361-(i-1)*imSize));
end
output = mat2gray(output);
output = im2uint8(output);
imwrite(output', sprintf('avgeigenface.png'));

meanshifted = score/coeff;
eigenvectors = score(:,1:x);
eigenfaces = eigenvectors'*meanshifted;
for j=1:x
    output=[];
    eigenface = eigenfaces(j,:);
    for i=1:imSize
        output=vertcat(output, eigenface(362-(i)*imSize:361-(i-1)*imSize));
    end
    output = mat2gray(output);
    output = im2uint8(output);
    topX = horzcat(topX, output');
    imwrite(output', sprintf('eigenface%d%s', j, '.png'));
end
imwrite(topX, 'eigenfaces.png');
end