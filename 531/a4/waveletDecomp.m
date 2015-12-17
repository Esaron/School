function[C, S, image] = waveletDecomp(file)

% Load image
image = imread(file);
image = rgb2gray(image);

% Define wavelet of your choice
wavelet = 'db1';
% Define wavelet decomposition level
level = 2;
% Compute multilevel 2D wavelet decomposition
[C S] = wavedec2(image,level,wavelet);
% Define colormap and set rescale value
map = gray(256);
map = brighten(map, 0.5);
colormap(map); 
rv = length(map);
% Plot wavelet decomposition using square mode
plotwavelet2(C,S,level,wavelet,rv,'square');
title(['Decomposition at level ',num2str(level)]);

end