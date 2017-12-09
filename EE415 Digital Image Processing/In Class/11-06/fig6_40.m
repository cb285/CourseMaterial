a = imread('Fig0638(a)(lenna_RGB).tif');

figure(1);
imshow(a);

m = ones(5,5)/25;

b = zeros(size(a));

for k = 1:3
  b(:,:,k) = conv2(a(:,:,k), m, 'same');
endfor

b = uint8(b);

figure(2);
imshow(b);

a_hsi = xx_rgb2hsi(a);

for k = 1:3
  a_hsi(:,:,k) = conv2(a_hsi(:,:,k), m, 'same');
endfor

c = uint8(255*xx_hsi2rgb(a_hsi));

figure(3);
imshow(c);

d = sum(abs(double(b)-double(c)), 3);

figure(4);
imshow(d, []);