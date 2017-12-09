% Generate images in Figure 6.16

% Have class download and run this script to generate images
% Download xx_hsi2rgb and xx_rgb2hsi m-files also.

% Recreate Figure 6.17 as an in-class exercise

clear all
M = 256;
rgb = zeros(M,M,3,'uint8');

r = M/3;

XX = [0:255]'*ones(1,M);
YY = ones(M,1)*[0:255];

xred = (2/3)*M;
yred = (2/3)*M;
MASK = ((XX - xred).^2 + (YY - yred).^2) <= r.^2;
rgb(:,:,1) = 255 * MASK;

xgrn = (1/3)*M;
ygrn = (1/2)*M;
MASK = ((XX - xgrn).^2 + (YY - ygrn).^2) <= r.^2;
rgb(:,:,2) = 255 * MASK;

xblu = (2/3)*M;
yblu = (1/3)*M;
MASK = ((XX - xblu).^2 + (YY - yblu).^2) <= r.^2;
rgb(:,:,3) = 255 * MASK;

% Because resulting RGB image contains only two values (0 and 255)
% in each RGB plane, imwrite writes the image out as an RGB binary
% (logical) image.  Octave does not support RGB logical image types
% so the image will need to be converted back to uint8 or double
% when read back in.

imwrite(rgb, 'fig_06_16_a.png');

clear all

% Image is read in as a logical type
% Convert to double (intensities between 0 and 1)
rgb = double(imread('fig_06_16_a.png'));

figure(1)
imshow(rgb)

hsi = xx_rgb2hsi(rgb);

figure(2)
imshow(hsi(:,:,1))
imwrite(uint8(hsi(:,:,1)*255),'fig_06_16_b.png');

figure(3)
imshow(hsi(:,:,2))
imwrite(uint8(hsi(:,:,2)*255),'fig_06_16_c.png');

% Display intensity (value)
figure(4)
imshow(hsi(:,:,3))
imwrite(uint8(hsi(:,:,3)*255),'fig_06_16_d.png');
