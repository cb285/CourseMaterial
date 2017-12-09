a = imread('Fig0429(a)(blown_ic).png');

figure(1);
imshow(a);

P = 2*size(a, 1);
Q = 2*size(a, 2);

a_fft = ifftshift(fft2(a, P, Q));

[YY, XX] = meshgrid(1:Q, 1:P);

DSQ = (XX - ((P/2)+1)).^2 + (YY - ((Q/2)+1)).^2;

D0 = 40; % radius of filter
H = exp(-2*DSQ/(2*D0^2));

b_fft = a_fft.*H;

b_full = uint8(real(ifft2(ifftshift(b_fft))));
figure(2);
imshow(b_full);

b_same = b_full(1:size(a,1), 1:size(a,2));

figure(3);
imshow(b_same);