function[It, Ix, Iy] = gaussianLinearMotion(x, y, u, v, t)

gaussianSize = 7;
gaussianMid = ceil(7/2);
h = fspecial('gaussian', [gaussianSize gaussianSize], 1);
initI = h(gaussianMid, gaussianMid);
for i=1:t+1
    x(i+1) = x(i) + u;
    y(i+1) = y(i) + v;
end
It = h(gaussianMid - t*u, gaussianMid - t*v) - initI;
Ix = h(gaussianMid, gaussianMid - (x(t+1)-x(1))) - initI;
Iy = h(gaussianMid - (y(t+1)-y(1)), gaussianMid) - initI;

end