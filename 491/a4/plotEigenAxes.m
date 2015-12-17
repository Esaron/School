uncorrelated = [1, 0.002; 0.002, 1.7];
correlated = [1, 0.8; 0.8, 1.2];
antiCorrelated = [1, -0.7; -0.7, 3.5];

[ucVectors, ucValues] = eig(uncorrelated);
[cVectors, cValues] = eig(correlated);
[acVectors, acValues] = eig(antiCorrelated);

zeroMu = zeros(1, 2);

figure
plotgauss2d(zeroMu, uncorrelated);
figure
plotgauss2d(zeroMu, correlated);
figure
plotgauss2d(zeroMu, antiCorrelated);