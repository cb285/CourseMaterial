% read original
a = imread('Fig0637(a)(caster_stand_original).png');

% show original
figure(1);
imshow(a);

% convert to hsi
a_hsi = xx_rgb2hsi(a);

% perform histogram equalization on intensity
eq_intensity = double(xx_imhisteq(uint8(255*a_hsi(:,:,3))))/255;

% create img b using equalized intensity
b_hsi = a_hsi;
b_hsi(:,:,3) = eq_intensity;

% convert to rgb
b = xx_hsi2rgb(b_hsi);

% show img b
figure(2);
imshow(b);

% create img c from img b
c_hsi = b_hsi;

% adjust saturation
A = .1;
c_sat = c_hsi(:,:,2) + A;
c_sat(c_sat > 1) = 1;
c_hsi(:,:,2) = c_sat;

% convert to rgb
c = xx_hsi2rgb(c_hsi);

% show img c
figure(3);
imshow(c);

% write images to files
mkdir 'imgs_637';
imwrite(a, 'imgs_637/img_a.png');
imwrite(b, 'imgs_637/img_b.png');
imwrite(c, 'imgs_637/img_c.png');