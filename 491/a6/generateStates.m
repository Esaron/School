function[states] = generateStates(num_sequences, length, T, T_header)

assert(size(T, 1) == size(T, 2), 'T must be a square matrix');
states = zeros(num_sequences, length);
num_states = size(T, 1);
for i=1:num_sequences
    % generate initial state
    init = ceil(rand(1)*3);
    % round up if 0
    if init == 0
        init = 1;
    end
    states(i, 1) = init;
    for j=1:length
        % set current state and initialize next
        current_state = states(i, j);
        next_state = 0;
        % generate next state
        next_rand = rand(1);
        cumulative = 0;
        for k=1:num_states
            % round up if 0
            if next_rand == 0
                next_state = 1;
                break;
            end
            % set state if the random number falls in the range in T
            if next_rand > cumulative && next_rand <= cumulative...
                    + T(current_state, k)
                next_state = k;
                break;
            end
            % update cumulative
            cumulative = cumulative + T(current_state, k);
        end
        % set next state if we haven't exceeded length
        if j < length
            states(i, j+1) = T_header(next_state);
        end
    end
    disp(mat2str(states(i,:)));
end

end