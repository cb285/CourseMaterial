a = imread('Fig0507(a)(ckt-board-orig).png');

figure(1);
imshow(a);

b = a + 255*binornd(1, 0.1, size(a)) - 255*binornd(1, .1, size(a));

figure(2);
imshow(b);

c = ordfiltn(b, 5, true(3), 'symmetric');

figure(3);
imshow(c);

