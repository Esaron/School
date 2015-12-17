function[] = plotWaveletCoeffs(file, outFile)

[C, S, image] = waveletDecomp(file);

f = figure();
plot(C);
saveas(f, outFile);

end