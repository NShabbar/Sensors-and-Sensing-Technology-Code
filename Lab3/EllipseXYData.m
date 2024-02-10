% 
% Simulated Data for CMPE-167 Winter 2018
%
R = 1;
Xmeas = [0.9347
    0.9570
   -0.6242
    1.2415
    1.0756
   -0.0298
    1.5189
    0.4453
   -0.5628
   -0.0476
   -0.7229
    0.0139
    1.3431
    1.4811
    1.0454
    0.9979
    1.2189
    0.5232
   -0.5799
    1.3681
    0.0681
    0.2069
   -0.6193
   -0.2821
    0.5394
   -0.8139
   -0.0148
    1.2408
   -0.5560
   -0.1411
   -0.7946
   -0.6001
    0.1714
   -0.1677
   -0.6358
   -0.8255
    1.0939
    1.5181
    1.6205
    1.3748
    0.6638
    0.1329
    0.1699
    0.2000
    0.8347
    1.3524
    1.4262
    1.1855
    1.3295
   -0.5547
    1.4096
    0.7090
   -0.6479
    0.3324
    0.6983
    1.0901
    1.4541
    0.2907
   -0.7798
    0.1069
    1.4057
   -0.6421
   -0.4982
    1.0944
    0.2416
   -0.6635
   -0.7096
    0.3634
    0.2665
    1.5058
    0.0225
   -0.7228
    1.5994
   -0.5159
    1.4125
    0.2263
    1.4188
    1.4561
    1.1533
   -0.6606
    0.4430
    1.3402
   -0.2161
    1.2337
   -0.5484
    1.1496
   -0.4635
   -0.4918
    0.7323
   -0.6476
   -0.4592
    1.1225
   -0.5976
    0.2905
    1.4232
    1.1820
    0.4304
   -0.6829
   -0.2182
    0.4317
    1.3932
    1.4542
    0.0273
   -0.0796
   -0.7569
    0.6432
    0.1065
   -0.2673
    0.4000
    0.9374
   -0.0138
    1.2939
   -0.6766
   -0.0510
   -0.1698
    1.0430
   -0.4285
   -0.5253
    0.0456
    1.4314
    1.1940
    0.9400
    1.1753
    1.2538
    0.6288
    0.7427
    0.7459
   -0.7379
   -0.4295
    1.1015
    0.3809
   -0.7079
    1.4174
   -0.6474
    1.3232
    0.0209
    0.4857
   -0.1420
    0.7069
    0.1560
    1.1866
    0.7311
   -0.3751
   -0.1832
    1.3090
   -0.6317
    0.9098
    1.5072
    0.2844
    1.5444];
Ymeas = [-2.2060
   -2.3082
   -1.4785
    1.1654
   -2.0800
    1.6180
    0.1472
    1.9846
   -1.7017
   -2.2126
   -0.9014
    1.6839
    0.5130
   -0.6402
    1.4768
   -2.1048
    1.0224
    1.8766
   -1.2218
   -1.3108
    1.6296
    1.6884
   -1.5972
   -2.0123
    1.6806
   -0.3194
   -2.2963
    1.1597
   -1.3952
    1.3348
   -0.5511
   -0.5201
    1.7365
   -1.9184
    0.1187
    0.5192
   -2.1046
   -0.5885
    0.2498
   -1.4155
   -2.3498
    1.6581
    1.5781
    1.7392
    1.5975
   -1.4834
   -1.3363
   -1.8185
    0.3529
    0.7205
    0.5420
    1.5699
   -0.4817
    1.9010
   -2.3546
    1.4125
   -1.2064
    1.5143
   -0.3000
    2.0410
    0.0781
    0.7285
   -1.6182
   -2.1147
   -2.3265
   -0.2124
   -0.6494
    1.8190
    1.7722
   -0.7596
    1.7966
    0.1797
   -0.6513
    0.6279
   -0.0503
   -2.2842
   -1.3540
    0.4736
    1.5601
   -0.6563
   -2.2978
    0.9160
   -1.9064
   -1.7244
   -1.3017
    1.3345
    1.2444
    0.9226
   -2.0024
   -0.4243
   -1.5977
    1.1444
   -1.3933
   -2.4185
   -0.8349
    1.1440
   -2.7658
   -1.2570
    1.4691
    1.8137
    0.0477
   -1.0218
   -2.3473
    1.5403
   -0.2810
   -2.4749
    1.7152
   -2.1144
    1.6246
    1.6802
   -2.3508
    0.7765
   -0.8349
    1.6367
   -2.1459
    1.2547
   -1.4310
   -1.6016
   -2.4803
    0.2932
    1.0310
   -2.2012
    1.0488
    1.0106
    1.6589
    1.5855
   -2.6320
   -1.0656
    0.9622
    1.3617
   -2.3449
   -0.3527
   -1.1779
   -1.3335
   -1.4093
    1.5086
    1.7755
   -2.1095
   -2.4443
    1.6028
    1.5606
   -2.3079
    0.8832
   -2.0762
   -1.3766
    0.4100
   -2.1250
   -0.6457
    1.7754
   -0.3000];




% Least Squares
% Length of data set X
len = length(Xmeas);

% Creating Matrix A for the Least Squares.
A = [Xmeas Ymeas.^2 Ymeas ones(len, 1)]; 

% Creating Matrix B for the Least Squares.
B = -Xmeas.^2;

% Get the Z matrix
Z = A\B; % NOTE: This is not division, this is (A^t * A)^-1 * A^t * B

% These values were taken from Z, and displayed by Z.
z1 = -0.7923;
z2 = 0.2658;
z3 = 0.1801;
z4 = -1.0349;

% Plug in above values into z1, z2, z3, and z4 equations to get...
x0 = 0.39615;
y0 = -0.3388;
a = 1.2073;
b = 2.3418;

% Calibrating equations from lab doc
xhat = (Xmeas-x0)/a;
yhat = (Ymeas-y0)/b;

% Plot of raw data
figure;
plot(Xmeas, Ymeas, 'r.'); 
hold on;

% Plot of calibrated data
plot(xhat, yhat, 'b.');

% Add legend and labels
legend('Original Data', 'Calibrated Data');
xlabel('X data');
ylabel('Y data');
title('Least Squares Data Calibration');