function[HMM] = generateHMM(num_sequences, length, T, T_header,...
    S, S_header)

% S is the sensor model's output probability matrix
HMM = cell(2, 1);

% Populate state sequences
HMM{1, 1} = generateStates(num_sequences, length, T, T_header);
% Print newline
fprintf('\n');

% Number of possible outputs is columns of S
num_outputs = size(S, 2);

for i=1:num_sequences
    for j=1:length
        current_state = HMM{1, 1}(i, j);
        current_output = 'NULL';
        current_rand = rand(1);
        cumulative = 0;
        for k=1:num_outputs
            % round up if 0
            if current_rand == 0
                current_output = S(current_state, 1);
                break;
            end
            % set state if the random number falls in the range in S
            if current_rand > cumulative && current_rand <= cumulative...
                    + S(current_state, k)
                current_output = k;
                break;
            end
            % update cumulative
            cumulative = cumulative + S(current_state, k);
        end
        % set next output value
        HMM{2, 1}(i, j) = current_output;
    end
    disp(mat2str(HMM{1,1}(i,:)));
    disp(mat2str(S_header(HMM{2,1}(i,:))));
end

end