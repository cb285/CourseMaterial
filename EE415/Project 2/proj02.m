% Clayton Brutus
% EE415 Project 2

% read original skull image
sourceimg = imread('Fig0221(a)(ctskull-256).tif');

% make directory imgs in case it doesn't exist
mkdir 'imgs';

% write the original image
imwrite(sourceimg, 'imgs/img1.png');

% write images 2-7 with gray levels 128, 64, 32, 16, 8, 4, 2
for i = 2:8
  levels = 256/(2**(i-1));
  imwrite(xx_graydepth(sourceimg, levels), strcat('imgs/img', num2str(i), '.png'));
endfor