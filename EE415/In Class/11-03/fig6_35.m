a = imread('Fig0635(top_ left_flower).tif');

figure(1);
imshow(a);

b = .5*(1-cos(pi*(double(a)/255)));

figure(2);
imshow(b);

c = imread('Fig0635(middle_row_left_chalk ).png');

figure(3);
imshow(c);

A = 2.5;
d = (double(c)/255).^A;

figure(4);
imshow(d);

hsi = xx_rgb2hsi(c);

B = .5;
hsi(:,:,3) = hsi(:,:,3).^A;
hsi(:,:,2) = hsi(:,:,2).^B;

d = xx_hsi2rgb(hsi);

figure(5);
imshow(d);

e = imread('Fig0635(bottom_left_stream).tif');
figure(6);
imshow(e);

C = .5;
f = (double(e)/255).^C;

figure(7);
imshow(f);