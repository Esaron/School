function [posterior] = estimatePosterior(numRolls, sides, givenTotal)
    % Only works for 2 dice at the moment
    numDice = 2;
    [counts] = rollAndPlot(numRolls, sides, ...
        numDice);
    sideProbabilities = zeros(sides, numDice);
    for i = 1:sides
        for j = 1:numDice
            sideProbabilities(i, j) = counts(i, j)/numRolls;
        end
    end
    diceJointProbability = zeros(sides);
    for i = 1:sides
        for j = 1:sides
            diceJointProbability(i, j) = sideProbabilities(i, 1)...
                * sideProbabilities(i, 2);
        end
    end
    pTotalGivenRolls = zeros(sides);
    for i = 1:sides
        for j = 1:sides
            if i + j == givenTotal
                pTotalGivenRolls(i,j) = diceJointProbability(i, j);
            end
        end
    end
    posterior = zeros(sides);
    for i = 1:sides
        for j = 1:sides
            if i + j == givenTotal
                posterior(i,j) = pTotalGivenRolls(i, j)...
                    /sum(pTotalGivenRolls(:));
            end
        end
    end
end

