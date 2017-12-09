a = imread('Fig0424(a)(rectangle).png');

figure(1);
imshow(a);

b = fft2(a);
b_mag = abs(b);

bmax = max(max(b_mag));
bmin = min(min(b_mag));

bimg = uint8((b_mag - bmin)/ (bmax - bmin)*255);

figure(2);
imshow(bimg);

cimg = fftshift(bimg);
figure(3);
imshow(cimg);

d = log10(fftshift(b_mag)+1);

dmax = max(max(d));
dmin = min(min(d));

dimg = uint8((d - dmin)/ (dmax - dmin)*255);
figure(4);
imshow(dimg);