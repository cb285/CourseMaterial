% make directory for output image
mkdir 'fig04_62_imgs';

% read original image
a = imread('Fig0462(a)(PET_image).png');

% show original
figure(1);
imshow(a);

% write to file
imwrite(a, 'fig04_62_imgs/img_a.png');

% get dimensions
M = size(a, 1);
N = size(a, 2);
P = 2*M;
Q = 2*N;

%a = log(a+1);

% perform fft of original
a_fft = fftshift(fft2(a, P, Q));

% apply homomorphic filter
gamL = .25;
gamH = 2;
c = 1;
D0 = 80;
H = homomorphic(P, Q, gamL, gamH, c, D0);
b_fft = a_fft.*H;

% get ifft
b_full = real(ifft2(ifftshift(b_fft)));

% crop to original image size
b = b_full(1:M,1:N);

b = uint8(xx_imstretch(b)); %exp(b)));

% show result
figure(2);
imshow(b);

% write to file
imwrite(b, 'fig04_62_imgs/img_b.png');