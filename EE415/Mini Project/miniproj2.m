orig = imread('Fig0314(a)(100-dollars).tif');

figure(1);
imshow(orig);

for i = 1:8
  figure(i+1);
  imshow(xx_imbitplane(orig, i));
endfor