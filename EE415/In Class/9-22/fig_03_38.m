a = imread('Fig0338(a)(blurry_moon).png');

figure(1);
imshow(a);

m1 = [-1,-1,-1;-1,8,-1;-1,-1,-1;];
m2 = [0,-1,0;-1,4,-1;0,-1,0;]

b1 = conv2(a, m1, 'same');
b2 = conv2(a, m2, 'same');

figure(2);
imshow(uint8(b1));

c = (((b1 - min(min(b1)))/(max(max(b1)) - min(min(b1))))*255);
figure(3);
imshow(uint8(c));

d = a + b1;

figure(4);
imshow(uint8(d));

e = a + b2;
figure(5)
imshow(uint8(e));