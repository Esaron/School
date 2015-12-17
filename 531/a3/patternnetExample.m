[x,t] = iris_dataset;
net = patternnet(3)
net = train(net,x,t);
view(net)
y = net(x);
perf = perform(net,t,y)
classes = vec2ind(y)