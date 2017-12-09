clear all
img1 = imread("clown_gray_int_uint8.png");

figure(1);
imshow(img1);
disp(['Original clown image size: ' int2str(size(img1))]);

xx_pgmwrite(img1,'test1.pgm');
img2 = imread('test1.pgm');
figure(2);
imshow(img2);
disp(['Clown image size after xx_pgmwrite: ' int2str(size(img2))]);
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

% Generate gray level image with rand number of rows and columns
% The number of columns may not be a multiple of 8
nc = 100 + floor(100*rand(1));
nr = 100 + floor(100*rand(1));

row = uint8([0:(nc-1)]/(nc-1)*255);
img3 = repmat(row,nr,1);
figure(1);
imshow(img3);
disp(['Original random-sized image size: ' int2str(size(img3))]);

xx_pgmwrite(img3,'test2.pgm');
img4 = imread('test2.pgm');
figure(2);
imshow(img4);
disp(['Random-sized image size after xx_pgmwrite: ' int2str(size(img4))]);
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
