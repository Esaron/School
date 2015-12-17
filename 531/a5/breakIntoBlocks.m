function[blocks] = breakIntoBlocks(image, faceSize)

% size
imRows = size(image, 1);
imCols = size(image, 2);
    
% number of positions to use
positions = 4;

% determine min scale
faceSizeScale = max(faceSize/imRows, faceSize/imCols);
if faceSizeScale <= 0.01
    minScale = 0.2;
elseif faceSizeScale <= 0.05 && faceSizeScale > 0.01
    minScale = 0.25;
elseif faceSizeScale <= 0.2 && faceSizeScale > 0.05
    minScale = 0.33;
elseif faceSizeScale <= 0.5 && faceSizeScale > 0.2
    minScale = 0.5;
else
    minScale = 1;
end

% number of scales
numScales = floor(1/minScale);

disp(strcat('Using minimum scaling coefficient of: ', num2str(minScale),...
    ' for: ', num2str(numScales), ' scales.'));

blocks = {};
for i = numScales:-1:1
    im = imresize(image, i*minScale);
    
    % get image size
    imRows = size(im, 1);
    imCols = size(im, 2);
    
    % bestblk for block size (add 5 pixel buffer to max)
    %blockSize = bestblk(size(im), faceSize+5);
    
    % hardcoded block size (faceSize)
    blockSize = [faceSize faceSize];
    
    % number of blocks that fit
    horz = imRows/blockSize(1);
    vert = imCols/blockSize(2);
    
    % number of times to loop
    horzBlocks = floor(horz);
    vertBlocks = floor(vert);
    
    % fractions of a face for positions
    fractionFace = ceil(faceSize/positions);
    
    % each of 4 positions
    for vertPos = 1:positions
        for horzPos = 1:positions
            % store blocks
            for j = 1:vertBlocks+2
                for k = 1:horzBlocks+2
                    % lower bound
                    blockRowStart = ((k-2)*blockSize(1)+1)...
                        +(horzPos-1)*fractionFace;
                    % upper bound
                    blockRowEnd = ((k-1)*blockSize(1))...
                        +(horzPos-1)*fractionFace;
                    if blockRowEnd > size(im, 1)
                        blockRowEnd = size(im, 1);
                    end
                    % left bound
                    blockColStart = ((j-2)*blockSize(2)+1)...
                        +(vertPos-1)*fractionFace;
                    % right bound
                    blockColEnd = ((j-1)*blockSize(2))...
                        +(vertPos-1)*fractionFace;
                    if blockColEnd > size(im, 2)
                        blockColEnd = size(im, 2);
                    end
                    % special case if position is offset from top/left
                    if k-1 == 0
                        blockRowStart = 1;
                        blockRowEnd = 1+(horzPos-1)*fractionFace;
                    end
                    if j-1 == 0
                        blockColStart = 1;
                        blockColEnd = 1+(vertPos-1)*fractionFace;
                    end
                    blocks{k, j, i, horzPos, vertPos} = ...
                        im(blockRowStart:...
                           blockRowEnd,...
                           blockColStart:...
                           blockColEnd);
                end
            end
        end
    end
    
end

end