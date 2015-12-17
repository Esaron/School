function[blocks, hits] = classifyBlocks(fileName, faceSize, net)

% Load image and convert to grayscale
image = imread(fileName);
image = rgb2gray(image);
imSize = size(image);

% Get the blocks
blocks = breakIntoBlocks(image, faceSize);

% number of scales, positions, and blocks in each dimension
scales = size(blocks, 3);
xPositions = size(blocks, 4);
yPositions = size(blocks, 5);
xBlocks = size(blocks, 2);
yBlocks = size(blocks, 1);

% Initialize output variable
hits = [];
outputIm = [];
totalHitZones = [];

% Feed each block into the NN
for i = 1:scales
    hitZones = [];
    scaleIm = [];
    for m = 1:yPositions
        for j = 1:xPositions
            out = [];
            for k = 1:yBlocks
                row = [];
                for l = 1:xBlocks
                    block = blocks{k, l, i, j, m};
                    % Ignore empty blocks
                    if isempty(block) == 0
                        doubleBlock = double(block);
                        newBlock = imresize(doubleBlock, [faceSize faceSize]);
                        class = net(newBlock(:));
                        % Register a hit with 99% certainty
                        if class(1) > 0.99 && length(doubleBlock) == 19
                            hits = vertcat(hits, mat2gray(newBlock),...
                                ones(1,19)*255);
                            % Set value of borders to 256 for later
                            doubleBlock(:,1) = 260;
                            doubleBlock(:,size(doubleBlock,2)) = 260;
                            doubleBlock(1,:) = 260;
                            doubleBlock(size(doubleBlock,1),:) = 260;
                        end
                        row = horzcat(row, doubleBlock);
                    end
                end
                out = vertcat(out, row);
            end
            if isempty(out) == 0
                %imwrite(mat2gray(out), strcat(fileName,...
                %    '_faces_', num2str(i), num2str(m), num2str(j),...
                %    '.jpg'));
                for u = 1:size(out, 2)
                    for v = 1:size(out, 1)
                        if out(v, u) > 255
                            hitZones = vertcat(hitZones, [v, u]);
                        end
                    end
                end
            end
        end
    end
    scaleIm = out;
    for a = 1:size(hitZones, 1);
        scaleIm(hitZones(a,1), hitZones(a,2)) = 260;
    end
    if  isempty(scaleIm) == 0
        %imwrite(mat2gray(scaleIm), strcat(fileName, '_faces_scale_',...
        %    num2str(i), '.jpg'));
        scaleIm = imresize(scaleIm, imSize, 'nearest');
        for u = 1:size(scaleIm, 2)
            for v = 1:size(scaleIm, 1)
                if scaleIm(v, u) > 255
                    totalHitZones = vertcat(totalHitZones, [v, u]);
                end
            end
        end
    end
end
outputIm = image;
for a = 1:size(totalHitZones, 1);
    outputIm(totalHitZones(a,1), totalHitZones(a,2)) = 260;
end
imwrite(mat2gray(outputIm), strcat(fileName, '_faces', '.jpg'));
if isempty(hits) == 0
    imwrite(hits, 'hits.jpg');
end

end