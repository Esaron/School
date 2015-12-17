function[dw] = summedGradient(dataSet, w, c)

% patterns
x = vertcat(dataSet{1}, dataSet{2});
N = size(x, 1);

% weights
numInputs = size(w, 2);

% init
dw = zeros(1, numInputs);
gradients = zeros(N, 1);

% summed gradient for weight (for each input)
for i=1:numInputs
    for n=1:N
        gradients(n) = (w(1)*x(n, 1) + w(2)*x(n, 2) + w(3)*x(n, 3)...
            - c(n, 1))*x(n, i);
    end
    dw(1, i) = sum(gradients);
end

end