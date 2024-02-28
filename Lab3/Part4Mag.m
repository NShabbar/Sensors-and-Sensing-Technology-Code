npts = 1000;
[Anoise, Hnoise, Adist, Bdist] = CreateTumbleData(npts);

disp('Hnoise:');
Hnoise = Hnoise * 1000;
%disp(Hnoise);

% Bias vector
disp('Bdist:');
%disp(Bdist);

% Plot Hnoise
figure(22); % Can't use Figure 1, 2, 3, or 4 because it will rewrite over figures from other functions
plot3(Hnoise(:,1), Hnoise(:,2), Hnoise(:,3), 'r.');
axis equal;
title('Uncalibrated Magnetometer Data');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;

% calibrates the data using CalibrateEllipsoidData3D
kstep = length(Anoise);  % Number of iterations for calibration
plotFlag = true;  % Set to true if you want to visualize the calibration steps
[Atilde_H, Btilde_H] = CalibrateEllipsoidData3D(Hnoise(:,1), Hnoise(:,2), Hnoise(:,3), kstep, plotFlag);

% Correct magnetometer data
[Xcorr_H, Ycorr_H, Zcorr_H] = CorrectEllipsoidData3D(Hnoise(:,1), Hnoise(:,2), Hnoise(:,3), Atilde_H, Btilde_H);


figure(23); % Can't use Figure 1, 2, 3, or 4 because it will rewrite over figures from other functions
scatter3(Xcorr_H, Ycorr_H, Zcorr_H, 'b.');
axis equal;
title('Calibrated Magnetometer Data');
xlabel('X');
ylabel('Y');
zlabel('Z');
grid on;


% Calculates means and standard deviations for magnetometer data
% Calculates norms for magnetometer data
norm_Uncalibrated_H = sqrt((Hnoise(:,1).^2) + (Hnoise(:,2).^2) + (Hnoise(:,3).^2));
norm_Calibrated_H = sqrt(Xcorr_H.^2 + Ycorr_H.^2 + Zcorr_H.^2);

% Find means and standard deviations 
disp('Uncalibrated Magnetometer Data Statistics:');
disp(['Standard Deviation: ', num2str(std(norm_Uncalibrated_H))]);
disp(['Mean: ', num2str(mean(norm_Uncalibrated_H))]);


disp('Calibrated Magnetometer Data Statistics:');
disp(['Standard Deviation: ', num2str(std(norm_Calibrated_H))]);
disp(['Mean: ', num2str(mean(norm_Calibrated_H))]);