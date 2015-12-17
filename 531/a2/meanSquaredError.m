function[error] = meanSquaredError(dataSet, w, c)

% Init
x = vertcat(dataSet{1}, dataSet{2});
numPatterns = size(x, 1);
ldf = zeros(numPatterns, 1);

% linear discriminant function for the current pattern
% w1*x1(i) + w2*x2(i) + w0
for i=1:numPatterns
    ldf(i, 1) = w(1)*x(i, 1) + w(2)*x(i, 2) + w(3);
end

% Mean Squared Error
% ldf value - c
sse = (ldf - c).^2;
% Times 1/2
error = 0.5*sum(sse);

end