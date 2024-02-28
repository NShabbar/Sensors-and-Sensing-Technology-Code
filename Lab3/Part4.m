% This MatLab file answers part 4 of Lab 3
% Author: Katherine Rogacheva 

%% Part 4.1 Accelerometer
close all;
clear all; 
clc;

% Call the CreateTumbleData function with npts = 1000
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
figure(21);
plot3(Anoise(:,1), Anoise(:,2), Anoise(:,3), 'b.');
axis equal;
title('Uncalibrated Accelerometer Data');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;


% Now, calibrate the data using CalibrateEllipsoidData3D
kstep = length(Anoise);  % Number of iterations for calibration (adjust as needed)
plotFlag = true;  % Set to true if you want to visualize the calibration steps
[Atilde_A, Btilde_A] = CalibrateEllipsoidData3D(Anoise(:,1), Anoise(:,2), Anoise(:,3), kstep, plotFlag);

% Correct accelerometer data
[Xcorr_A, Ycorr_A, Zcorr_A] = CorrectEllipsoidData3D(Anoise(:,1), Anoise(:,2), Anoise(:,3), Atilde_A, Btilde_A);

% Plot the corrected data
figure(22);
scatter3(Xcorr_A, Ycorr_A, Zcorr_A, 'b.');
axis equal;
title('Calibrated Accelerometer Data');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;


% Calculate means and standard deviations for accelerometer data
% Calculate norms for accelerometer data
% norm_Uncalibrated_A = sqrt(sum(Anoise.^2, 2));
norm_Uncalibrated_A = sqrt((Anoise(:,1).^2) + (Anoise(:,2).^2) + (Anoise(:,3).^2));
norm_Calibrated_A = sqrt(Xcorr_A.^2 + Ycorr_A.^2 + Zcorr_A.^2);

% Find means and standard deviations 
disp('Uncalibrated Accelerometer Data Statistics:');
disp(['Mean: ', num2str(mean(norm_Uncalibrated_A))]);
disp(['Standard Deviation: ', num2str(std(norm_Uncalibrated_A))]);

disp('Calibrated Accelerometer Data Statistics:');
disp(['Mean: ', num2str(mean(norm_Calibrated_A))]);
disp(['Standard Deviation: ', num2str(std(norm_Calibrated_A))]);


%% Part 4.1 Magnetometer
close all;
clear all; 
clc;

npts = 1000;
[Anoise, Hnoise, Adist, Bdist] = CreateTumbleData(npts);

disp('Hnoise:');
Hnoise = Hnoise * 1000;
%disp(Hnoise);

% Bias vector
disp('Bdist:');
%disp(Bdist);

% Plot Hnoise
figure(20);
plot3(Hnoise(:,1), Hnoise(:,2), Hnoise(:,3), 'r.');
axis equal;
title('Uncalibrated Magnetometer Data');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;

% Now, calibrate the data using CalibrateEllipsoidData3D
kstep = length(Anoise);  % Number of iterations for calibration (adjust as needed)
plotFlag = true;  % Set to true if you want to visualize the calibration steps
[Atilde_H, Btilde_H] = CalibrateEllipsoidData3D(Hnoise(:,1), Hnoise(:,2), Hnoise(:,3), kstep, plotFlag);

% Correct magnetometer data
[Xcorr_H, Ycorr_H, Zcorr_H] = CorrectEllipsoidData3D(Hnoise(:,1), Hnoise(:,2), Hnoise(:,3), Atilde_H, Btilde_H);


figure(23);
scatter3(Xcorr_H, Ycorr_H, Zcorr_H, 'r.');
axis equal;
title('Calibrated Magnetometer Data');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;


% Calculate means and standard deviations for magnetometer data
% Calculate norms for magnetometer data
norm_Uncalibrated_H = sqrt((Hnoise(:,1).^2) + (Hnoise(:,2).^2) + (Hnoise(:,3).^2));
norm_Calibrated_H = sqrt(Xcorr_H.^2 + Ycorr_H.^2 + Zcorr_H.^2);

% Find means and standard deviations 
disp('Uncalibrated Magnetometer Data Statistics:');
disp(['Mean: ', num2str(mean(norm_Uncalibrated_H))]);
disp(['Standard Deviation: ', num2str(std(norm_Uncalibrated_H))]);

disp('Calibrated Magnetometer Data Statistics:');
disp(['Mean: ', num2str(mean(norm_Calibrated_H))]);
disp(['Standard Deviation: ', num2str(std(norm_Calibrated_H))]);

%%