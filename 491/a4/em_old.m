% cases
cases = 100;

% simulate data
numClasses = 2;
trueMuX = [-1, 1];
trueSigmaX = [0.6, 0.7; 0.7, 1];
x = mvnrnd(trueMuX, trueSigmaX, cases/numClasses);
trueMuY = [0, 3];
trueSigmaY = [0.3, 0.4; 0.4, 0.6];
y = mvnrnd(trueMuY, trueSigmaY, cases/numClasses);
z = x+y;

% init prior data
likelihood = 0;
p = ones(cases, 2)./cases;
pNgI = mat2cell(p, cases, ones(1, numClasses));
pIgX = pNgI;
mu = mat2cell(zeros(2, numClasses), 2, ones(1, numClasses));
sigma = mat2cell(repmat([1, 0; 0, 1], 1, numClasses), 2,...
    2.*ones(1, numClasses));
maxiter = 500;
converged = false;
t = 1;

while ~converged && t < maxiter
    t = t+1;
    % energy
    for n=1:cases
        for i=1:numClasses
            si = sigma{i};
            pIgX{i} = p(n, i)...
                *(-0.5*(z(n) - mu{i})\si*(z(n) - mu{i})...
                *det(si)^-0.5);
            pIgX{i} = pIgX{i}./sum(pIgX{i});
            pNgI{i}(n) = pIgX{i};
            pNgI{i} = pNgI{i}./sum(pNgI{i});
        end
    end
    for i=1:numClasses
        mu{i} = sum(pNgI{i}*z, 1);
        sigma{i} = sum(pNgI{i}*(z-repmat(mu{i}, cases, 1))...
            *(z-repmat(mu{i}, cases, 1))', 2);
        p(:, i) = (1/numClasses)*sum(pNgI{i});
    end
    for n=1:cases
        for i=1:numClasses
            likelihood = likelihood +...
                p(i)*(1/sqrt(det(2*pi()*sigma{i})))...
                *exp(-0.5*(z(n)-mu{i})'\sigma{i}*(z(n)-mu{i}));
        end
        likelihood = log(likelihood);
    end
end