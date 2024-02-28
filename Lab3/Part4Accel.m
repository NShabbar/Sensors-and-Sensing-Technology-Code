%% Part 4.1 Accelerometer
npts = 1000;
[Anoise, Hnoise, Adist, Bdist] = CreateTumbleData(npts);

% Display or use the generated data as needed
disp('Anoise:');
Anoise = Anoise / 2^15-1;
%disp(Anoise);

% Distortion matrix
disp('Adist:');
%disp(Adist);

% Plot Anoise
figure(20); % Can't use Figure 1, 2, 3, or 4 because it will rewrite over figures from other functions
plot3(Anoise(:,1), Anoise(:,2), Anoise(:,3), 'g.');
axis equal;
title('Uncalibrated Accelerometer Data');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;

kstep = length(Anoise);  % Number of iterations for calibration
plotFlag = true;  % Sets to true  to visualize the calibration steps
[Atilde_A, Btilde_A] = CalibrateEllipsoidData3D(Anoise(:,1), Anoise(:,2), Anoise(:,3), kstep, plotFlag);

% Correct accelerometer data
[Xcorr_A, Ycorr_A, Zcorr_A] = CorrectEllipsoidData3D(Anoise(:,1), Anoise(:,2), Anoise(:,3), Atilde_A, Btilde_A);

% Plots the corrected data
figure(21); % Can't use Figure 1, 2, 3, or 4 because it will rewrite over figures from other functions
scatter3(Xcorr_A, Ycorr_A, Zcorr_A, 'b.');
axis equal;
title('Calibrated Accelerometer Data');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;


% Calculates means and standard deviations for accelerometer data
% Calculates norms for accelerometer data
norm_Uncalibrated_A = sqrt((Anoise(:,1).^2) + (Anoise(:,2).^2) + (Anoise(:,3).^2));
norm_Calibrated_A = sqrt(Xcorr_A.^2 + Ycorr_A.^2 + Zcorr_A.^2);

% Finds means and standard deviations 
disp('Uncalibrated Accelerometer Data Statistics:');
disp(['Standard Deviation: ', num2str(std(norm_Uncalibrated_A))]);
disp(['Mean: ', num2str(mean(norm_Uncalibrated_A))]);


disp('Calibrated Accelerometer Data Statistics:');
disp(['Standard Deviation: ', num2str(std(norm_Calibrated_A))]);
disp(['Mean: ', num2str(mean(norm_Calibrated_A))]);