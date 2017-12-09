a = double(imread('Fig0427(a)(woman).png'));
b = double(imread('Fig0458(a)(blurry_moon).png'));

i = 1;

imshow(c);

for n = [0:0.1:1]
  figure(i);
  c = (1-n)*a + n*b;
  set(get(gca, 'children'), c); % not right
  pause(0.5);
  i = i + 1;
endfor