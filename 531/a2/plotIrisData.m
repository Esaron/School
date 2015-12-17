function[] = plotIrisData(dataSet, w, name)

% init for save
f = figure();

% Pull in iris data
x1 = dataSet{1};
x2 = dataSet{2};

% Plot versicolor length=x1 width=x2
scatter(x1(:, 1), x1(:, 2));
% Continue plotting points
hold on;
% Plot virginica length=x1 width=x2
scatter(x2(:, 1), x2(:, 2));

% Decision boundary
refline([w(1)/-w(2), w(3)/-w(2)]);

% Axis scale, labels, and plot legend
axis([0 8 0 3]);
xlabel('Petal length (cm)');
ylabel('Petal width (cm)');
legend('Iris-versicolor', 'Iris-virginica');

% save
saveas(f, name);
end