% make directory for output images
mkdir 'fig04_49_imgs';

% read original image
a = uint8(255*imread('Fig0449(a)(text_gaps_of_1_and_2_pixels).png'));

% show original
figure(1);
imshow(a);

% write to file
imwrite(a, 'fig04_49_imgs/img_a.png');

% get dimensions
M = size(a, 1);
N = size(a, 2);
P = 2*M;
Q = 2*N;

% perform fft of original
a_fft = fftshift(fft2(a, P, Q));

% apply GLPF
D0 = 80;
H = glpf(P, Q, D0);
b_fft = a_fft.*H;

% get ifft
b_full = uint8(real(ifft2(ifftshift(b_fft))));

% crop to original image size
b = b_full(1:M,1:N);

% show result
figure(2);
imshow(b);

% write to file
imwrite(b, 'fig04_49_imgs/img_b.png');