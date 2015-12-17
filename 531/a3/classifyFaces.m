[x,t] = readFaceData;
x=x';
t=t';
net = patternnet(1)
net.divideparam
net = train(net,x,t);
view(net)
y = net(x);
perf = perform(net,t,y)
classes = vec2ind(y)