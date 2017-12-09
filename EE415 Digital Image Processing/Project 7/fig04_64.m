% make output image directory
mkdir 'fig04_64_imgs';

% read original image
a = imread('Fig0464(a)(car_75DPI_Moire).png');

% write to file
imwrite(a, 'fig04_64_imgs/img_a.png');

% show original
figure(1);
imshow(a);

% get dimensions
M = size(a, 1);
N = size(a, 2);
P = 2*M;
Q = 2*N;

% perform fft of original
a_fft = fftshift(fft2(a, P, Q));

% show spectrum of original
b = uint8(xx_imstretch(log(1+abs(a_fft))));
figure(2);
imshow(b);

% write to file
imwrite(b, 'fig04_64_imgs/img_b.png');

% set filter parameters
D0 = 27;
n = 4;

% create 4 pairs of notch filters
H1 = notch(P, Q, D0, n, 77, 58);
H2 = notch(P, Q, D0, n, -84, 55);
H3 = notch(P, Q, D0, n, 166, -54);
H4 = notch(P, Q, D0, n, 159, 59);

% apply notch filters
d_fft = a_fft.*H1.*H2.*H3.*H4;

% show spectrum of result
c = uint8(xx_imstretch(log(1+abs(d_fft))));
figure(3);
imshow(c);

% write to file
imwrite(c, 'fig04_64_imgs/img_c.png');

% get ifft
d = uint8(xx_imstretch(real(ifft2(ifftshift(d_fft)))))(1:M,1:N);

% show result
figure(4);
imshow(d);

% write to file
imwrite(d, 'fig04_64_imgs/img_d.png');