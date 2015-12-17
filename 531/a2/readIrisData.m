function [dataSet, classes] = readIrisData()

% read IRIS data

clear all
fid = fopen('iris.data');

% 7. Attribute Information:
%   1. sepal length in cm
%   2. sepal width in cm
%   3. petal length in cm
%   4. petal width in cm
%   5. class: 
%      -- Iris Setosa
%      -- Iris Versicolour
%      -- Iris Virginica

D = textscan(fid, '%n%n%n%n%s', 'delimiter',',');

Cidx{1} = find(strcmp(D{5},'Iris-setosa'));
Cidx{2} = find(strcmp(D{5},'Iris-versicolor'));
Cidx{3} = find(strcmp(D{5},'Iris-virginica'));

% Versicolor data for x1, x2, and x0
dataSet{1} = [D{3}(Cidx{2}(1):Cidx{2}(50)), D{4}(Cidx{2}(1):Cidx{2}(50)),...
    ones(50, 1)];
% Virginica data for x1, x2, and x0
dataSet{2} = [D{3}(Cidx{3}(1):Cidx{3}(50)), D{4}(Cidx{3}(1):Cidx{3}(50)),...
    ones(50, 1)];
classes = vertcat(zeros(50, 1), ones(50, 1));
end