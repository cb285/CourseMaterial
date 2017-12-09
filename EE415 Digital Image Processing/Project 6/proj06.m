% Clayton Brutus
% EE415
% Project 6 (Fig. 4.27)

% make output image directory
mkdir 'imgs';

% read original image
a = imread('Fig0427(a)(woman).png');

% write to file
imwrite(a, 'imgs/img_a.png');

% get dimensions of image
M = size(a, 1);
N = size(a, 2);

% calculate P and Q
P = 2*M;
Q = 2*N;

% show original image
figure(1);
imshow(a);

% calculate fft of original
a_fft = ifftshift(fft2(a, P, Q));

% separate magnitude and angle of fft
a_mag = abs(a_fft);
a_phase = angle(a_fft);

% show fft phase image
b = uint8(xx_imstretch(a_phase));
figure(2);
imshow(b);
pause(3);

% write to file
imwrite(b, 'imgs/img_b.png');

% get/show image of ifft of phase
c = uint8(xx_imstretch(real(ifft2(ifftshift(exp(i*a_phase))))))(1:M, 1:N);

figure(2);
imshow(c);
pause(3);

% write to file
imwrite(c, 'imgs/img_c.png');

% get/show image of ifft of magnitude
d = uint8(xx_imstretch(real(ifftshift(ifft2(ifftshift(a_mag))))));

figure(2);
imshow(d);
pause(3);

% write to file
imwrite(d, 'imgs/img_d.png');

% read rectangle image
rect = 255*imread('Fig0424(a)(rectangle).png');

% calculate fft of rectangle
rect_fft = ifftshift(fft2(rect, P, Q));

% separate magnitude and phase
rect_mag = abs(rect_fft);
rect_phase = angle(rect_fft);

% create image from phase of (a) and magnitude of rectangle
e = uint8(xx_imstretch(real(ifft2(ifftshift(rect_mag.*exp(i*a_phase))))))(1:M, 1:N);

% write to file
imwrite(e, 'imgs/img_e.png');

figure(2);
imshow(e);
pause(3);

% create image from phase of rectangle and magnitude of (a)
f = uint8(xx_imstretch(real(ifft2(ifftshift(a_mag.*exp(i*rect_phase))))));

figure(2);
imshow(f);

% write to file
imwrite(f, 'imgs/img_f.png');