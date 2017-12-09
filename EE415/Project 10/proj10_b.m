% read original images
a = imread('Fig0627(a)(WashingtonDC Band3-RED).png');
b = imread('Fig0627(b)(WashingtonDC Band2-GREEN).png');
c = imread('Fig0627(c)(1)(WashingtonDC Band1-BLUE).png');
d = imread('Fig0627(d)(WashingtonDC Band4).png');

% show original images in subplots
figure(1);
subplot(2,2,1);
imshow(a);
subplot(2,2,2);
imshow(b);
subplot(2,2,3);
imshow(c);
subplot(2,2,4);
imshow(d);

[M,N] = size(a);

% create image e from a, b, c as red, green, blue
e = zeros(M, N, 3, 'uint8');

% red
e(:,:,1) = a;
% green
e(:,:,2) = b;
% blue
e(:,:,3) = c;

% show image e
figure(2);
imshow(e);

% create image f using image d as red
f = zeros(M, N, 3, 'uint8');

% red
f(:,:,1) = d;
% green
f(:,:,2) = b;
% blue
f(:,:,3) = c;

% show image f
figure(3);
imshow(f);

% write all images to output folder
mkdir 'imgs_627';
imwrite(a, 'imgs_627/img_a.png');
imwrite(b, 'imgs_627/img_b.png');
imwrite(c, 'imgs_627/img_c.png');
imwrite(d, 'imgs_627/img_d.png');
imwrite(e, 'imgs_627/img_e.png');
imwrite(f, 'imgs_627/img_f.png');