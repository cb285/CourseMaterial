a = imread('Fig0636(woman_baby_original).png');

figure(1);
imshow(a);

b = zeros(size(a), 'uint8');

b(:,:,1) = a(:,:,1);

figure(2);
imshow(b);

b(:,:,1) = 0;

b(:,:,2) = a(:,:,2);

figure(3);
imshow(b);

b(:,:,2) = 0;
b(:,:,3) = a(:,:,3);

figure(4);
imshow(b);