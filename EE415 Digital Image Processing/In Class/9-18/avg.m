a = imread('Fig0334(a)(hubble-original).tif');

figure(1);
imshow(a);

w = ones(15,15)/(15*15);

b = uint8(round(conv2(a,w,"same")));
figure(2);
imshow(b);

%c = b > mean(mean(b));

thresh = .25*max(max(b))
c = b > thresh;


figure(3);
imshow(c);

d = c.*a;

figure(4);
imshow(d);