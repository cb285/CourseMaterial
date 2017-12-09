% read original image
a = imread('Fig0622(a)(tropical_rain_grayscale.png');

% show original
figure(1);
imshow(a);

% create jet colormap of 256 levels
cmap = jet(256);

% show colored image
figure(2);
imshow(a, cmap);

% write images to file
mkdir 'imgs_622';
imwrite(a, 'imgs_622/img_a.png');
imwrite(a, cmap, 'imgs_622/img_b.png');
