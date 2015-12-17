function[] = plotBlocks(fileName)

% read in image and convert to grayscale
image = imread(fileName);
image = rgb2gray(image);

% size of a face
faceSize = 19;

% get blocks
blocks = breakIntoBlocks(image, faceSize);

% number of scales
scales = size(blocks, 3);
xPositions = size(blocks, 4);
yPositions = size(blocks, 5);
xBlocks = size(blocks, 2);
yBlocks = size(blocks, 1);

% output image holder
out = [];

for i = 1:scales
    for m = 1:yPositions
        for j = 1:xPositions
            out = [];
            for k = 1:yBlocks
                row = [];
                for l = 1:xBlocks
                    block = blocks{k, l, i, j, m};
                    xSize = size(block, 1);
                    ySize = size(block, 2);
                    if isempty(block) == 0
                        % white dividers
                        horzDivide = ones(1, ySize).*255;
                        vertDivide = ones(xSize, 1).*255;
                        block(1,:) = horzDivide;
                        block(xSize,:) = horzDivide;
                        block(:,1) = vertDivide;
                        block(:,ySize) = vertDivide;
                        row = horzcat(row, block);
                    end
                end
                out = vertcat(out, row);
            end
            if isempty(out) == 0
                imwrite(out, strcat('scale', num2str(i),...
                    '_posX', num2str(j), '_poxY', num2str(m), '.jpg'));
            end
        end
    end
end

end
