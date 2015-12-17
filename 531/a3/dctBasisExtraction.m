function[output] = dctBasisExtraction()
Adct = [];
for j=1:8
    for i=1:8
        Dct = zeros(8);
        Dct(j,i) = 1;
        % inverse transform
        output = idct2(Dct);
        % unroll
        Adct(:,((j-1)*8)+i) = output(:);
    end
end
plotBFs(Adct);
end