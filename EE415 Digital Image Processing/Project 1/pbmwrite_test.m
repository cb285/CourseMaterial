clear all
img1 = imread("clown_bkwh_int_uint1.png");

figure(1);
imshow(img1);
disp(['Original clown image size: ' int2str(size(img1))]);

xx_pbmwrite(img1,'test1.pbm');
img2 = imread('test1.pbm');
figure(2);
imshow(img2);
disp(['Clown image size after xx_pbmwrite: ' int2str(size(img2))]);
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

% Generate BW image with rand number of rows and columns
% The number of columns may not be a multiple of 8
nc = 100 + floor(100*rand(1));
nr = 100 + floor(100*rand(1));

% 5 black and 5 white stripes
swid = floor(nc/10);

pad = floor(mod(nc, 10)/2);
row = repmat([zeros(1,swid,'logical') ones(1,swid, 'logical')], 1 , 5);
row = prepad(row, length(row)+pad, 1);
row = logical(postpad(row, nc, 0));
img3 = repmat(row(1:nc),nr,1);
figure(1);
imshow(img3);
disp(['Original random-sized image size: ' int2str(size(img3))]);

xx_pbmwrite(img3,'test2.pbm');
img4 = imread('test2.pbm');
figure(2);
imshow(img4);
disp(['Random-sized image size after xx_pbmwrite: ' int2str(size(img4))]);
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
