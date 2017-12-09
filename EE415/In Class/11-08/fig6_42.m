a = imread('Fig0642(a)(jupiter_moon_original).tif');

figure(1);
imshow(a);

a_hsi = xx_rgb2hsi(a);

for k = 1:3
  figure(1+k);
  imshow(a_hsi(:,:,k));
endfor

a_sat = a_hsi(:,:,2);
t = max(max(a_sat))/5;

mask = a_sat > t;

figure(5);
imshow(mask)

f = a_hsi(:,:,1).*mask;

figure(6);
imshow(f);

figure(7);
hist(f(:), 1000);

a_sat(~mask) = 0;
g_hsi = a_hsi;
g_hsi(:,:,1) = f;
g_hsi(:,:,2) = a_sat;

g = xx_hsi2rgb(g_hsi);

figure(8);
imshow(g);