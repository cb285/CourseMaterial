a = imread('Fig0310(b)(washed_out_pollen_image).png');

figure(1);
imshow(a);

b = xx_imhisteq(a);

figure(2);
imshow(b);

figure(3);
subplot(2,1,1);
hist(a(:), 256)
xlim([0 255]);
subplot(2,1,2);
hist(b(:), 256)
xlim([0 255]);
