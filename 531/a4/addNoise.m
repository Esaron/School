function[noisy] = addNoise(inFile, outFile)

% Load image
image = imread(inFile);
image = rgb2gray(image);

noisy = imnoise(image, 'gaussian');

imwrite(noisy, outFile);

end