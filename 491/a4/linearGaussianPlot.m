muX = [1 2];
muZ = [-1 4];
covX = [1 1.5; 1.5 3];
covZ = [1.4 -0.5; -0.5 1];

x = mvnrnd(muX, covX, 100);
z = mvnrnd(muZ, covZ, 100);

y = x + z;
muY = muX + muZ;
covY = covX + covZ;

meanShiftedX = x(1) - muX;

covXX = cov(x, x);
covXY = cov(x, y);
covYX = cov(y, x);
covYY = cov(y, y);

a = covYX\covXX*(meanShiftedX');
muYgX = muY + a';
covYgX = covYY - covYX\covXX*covXY;

figure
plotgauss2d(muX', covX);
hold on;
scatter(x(:, 1), x(:, 2));
hold off;
figure
plotgauss2d(muZ', covZ);
hold on;
scatter(z(:, 1), z(:, 2));
hold off;
figure
plotgauss2d(muY', covY);
hold on;
scatter(y(:, 1), y(:, 2));
hold off;
figure
plotgauss2d(muYgX', covYgX);
hold on;
scatter(y(:, 1), y(:, 2));
hold off;