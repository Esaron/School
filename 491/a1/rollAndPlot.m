function[counts, countOfSums, sums, outcomes] = rollAndPlot(n, sides, numberDice)
    maxResult = sides * numberDice;
    countOfSums = zeros(maxResult, 1);
    counts = zeros(sides, 2);
    outcomes = randi(sides, n, numberDice);
    for i = 1:size(outcomes, 1)
        counts(outcomes(i, 1), 1) = counts(outcomes(i, 1), 1) + 1;
        counts(outcomes(i, 2), 2) = counts(outcomes(i, 2), 2) + 1;
        outcome = sum(outcomes(i, :));
        countOfSums(outcome) = countOfSums(outcome) + 1;
        sums = sum(outcomes, 2);
    end
    hist(sums);
end
