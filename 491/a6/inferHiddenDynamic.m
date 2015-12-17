function[] = inferHiddenDynamic(num_sequences, length,...
    T, T_header, S, S_header)

import brml.*

% Generate the model output
hmm = generateHMM(num_sequences, length, T, T_header, S, S_header);
% Display newline
fprintf('\n');

% Initial output probabilities assumed uniform
S_init = ones(size(S, 1), 1)/size(S, 1);

% Run viterbi for each sequence, at every index
for i=1:num_sequences
    disp('=======================================');
    fprintf('\n');
    disp(['Observed output for sequence ' int2str(i)]);
    disp(mat2str(S_header(hmm{2,1}(i,:))));
    fprintf('\n');
    disp('=======================================');
    fprintf('\n');
    for j=1:length
        [maxstate logprob] = HMMviterbi(hmm{2,1}(i, 1:j), T', S_init, S');
        % Display true state
        disp(['Sequence ' int2str(i) ' through index ' int2str(j)]);
        fprintf('\n');
        disp('True States:');
        disp(mat2str(hmm{1,1}(i, 1:j)));
        fprintf('\n');
        disp('Evidence:');
        disp(mat2str(S_header(hmm{2,1}(i, 1:j))));
        fprintf('\n');
        disp('Most Likely Hidden State Sequence:');
        disp(mat2str(maxstate));
        disp('---------------------------------------');
    end
    fprintf('\n');
end

end