% Takes an input image (input) and performs cross-correlation by applying
% a filter (filter) to produce an output image (output).
           
function [] = convEdge()

% THRESHOLD
minThreshold = input('Enter minimum output gradient for registering an edge: ');
maxThreshold = input('Enter maximum output gradient for registering an edge: ');

% read in image and convert to grayscale (if needed)
image = rgb2gray(imread('edgeImage.jpg'));

% set filter (vertical edge detection)
filter = [  2,  1,  0,  -1, -2;
            2,  1,  0,  -1, -2;
            2,  1,  0,  -1, -2;
            2,  1,  0,  -1, -2;
            2,  1,  0,  -1, -2];

% flip matrix for convolution
filter = rot90(filter, 2);
% size vars
imWid = size(image, 1);
imHt = size(image, 2);
filtWid = size(filter, 1);
filtHt = size(filter, 2);

% copy image
imageCopy = image;

% correlate
for y = 1:imHt-filtHt+1
    for x = 1:imWid-filtWid+1
        pixel = 0;
        for l = 1:filtHt
            for k = 1:filtWid
                % correlation: sum up the weighted pixel values in the
                % current region of the image (weighted according to the
                % filter) and store sum in output pixel (minus 1 is due
                % to indices being one off because of matrix row/column
                % indices starting at 1 and not 0)
                pixel = pixel + imageCopy(x+k-1,y+l-1)*filter(k,l);
            end
        end
        % Set to black if output within threshold, white if not
        if (pixel > minThreshold && pixel < maxThreshold)
            image(x+3,y+3) = 0;
        else
            image(x+3,y+3) = 255;
        end
    end
end
% write image to file
imwrite(image, 'convEdgeOutput.jpg');
% show modified images on screen
imshow(image);
end