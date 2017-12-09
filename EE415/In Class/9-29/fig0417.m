a = imread('Fig0417(a)(barbara).tif');

figure(1);
imshow(a);

b = a(1:2:end, 1:2:end);
figure(2);
imshow(b);

c1 = uint8(conv2(a, ones(3,3)/9, 'same'));
c2 = c1(1:2:end, 1:2:end);
figure(3);
imshow(c2);