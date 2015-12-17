% Takes an input image (input) and performs cross-correlation by applying
% a filter (filter) to produce an output image (output).
           
function [] = convolve()
% read in image and convert to grayscale (if needed)
image = rgb2gray(imread('forward_slash.jpg'));

% filter cases
% horizontal: filterSwitch = 0
% vertical: filterSwitch = 1
% diagonal (forward): filterSwitch = 2
% diagonal (back): filterSwitch = 3
% equal: filterSwitch = 4
% ---This could trivially ask for input instead, but it is unnecessary---
filterSwitch = 4;
preSize = 5;
myFilter = zeros(preSize, preSize);

% size vars
imWid = size(image, 1);
imHt = size(image, 2);
filtWid = size(myFilter, 1);
filtHt = size(myFilter, 2);
centerOffset = ceil(filtWid/2);

switch filterSwitch
    case 0
        myFilter(1:filtWid, centerOffset) = 1;
        myFilter = myFilter./filtWid;
    case 1
        myFilter(centerOffset, 1:filtHt) = 1;
        myFilter = myFilter./filtHt;
    case 2
        for z = 1:filtWid
            myFilter(filtWid - z + 1, z) = 1;
        end
        myFilter = myFilter./filtWid;
    case 3
        for z = 1:filtWid
            myFilter(z, z) = 1;
        end
        myFilter = myFilter./filtWid;
    otherwise
        % set filter (equal weight to edges, 0 in middle) - produces an blurred
        % version of the original image
        myFilter = ones(preSize, preSize);
        myFilter = myFilter/((filtWid)*(filtHt));
end

% flip matrix for convolution
myFilter = rot90(myFilter, 2);

% copy image to avoid skewing output values
imageCopy = image;

% convolve
for y = 1:imHt-filtHt+1
    for x = 1:imWid-filtWid+1
        pixel = 0;
        for l = 1:filtHt
            for k = 1:filtWid
                % convolution: sum up the weighted pixel values in the
                % current region of the image (weighted according to the
                % filter) and store sum in output pixel (minus 1 is due
                % to indices being 1 off because of matrix row/column
                % indices starting at 1 and not 0)
                pixel = pixel + imageCopy(x+k-1,y+l-1)*myFilter(k,l);
            end
        end
        image(x+centerOffset,y+centerOffset) = pixel;
    end
end
% write image to file
imwrite(image, 'output.jpg');
% show modified images on screen
imshow(image);
end