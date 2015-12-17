function[] = plotComponents(scoresX, scoresY, coeffX, coeffY, name, ...
    eigenplotname)

    xScoreX = scoresX(:, 1);
    xScoreY = scoresX(:, 2);
    yScoreX = scoresY(:, 1);
    yScoreY = scoresY(:, 2);
    f = figure();
    scatter(xScoreX, xScoreY);
    hold on;
    scatter(yScoreX, yScoreY);
    xlabel('PC 1');
    ylabel('PC 2');
    legend('Faces', 'NonFaces');
    saveas(f, name);
    
    % image size
    imSize = 19;
    
    % nonface PC1 and PC2 prep
    meanshiftedNonFaces = scoresY/coeffY;
    eigenvectorsNonFaces = scoresY(:,1:2);
    eigennonfaces = eigenvectorsNonFaces'*meanshiftedNonFaces;
    eigennonfaces = mat2gray(eigennonfaces);
    eigennonfaces = im2uint8(eigennonfaces);
    
    % face PC1 and PC2 prep
    meanshiftedFaces = scoresX/coeffX;
    eigenvectorsFaces = scoresX(:,1:2);
    eigenfaces = eigenvectorsFaces'*meanshiftedFaces;
    eigenfaces = mat2gray(eigenfaces);
    eigenfaces = im2uint8(eigenfaces);
    
    % plot eigenvectors for PC1 of face and nonface data
    g = figure();
    scatter(eigenfaces(1, :), eigenfaces(2, :));
    hold on;
    scatter(eigennonfaces(1, :), eigennonfaces(2, :));
    xlabel('PC 1');
    ylabel('PC 2');
    legend('Faces', 'NonFaces');
    saveas(g, eigenplotname);
    
end