function[grayscaleFrames] = getFrames(fileName, startFrame, endFrame)

vidReader = VideoReader(fileName);
frames = read(vidReader, [startFrame endFrame]);
numFrames = size(frames, 4);
grayscaleFrames = zeros(720, 1280, 2);
grayscaleFrames(:, :, 1) = rgb2gray(frames(:, :, :, 1));
grayscaleFrames(:, :, 2) = rgb2gray(frames(:, :, :, numFrames));

for i=1:2
    imwrite(imresize(mat2gray(grayscaleFrames(:, :, i)),[480 640]), strcat('frame',...
        num2str(i),'.jpg'));

end