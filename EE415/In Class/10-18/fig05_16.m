a = imread('Fig0516(a)(applo17_boulder_noisy).png');

figure(1);
imshow(a);

M = size(a, 1);
N = size(a, 2);

a_fft = ifftshift(fft2(a));

a_mag = log10(abs(a_fft));

figure(2);
imshow(a_mag, []);

n = 3;
DW = 2500;
D0 = 177;
H = 1./ (1 + (DW ./ (dsquared(M, N) - D0^2)).^(2*n));

figure(3);
imshow(H);

d_fft = a_fft.*H;

figure(4);
imshow(log(1+abs(d_fft)), []);

d = real(ifft2(ifftshift(d_fft)));

figure(5);
imshow(d, []);

imwrite(xx_imstretch(d), 'img_b.png');