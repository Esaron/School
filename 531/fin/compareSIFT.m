function[features1, features2, keys1, keys2, img1, img2] = ...
    compareSIFT(img1, img2)

% Detect the SIFT features:
[features1,~,~,keys1] = detect_features(img1);

% Detect the SIFT features:
[features2,~,~,keys2] = detect_features(img2);

end