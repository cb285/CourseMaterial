a = imread('Fig0457(a)(thumb_print).png');

figure(1);
imshow(a);

M = size(a, 1);
N = size(a, 2);

P = 2*M;
Q = 2*N;

a_fft = ifftshift(fft2(a, P, Q));

D0 = 50;
n_filter = 4;
H = 1 ./ (1+(D0./dsquared(P,Q)).^(2*n_filter));

a_full = real(ifft2(ifftshift(a_fft.*H)));

a_hp = a_full(1:M,1:N);

figure(2);
imshow(uint8(a_hp));

a_thr = a_hp > 0;

figure(3);
imshow(a_thr)