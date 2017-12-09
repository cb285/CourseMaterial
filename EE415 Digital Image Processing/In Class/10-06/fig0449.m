a = uint8(255*imread('Fig0449(a)(text_gaps_of_1_and_2_pixels).png'));

figure(1);
imshow(a);

m = size(a, 1);
n = size(a, 2);

p = 2*m;
q = 2*n;

a_fft = fftshift(fft2(a, p, q));

dsq = dsquared(p, q);

figure(2);
np = 20;

d0 = 80;

H = exp(-dsq/((2*d0)^2));
b_fft = a_fft.*H;
b_full = uint8(real(ifft2(ifftshift(b_fft))));
b = b_full(1:m,1:n);
imshow(b);
title(strcat('D0=',int2str(d0)));
pause(2);

figure(3);
imshow(b > 1);