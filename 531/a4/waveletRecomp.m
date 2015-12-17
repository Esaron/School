function[C, S, image] = waveletRecomp(file, outFile)

% Load image
image = imread(file);

% Define wavelet of your choice
wavelet = 'db1';
% Define wavelet decomposition level
level = 2;
% Compute multilevel 2D wavelet decomposition
[C S] = wavedec2(image,level,wavelet);

% Remove all but the first level of detail coefficients
S = S(1:2, :);

% Reconstruct image
newImage = waverec2(C, S, 'db1');

newImage = mat2gray(newImage);
newImage = im2uint8(newImage);

% Write
imwrite(newImage, outFile);

end