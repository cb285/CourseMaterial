a = imread('01vlcsnapBW.png');
b = imread('02vlcsnapBW.png');
c = imread('03vlcsnapBW.png');

figure(1);
imshow(a);
figure(2);
imshow(b);
figure(3);
imshow(c);

Idd1 = abs(double(b) - double(a));
Idd2 = abs(double(b) - double(c));

thresh = 13;

I1 = and(Idd1 > thresh, Idd2 > thresh);

figure(4);
imshow(I1);

I2 = (Idd1 + Idd2 - abs(double(a) - double(c))) > thresh; 
figure(5);
imshow(I2);
