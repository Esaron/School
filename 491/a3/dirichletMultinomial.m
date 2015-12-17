function[MAPs] = dirichletMultinomial(n)

% Init
theta = [0.05, 0.10, 0.15, 0.20, 0.25, 0.25];
K = size(theta, 2);

outcomes = zeros(1, n);
counts = zeros(1, K);

% Uniform prior
alpha = ones(1, K);

% Simulate n rolls
for i=1:n
    cur = rand(1);
    if cur >= 0 && cur <= 0.05
        outcomes(i) = 1;
        counts(1) = counts(1) + 1;
    elseif cur >= 0.05 && cur < 0.15
        outcomes(i) = 2;
        counts(2) = counts(2) + 1;
    elseif cur >= 0.15 && cur < 0.30
        outcomes(i) = 3;
        counts(3) = counts(3) + 1;
    elseif cur >= 0.30 && cur < 0.50
        outcomes(i) = 4;
        counts(4) = counts(4) + 1;
    elseif cur >= 0.50 && cur < 0.75
        outcomes(i) = 5;
        counts(5) = counts(5) + 1;
    else
        outcomes(i) = 6;
        counts(6) = counts(6) + 1;
    end
end

% Determine likelihood
MAPs = (counts + alpha - 1)./(n + sum(alpha) - K);
bar(MAPs);

end