function[] = testing()

image = zeros(11,11);
image(6,6) = 255;
image
% set filter (equal weight to edges, 0 in middle) - produces an blurred
% version of the original image
preSize = 5;
filter = ones(preSize, preSize);
padSize = 1;
filter = padarray(filter, [padSize padSize], 1);
filter = filter/preSize^2;

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
        image(x+3,y+3) = pixel;
    end
end

image

output = conv2(image, filter)
end