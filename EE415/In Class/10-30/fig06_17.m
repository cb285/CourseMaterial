a = double(imread('fig_06_16_a.png'));

figure(1);
imshow(a);

hsv = xx_rgb2hsi(a);

hue = hsv(:,:,1);

ind = and(hue > .3, hue < .35);
hue(ind) = 0;

ind = and(hue > .62, hue < .68);
hue(ind) = 0;

hsv(:,:,1) = hue;

figure(2);
imshow(hue);

ind = and(hue > .45, hue < .55);
sat = hsv(:,:,2);
sat(ind) = .5*sat(ind);

hsv(:,:,2) = sat;

figure(3);
imshow(sat);

int = hsv(:,:,3);
int(int==1) = 0;
hsv(:,:,3) = int;
figure(4);
imshow(int);

rgb = xx_hsi2rgb(hsv);
figure(5);
imshow(rgb);