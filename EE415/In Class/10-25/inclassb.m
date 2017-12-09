a = zeros(256,256,3, 'uint8');

C = uint8([0:255]'*ones(1, 256));

a(:,:,1) = 255;
a(:,:,2) = C';
a(:,:,3) = flipud(C);

figure(1);
imshow(a);

a(:,:,1) = 0;
a(:,:,2) = C';
a(:,:,3) = flipud(C);

figure(2);
imshow(a);

a(:,:,1) = flipud(C);
a(:,:,2) = C';
a(:,:,3) = 0;

figure(3);
imshow(a);