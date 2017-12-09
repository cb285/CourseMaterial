% read original image
a = imread('Fig0646(a)(lenna_original_RGB).tif');

% show original
figure(1);
imshow(a);

% define sobel gradient masks
mx = [-1, -2, -1;
      0,   0,  0;
      1,   2,  1];

my = [-1,  0,  1;
      -2,  0,  2;
      -1,  0,  1];

% compute gradients
rdx = conv2(a(:,:,1), mx, 'same');
rdy = conv2(a(:,:,1), my, 'same');

gdx = conv2(a(:,:,2), mx, 'same');
gdy = conv2(a(:,:,2), my, 'same');

bdx = conv2(a(:,:,3), mx, 'same');
bdy = conv2(a(:,:,3), my, 'same');

% vector method algorithm
gxx = rdx.^2 + gdx.^2 + bdx.^2;
gyy = rdy.^2 + gdy.^2 + bdy.^2;
gxy = rdx.*rdy + gdx.*gdy + bdx.*bdy;

theta = 0.5*atan2(2*gxy, (gxx-gyy));

f1 = (abs(0.5*((gxx + gyy) + (gxx - gyy).*cos(2*theta) + 2*gxy.*sin(2*theta)))).^0.5;
f2 = (abs(0.5*((gxx + gyy) + (gxx - gyy).*cos(2*(theta+pi/2)) + 2*gxy.*sin(2*(theta+pi/2))))).^0.5;

db = max(f1, f2);

db_max = max(max(db));
db_min = min(min(db));

b = uint8(255*((db - db_min) / (db_max - db_min)));

% show result
figure(2);
imshow(b);

% write images to file
mkdir 'imgs';
imwrite(a, 'imgs/img_a.png');
imwrite(b, 'imgs/img_b.png');