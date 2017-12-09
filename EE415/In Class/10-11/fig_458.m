a = imread('Fig0458(a)(blurry_moon).png');

figure(1);
imshow(a);

M = size(a, 1);
N = size(a, 2);

P = 2*M;
Q = 2*N;

a_fft = ifftshift(fft2(a, P, Q));

H = -4*pi^2*dsquared(P, Q);

da = real(ifft2(ifftshift(a_fft.*H)))(1:M,1:N);

a_max = double(max(max(a)));
a_min = double(min(min(a)));

ad = (double(a) - a_min)/(a_max-a_min);

b = ad - da/max(max(da));

figure(2);
imshow(b);