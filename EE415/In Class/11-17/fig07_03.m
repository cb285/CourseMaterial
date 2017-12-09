a = imread('Fig_07_03.png');

p1{1} = double(a)/255.0;

% number of levels to generate
p = 3;

% 3x3 filter
f = fspecial("gaussian", 3);

for k = 1:p
  t = imfilter(p1{k}, f, "same");
  p1{k+1} = imresize(t, 0.5);
endfor

for k = 1:(p+1)
  figure(k);
  imshow(p1{k});
endfor

p2{p+1} = p1{p+1};

for k = p:-1:1
    t = imresize(p1{k+1}, 2); % upscale
    p2{k} = p1{k} - t;
endfor

for k = 1:(p+1)
  figure(k+4);
  imshow(p2{k}, []);
endfor

img = p2{p+1};

for k = p:-1:1
  t = imresize(img, 2);
  img = t + p2{k};
endfor

figure(9);
imshow(img);