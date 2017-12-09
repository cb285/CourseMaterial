% Clayton Brutus
% EE415
% Project 3

% read original image
img1 = imread('Fig0222(b)(cameraman).tif');

% reduce to half size
img2 = xx_imbilinear(img1, .5);

% enlarge back to original size
img3 = xx_imbilinear(img2, 2);
img4 = xx_imnearest2(img2, 2);

% make output image directory
mkdir "imgs"

% write images to files
imwrite(img1, "imgs/img1.png");
imwrite(img2, "imgs/img2.png");
imwrite(img3, "imgs/img3.png");
imwrite(img4, "imgs/img4.png");
