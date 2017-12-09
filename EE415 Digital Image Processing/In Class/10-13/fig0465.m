a = imread('Fig0465(a)(cassini).png');

figure(1);
imshow(a);

M = size(a, 1);
N = size(a, 2);

a_fft = ifftshift(fft2(a));

H = ones(M, N);

DH = 10;
DW = 2;
H(1:(M/2)-DH, (N/2)-DW:(N/2)+DW) = 0;
H((M/2)+DH:end, (N/2)-DW:(N/2)+DW) = 0;

figure(2);
imshow(H);

b_fft = a_fft.*H;

b = uint8(real(ifft2(ifftshift(b_fft))))(1:M, 1:N);

figure(3);
imshow(b);

a_mag = abs(a_fft);

a_db = log(1+a_mag);

a_max = max(max(a_db));
a_min = min(min(a_db));
figure(4);
imshow((a_db - a_min)/(a_max-a_min));

H2 = 1 - H;
c_fft = a_fft.*H2;

c = real(ifft2(fftshift(c_fft)));
c_max = max(max(c));
c_min = min(min(c));

figure(5);
imshow((c-c_min)/(c_max-c_min));