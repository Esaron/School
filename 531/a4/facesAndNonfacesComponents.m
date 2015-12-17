function[COEFF, SCORE, latent, x, cumProportion]...
    = facesAndNonfacesComponents()

[x,y,t] = readFaceAndNonfaceData;

[COEFF, SCORE, latent] = princomp(horzcat(x,y));
[faceCoeff, faceScore] = princomp(x);
[nonfaceCoeff, nonfaceScore] = princomp(y);

plotComponents(faceScore, nonfaceScore, faceCoeff, nonfaceCoeff, ...
    'componentPlot.png', 'componentEigenplot.png');

proportion = latent./sum(latent);
cumProportion = proportion;
for i = 1:size(proportion)
    curProp = proportion(i);
    for j = i+1:size(proportion)
        cumProportion(j) = cumProportion(j) + curProp;
    end
end

end