a = imread('Fig0635(middle_row_left_chalk ).png');

figure(1);
imshow(a);

hsi = xx_rgb2hsi(a);

% get hue value from imagej and xx_rgb2hsi

hue = hsi(:,:,1);

delta = .05;
yellow_hue = .17;
yind = and(hue > (yellow_hue - delta), hue < (yellow_hue + delta));

delta = .03;
purple_hue = .64;
pind = and(hue > (purple_hue - delta), hue < (purple_hue + delta));

hind = or(yind, pind);

sat = hsi(:,:,2);
sat(~hind) = 1;
hsi(:,:,2) = sat;

b = xx_hsi2rgb(hsi);

figure(2);
imshow(b);