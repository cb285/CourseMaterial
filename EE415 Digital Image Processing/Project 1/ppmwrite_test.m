clear all
img1 = imread("clown_colr_rgb_uint8.png");

figure(1);
imshow(img1);
disp(['Original clown image size: ' int2str(size(img1))]);

xx_ppmwrite(img1,'test1.ppm');
img2 = imread('test1.ppm');
figure(2);
imshow(img2);
disp(['Clown image size after xx_ppmwrite: ' int2str(size(img2))]);
if(size(img1)==size(img2))
  str = 'Testing to see if images are identical: ';
  if (all(all(img1==img2)))
    disp([str 'yep, good job!']);
  else
    disp([str 'nope, keep trying!'])
  end
else
  disp('Error - images are not the same size!');
end
disp('');
pause(2);

% Generate color bar image with rand number of rows and columns
nc = 100 + floor(100*rand(1));
nr = 100 + floor(100*rand(1));

bwid = floor(nc/7);
% red = 1 1 0 0 1 1 0
% blu = 1 0 1 0 1 0 1
% gre = 1 1 1 1 0 0 0

bar_hi = 192*ones(1, bwid, 'uint8');
bar_lo = zeros(1, bwid, 'uint8');

rred = [repmat(bar_hi,1,2) repmat(bar_lo,1,2) repmat(bar_hi,1,2) bar_lo];
rred = prepad(rred, nc, 255);
rgre = [repmat(bar_hi,1,4) repmat(bar_lo,1,3)];
rgre = prepad(rgre, nc, 255);
rblu = [repmat([bar_hi bar_lo],1,3) bar_hi];
rblu = prepad(rblu, nc, 255);

img3 = cat(3,repmat(rred,nr,1), repmat(rgre,nr,1), repmat(rblu,nr,1));
figure(1);
imshow(img3);
disp(['Original random-sized image size: ' int2str(size(img3))]);

xx_ppmwrite(img3,'test2.ppm');
img4 = imread('test2.ppm');
figure(2);
imshow(img4);
disp(['Random-sized image size after xx_ppmwrite: ' int2str(size(img4))]);
if(size(img3)==size(img4))
  str = 'Testing to see if images are identical: ';
  if (all(all(img3==img4)))
    disp([str 'yep, good job!']);
  else
    disp([str 'nope, keep trying!'])
  end
else
  disp('Error - images are not the same size!');
end
