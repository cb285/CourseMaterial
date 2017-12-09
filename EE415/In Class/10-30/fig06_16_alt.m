% This uses Octave's rgb2hsv function.
% Intensities are max(R,G,B)

clear all
M = 256;
rgb = zeros(M,M,3,'uint8');

r = M/3;

XX = [0:255]'*ones(1,M);
YY = ones(M,1)*[0:255];

xred = (2/3)*M;
yred = (2/3)*M;
MASK = ((XX - xred).^2 + (YY - yred).^2) <= r.^2;
rgb(:,:,1) = 255 * MASK;

xgrn = (1/3)*M;
ygrn = (1/2)*M;
MASK = ((XX - xgrn).^2 + (YY - ygrn).^2) <= r.^2;
rgb(:,:,2) = 255 * MASK;

xblu = (2/3)*M;
yblu = (1/3)*M;
MASK = ((XX - xblu).^2 + (YY - yblu).^2) <= r.^2;
rgb(:,:,3) = 255 * MASK;

figure(1)
imshow(rgb)

hsi = rgb2hsv(rgb);

figure(2)
imshow(hsi(:,:,1))

figure(3)
imshow(hsi(:,:,2))

% Display intensity (value)
figure(4)
imshow(hsi(:,:,3))
