function[] = plotFrames(fileName)

frames = getFrames(fileName);
numFrames = size(frames, 3);
sideBySide = [];
for i=1:numFrames
    sideBySide = horzcat(sideBySide, mat2gray(frames(:, :, i)));
end
imshow(sideBySide);

end