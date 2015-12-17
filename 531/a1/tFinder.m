% Takes an input image (input) and performs cross-correlation by applying
% a filter (filter) to produce an output image (output).
           
function [] = tFinder()

% read in image and convert to grayscale (if needed)
image = rgb2gray(imread('text.jpg'));

% set filter (t detection)
myFilter = rgb2gray(imread('t.jpg'));
myFilter = 255.-myFilter;
myFilter = myFilter./sum(sum(myFilter));

% flip matrix for convolution
%myFilter = rot90(myFilter, 2);
% size vars
imWid = size(image, 1);
imHt = size(image, 2);
filtWid = size(myFilter, 1);
filtHt = size(myFilter, 2);
centerOffset = ceil(filtWid/2);

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
                pixel = pixel + imageCopy(x+k-1,y+l-1)*myFilter(k,l);
            end
        end
        % Set to black if output within threshold, white if not
        if (pixel < 75 && pixel > 0)
            image(x+centerOffset,y+centerOffset) = 0;
        else
            image(x+centerOffset,y+centerOffset) = 255;
        end
    end
end
% write image to file
imwrite(image, 'tFinderOutput.jpg');
% show modified images on screen
imshow(image);
end