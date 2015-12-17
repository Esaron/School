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

