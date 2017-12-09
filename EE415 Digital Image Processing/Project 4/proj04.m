% read original image
img = imread('clown_gray_int_uint8.png');

% convert to logical using half-tone approx
outimg = xx_halftone(img);

% write resulting image to file
imwrite(outimg, 'clown_gray__halftone.png');

% show original image
figure(1);
imshow(img);

% show half-tone image
figure(2);
imshow(outimg);
