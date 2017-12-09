% read original
a = imread('Fig0631(a)(strawberries_coffee_full_color).tif');

% show original
figure(1);
imshow(a);

% convert to double image
a_double = double(a)/255;

% get pixels within color cube
cubeind = ones(size(a(:,:,1)));

W = .2549;
r = [.6863, .1608, .1922];

for k = 1:3
  cubeind = and(cubeind, abs(r(k) - a_double(:,:,k)) <= W/2);
endfor

cubeind3 = cat(3, cubeind, cubeind, cubeind);

% set all other pixels to gray
b = a.*cubeind;
b(~cubeind3) = 128;

% show result
figure(2);
imshow(b);

% get pixels within color sphere
sphind = ones(size(a(:,:,1)));

R0 = .1765;

for k = 1:3
  sphind = and(sphind, (r(k) - a_double(:,:,k)).^2 <= R0^2);
endfor

sphind3 = cat(3, sphind, sphind, sphind);

% set all others pixels to gray
c = a.*sphind;
c(~sphind3) = 128;

% show result
figure(3);
imshow(c);

% convert original to hsi
a_hsi = xx_rgb2hsi(a);

% get pixels with hue in [0, .05) or (.9, 1] and saturation in (.465, 1]
hsiind = zeros(size(a(:,:,1)));
hsiind = or(a_hsi(:,:,1) > .9, a_hsi(:,:,1) < .05);
hsiind = and(hsiind, a_hsi(:,:,2) > .465);

% set all other pixels to gray
d = a.*hsiind;
hsiind3 = cat(3, hsiind, hsiind, hsiind);
d(~hsiind3) = 128;

% show result
figure(4);
imshow(d);

% write images to file
mkdir 'imgs_634';
imwrite(a, 'imgs_634/img_a.png');
imwrite(b, 'imgs_634/img_b.png');
imwrite(c, 'imgs_634/img_c.png');
imwrite(d, 'imgs_634/img_d.png');
