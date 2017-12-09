a = imread('Fig0310(b)(washed_out_pollen_image).png');
figure(1);
imshow(a);

b = xx_imthresh(a);
figure(2);
imshow(b);