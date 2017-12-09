a = imread('Fig0526(a)(original_DIP).png');

[M N] = size(a);

figure(1);
imshow(a);

a_fft = ifftshift(fft2(a));

u = [ -(ceil((M-1)/2):-1:1), 0, (1:floor((M-1)/2)) ]' * ones(1, N);
v = ones(M, 1) * [ -(ceil((N-1)/2):-1:1), 0, (1:floor((N-1)/2)) ];

a = .1;
b = .1;
T = 1;

s = u*a+v*b;

H = T*sinc(s).*exp(-j*pi*s);

b_fft = a_fft.*H;

b = uint8(real(ifft2(ifftshift(b_fft))));

figure(2);
imshow(b);

c_fft = b_fft ./ H;

c = uint8(real(ifft2(ifftshift(c_fft))));

figure(3);
imshow(c);

sigma = 0.01;
d_fftn = b_fft + normrnd(0, sigma, size(b_fft));
d_fft = (d_fftn ./ H);

d = uint8(real(ifft2(ifftshift(d_fft))));

figure(4);
imshow(d);

K = 0.0001;
Hm = conj(H)./(abs(H).^2 + K);

e_fft = d_fftn .* Hm;
e = uint8(real(ifft2(ifftshift(e_fft))));

figure(5);
imshow(e);