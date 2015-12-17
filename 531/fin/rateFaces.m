function[x] = rateFaces() 

gender = input('Select a gender [male, female]: ', 's');
while ~strcmp(gender, 'male') && ~strcmp(gender, 'female')
    disp('Invalid entry. Try again.');
    gender = input('Select a gender [male, female]: ', 's');
end
race = input('Select a race [caucasian, african, asian]: ', 's');
while ~strcmp(race, 'caucasian') && ~strcmp(race, 'african')...
        && ~strcmp(race, 'asian')
    disp('Invalid entry. Try again.');
    race = input('Select a race [caucasian, african, asian]: ', 's');
end

% get file listings
root = sprintf('userChoice/%s/%s/', gender, race);
faces = dir(sprintf('%s*.jpeg', root));

% size of filesets
faceSize = size(faces, 1);
imageSize = 19^2;

% init cell arrays (+1 for rating)
x = zeros(faceSize,imageSize + 1);

% display images and allow user to rate from 1-10
for i=1:faceSize
    faceFile = sprintf('%s%s', root, faces(i).name);
    faceImage = imread(faceFile);
    dispImage = imresize(faceImage, [100, 100]);
    imwrite(dispImage, sprintf('face%d.jpeg', i));
    imshow(dispImage);
    rating = input('Rate this face [1-10]: ');
    while rating > 10 || rating < 1
        Disp('Invalid entry. Try again.');
        rating = input('Rate this face [1-10]: ');
    end
    scaledGrayImage = imresize(rgb2gray(faceImage), [19 19]);
    doubleImage = double(scaledGrayImage);
    x(i,1:imageSize) = doubleImage(:);
    x(i,imageSize+1) = rating;
end

end