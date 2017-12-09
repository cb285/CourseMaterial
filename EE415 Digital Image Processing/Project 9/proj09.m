% read corrupted image
a = imread('proj09_img.png');

% get dimensions of image
[M N] = size(a);

% show corrupted image
figure(1);
imshow(a);

% get fft
a_fft = ifftshift(fft2(a));

% create degredation function
u = [ -(ceil((M-1)/2):-1:1), 0, (1:floor((M-1)/2)) ]' * ones(1, N);
v = ones(M, 1) * [ -(ceil((N-1)/2):-1:1), 0, (1:floor((N-1)/2)) ];

a = .32;
b = .27;
T = 1;

s = u*a+v*b;

H = T*sinc(s).*exp(-j*pi*s);

% use inverse filtering
b_fft = a_fft ./ H;
b = uint8(real(ifft2(ifftshift(b_fft))));

% show inverse filtered image
figure(2);
imshow(b);

% write to file
imwrite(b, 'proj09_inv.png');

% create CLS function
p = [0, -1, 0; -1, 4, -1; 0, -1, 0];
p_fft = ifftshift(fft2(p, M, N));

gam = 0.0001;
Hm = conj(H)./(abs(H).^2 + gam*(abs(p_fft).^2));

% use CLS function
c_fft = a_fft .* Hm;
c = uint8(real(ifft2(ifftshift(c_fft))));

% show CLS image
figure(3);
imshow(c);

% write to file
imwrite(c, 'proj09_cls.png');