a = imread('Fig0442(a)(characters_test_pattern).png');

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

for d0 = [10, 30, 60, 160, 460]
  H = exp(-dsq/((2*d0)^2));
  b_fft = a_fft.*H;
  b_full = uint8(real(ifft2(ifftshift(b_fft))));
  b = b_full(1:m,1:n);
  imshow(b);
  title(strcat('D0=',int2str(d0)));
  pause(2);
endfor