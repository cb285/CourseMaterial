a = imread('Fig0638(a)(lenna_RGB).tif');

figure(1);
imshow(a);

m = [0, -1,  0;
    -1,  4, -1;
     0, -1,  0];

lap = zeros(size(a));

for k = 1:3
  lap(:,:,k) = conv2(a(:,:,k), m, 'same');
endfor

b = uint8(double(a) + lap);

figure(2);
imshow(b);

a_hsi = xx_rgb2hsi(a);

c_hsi(:,:,3) = conv2(a_hsi(:,:,3), m, 'same');

c = uint8(255*xx_hsi2rgb(c_hsi));

figure(3);
imshow(c);

d = sum(abs(double(b)-double(c)), 3);

figure(4);
imshow(d, []);