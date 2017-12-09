% make output image directory
mkdir 'imgs';

% read original image
a = imread('Fig0514(a)(ckt_saltpep_prob_pt25).png');

% show original
figure(1);
imshow(a);

% write to file
imwrite(a, 'imgs/img_a.png');

% get median filter image
fsize = 7;
b = uint8(ordfiltn(a, ((fsize^2)+1)/2, true(fsize)));

% show image b
figure(2);
imshow(b);

% write to file
imwrite(b, 'imgs/img_b.png');

% get adaptive filter image
c = xx_imadapt(a);

% show image c
figure(3);
imshow(c);

% write to file
imwrite(c, 'imgs/img_c.png');