[x,t] = readFaceData;
[COEFF, SCORE] = princomp(x);

% image size
imSize = size(x, 2);

% number of images
numIms = size(x, 1);

% keep first 2 components for data
for i = 1:19;
    SCORE(:,i) = 0;
end

% average face
avgIm = mean(x, 1);

meanShifted = SCORE/COEFF;

% reduced PCs
for i = 1:numIms
    reduced = meanShifted(i, :) + avgIm;
    x(i, :) = x(i, :) - reduced;
end

net = patternnet(1)
net.divideparam
net = train(net,x,t);
view(net)
y = net(x);
perf = perform(net,t,y)
classes = vec2ind(y)