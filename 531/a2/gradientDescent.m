function[wNew] = gradientDescent(dataSet, w, c)

% patterns
x = vertcat(dataSet{1}, dataSet{2});
numPatterns = size(x, 1);

% inputs
wNew = w;

% step size
E = 0.01/numPatterns;

% stopping point
stop = 75;

% init error
error = meanSquaredError(dataSet, wNew, c);
iteration = 1;
xData(1) = iteration
yData(1) = error

% first plot
plotIrisData(dataSet, wNew, 'gdFirst.png');
plotGeneric(xData, yData, 'errorFirst.png');

% gradient descent
while (error > stop)
    iteration = iteration + 1;
    dw = summedGradient(dataSet, wNew, c);
    wNew = wNew - E.*dw;
    error = meanSquaredError(dataSet, wNew, c);
    xData(iteration) = iteration;
    yData(iteration) = error;
    % plot refline and error (don't plot on last iteration)
    if (error > stop)
        plotIrisData(dataSet, wNew, 'gdMid.png');
        plotGeneric(xData, yData, 'errorMid.png');
    else
        % last plot
        plotIrisData(dataSet, wNew, 'gdLast.png');
        plotGeneric(xData, yData, 'errorLast.png');
    end
end

end