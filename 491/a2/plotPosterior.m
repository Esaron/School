function[] = plotPosterior(n, T, alpha, beta)
    % Plot from lamda = 0 to lamda = 10 in increments of 0.5
	lamda = 0:0.5:10;
    
    % Poisson likelihood taking T into account
    likelihood = (beta/factorial(n)*alpha) ...
        .*((lamda.*T).^n.*exp(1).^(-lamda.*T));
    
    % Prior - Gamma function
    prior = (beta^alpha)/alpha.*(lamda.*T) ...
        .^(alpha - 1).*exp(1).^(-beta.*lamda.*T);
    
    % Normalizing constant set to 1.  We can ignore it since it will be
    % constant and the posterior is a Gamma function just like the prior.
    normalizer = 1;
    
    plot(likelihood.*prior./normalizer);
    hold on;
    xlabel('lamda');
    ylabel('p(lamda|n, T, alpha, beta) (Likelihood)');
    title('Belief About the Value of Lamda');
    hold off;
end