% make output directory
mkdir 'imgs';
32
% read original image
a = double(imread('Fig0343(a)(skeleton_orig).png'));

% show/write original image
figure(1);
imshow(uint8(a));
title('(a) Original Image');
imwrite(uint8(a), 'imgs/img_a.png');

% create mask for laplacian transformation
lapmask = [-1,-1,-1;-1,8,-1;-1,-1,-1;];

% convolve with laplacian mask
b = conv2(a, lapmask, 'same');

% show/write image b
pause(3);
figure(2);
bshow = uint8(xx_imstretch(b));
imshow(bshow);
title('(b) Laplacian of (a)');
imwrite(bshow, 'imgs/img_b.png');

% get image c
c = a + b;

% show/write image c
pause(3);
figure(1);
cshow = uint8(c);
imshow(cshow);
title('(c) Sharpened image (a+b)');
imwrite(cshow, 'imgs/img_c.png');

% create masks for sobel transformation
xsobelmask = [-1,-2,-1;0,0,0;1,2,1;];
ysobelmask = xsobelmask';

% get image d using sobel masks
d = abs(conv2(a, xsobelmask, 'same')) + abs(conv2(a,ysobelmask, 'same'));

% show/write image d
pause(3)
figure(2);
dshow = uint8(d);
imshow(dshow);
title('(d) Sobel gradient of (a)');
imwrite(dshow, 'imgs/img_d.png');

% get image e from convolution with averaging mask
e = conv2(d, ones(5,5)/25, 'same');

% show/write image e
pause(3);
figure(1);
eshow = uint8(e);
imshow(eshow);
title('(e) Sobel gradient smoothed by 5x5 avg filter');
imwrite(eshow, 'imgs/img_e.png');

% get image f
f = c.*e;

% show/write image f
pause(3);
figure(2);
f = uint8(xx_imstretch(f));
imshow(f)
title('(f) Mask image (c*e)');
imwrite(f, 'imgs/img_f.png');

% get image g
g = a + f;
g = uint8(xx_imstretch(g));

% show/write image g
pause(3);
figure(1);
imshow(g);
title('(g) Sharpened image (a+f)');
imwrite(g, 'imgs/img_g.png');

% get image h from gamma transformation
h = uint8((((double(g)/255)+eps).^.5)*255);

% show/write image h
pause(3);
figure(2);
imshow(h);
title('(h) Final result - power-law transformation of (g)');
imwrite(h, 'imgs/img_h.png');