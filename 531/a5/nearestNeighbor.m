% This function calculates the shortest euclidean distance between
% features in feats1 and feats2 from img1 and img2 by examining the 
% SIFT keys in corresponding keys1 and keys2, then plots the SIFT features
% on img1 whilst showing img2 beside it.  It returns a vector describing
% the average distance between the features to estimate a motion vector.
function[motionVec] =...
    nearestNeighbor(feats1, feats2, keys1, keys2, img1, img2)

numFeats1 = size(keys1, 1);
numFeats2 = size(keys2, 1);

% find indices of smallest distances between smallest keys matrix
% and items in the remaining matrix with euclidean distance metric
if numFeats2 <= numFeats1
    [D I] = pdist2(keys1, keys2, 'euclidean', 'Smallest', 1);
    % feats1 has more features
    maxKeys = numFeats2;
    im1PairI = 2;
    im2PairI = 1;
else
    [D I] = pdist2(keys2, keys1, 'euclidean', 'Smallest', 1);
    % feats2 has more features
    maxKeys = numFeats1;
    im1PairI = 1;
    im2PairI = 2;
end

% compile the paired indices
pairs = zeros(maxKeys, 2);
for i=1:maxKeys
    pairs(i, 1) = i;
    pairs(i, 2) = I(i);
end

% horzcat images
output = horzcat(img1, img2);
% get offset for column indices in img2
img2XOffset = size(img1, 2);

% plot everything and get the average distance in the x and y directions
% between points
distMat = zeros(maxKeys, 2);
figure(1);
imagesc(output)
hold on
colormap gray
for i=1:maxKeys
    % SIFT components
    im1I = pairs(i, im1PairI);
    im2I = pairs(i, im2PairI);
    x1 = feats1(im1I, 1);
    y1 = feats1(im1I, 2);
    size1 = feats1(im1I, 4);
    flag1 = feats1(im1I, 5);
    ang1 = feats1(im1I, 6);
    weight1 = feats1(im1I, 7);
    x2 = feats2(im2I, 1);
    y2 = feats2(im2I, 2);
    size2 = feats2(im2I, 4);
    flag2 = feats2(im2I, 5);
    weight2 = feats2(im2I, 7);
    ang2 = feats2(im2I, 6);
    plotFeats(x1, y1, size1, flag1, weight1, ang1);
    plotFeats(x2, y2, size2, flag2, weight2, ang2);
    %plotFeats(x2 + img2XOffset, y2, size2, flag2, weight2, ang2);
    distMat(i, 1) = x2 - x1;
    distMat(i, 2) = y2 - y1;
end
drawnow;
avgX = mean(distMat(:, 1));
avgY = mean(distMat(:, 2));
motionVec = [avgX, avgY];

end

function[] = plotFeats(x, y, sz, flag, weight, ang)
if flag > 0        %check edge flag
    
    plot(x,y,'g+');         %draw box around real feature
    
    if abs(weight)>1.8
        drawbox(0,sz,x,y,[0 0 1]);
    else
        drawbox(0,sz,x,y,[0 .9 .2]);
    end
else                        %draw as edge
    
    px = [x-sz*cos(ang) x+sz*cos(ang)];
    py = [y-sz*sin(ang) y+sz*sin(ang)];
    plot(px,py,'r');
end
end