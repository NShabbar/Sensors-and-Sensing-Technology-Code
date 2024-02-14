
% Read data from the .txt file specifying the delimiter as comma
data = dlmread('Gyro hour.txt', ',');

% Check the size of the data array
[num_rows, num_cols] = size(data); % reveals there is a fourth column we have artifact data in.

% Extract X, Y, and Z components
X = data(:, 1); % Assuming the X component is in the first column
Y = data(:, 2); % Assuming the Y component is in the second column
Z = data(:, 3); % Assuming the Z component is in the third column

% Define bias values
X_bias = -16.20960699;
Y_bias = 28.33973081;
Z_bias = 13.71325617;

% Subtract bias values
X_corrected = (X - X_bias)/131;
Y_corrected = (Y - Y_bias)/131;
Z_corrected = (Z - Z_bias)/131;

% Compute cumulative sums
cumulative_sum_X = cumsum(X_corrected);
cumulative_sum_Y = cumsum(Y_corrected);
cumulative_sum_Z = cumsum(Z_corrected);

step = (3600/985012);

time = 0:step:3599.9964;

% Lengths of each item
length(X);
length(Y);
length(Z);
length(cumulative_sum_X);
length(cumulative_sum_Y);
length(cumulative_sum_Z);
length(time);

% Display cumulative sums if needed
% disp(cumulative_sum_X);
% disp(cumulative_sum_Y(84140:90000))
% disp(cumulative_sum_Z);

% Plot over time
plot(time, cumulative_sum_X, 'r');
hold on;
plot(time, cumulative_sum_Y, 'b');
hold on;
plot(time, cumulative_sum_Z, 'g');
