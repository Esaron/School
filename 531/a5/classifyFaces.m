function[net] = classifyFaces()

[x,t] = readFaceData;

% image size
imSize = size(x, 2)

% number of images
numIms = size(x, 1);

net = patternnet(5)
net = train(net,x,t);
view(net)
y = net(x);
perf = perform(net,t,y)
classes = vec2ind(y)

end