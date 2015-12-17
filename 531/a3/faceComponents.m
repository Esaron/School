function[COEFF, SCORE, latent, cumProportion, x] = faceComponents()

[x,t] = readOnlyFaceData;

[COEFF, SCORE, latent] = princomp(x);

proportion = latent./sum(latent);
cumProportion = proportion;
for i = 1:size(proportion)
    curProp = proportion(i);
    for j = i+1:size(proportion)
        cumProportion(j) = cumProportion(j) + curProp;
    end
end

end