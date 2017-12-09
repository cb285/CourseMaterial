figure(1)
img1 = imread('clown_bkwh_int_uint1.png');
imshow(img1)

img2 = imread('clown_gray_int_uint8.png');
figure(2)
imshow(255-img2)

img3 = imread('clown_colr_rgb_uint8.png');
figure(3)
imshow(img3)

[img4, map] = imread('clown_colr_map_uint8.png');
figure(4);
imshow(img4, map)