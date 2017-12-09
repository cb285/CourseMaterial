function r = xx_halftone(img)
  
  % create var for storing 3x3 approximations
  m = zeros(3,3,10);
  
  % create 3x3 approximation matrices
  m(:,:,1)  = [0,0,0;
               0,0,0;
               0,0,0];
               
  m(:,:,2)  = [0,0,0;
               0,1,0;
               0,0,0];
               
  m(:,:,3)  = [0,0,0;
               1,0,1;
               0,0,0];
               
  m(:,:,4)  = [0,0,0;
               1,1,1;
               0,0,0];
               
  m(:,:,5)  = [0,1,0;
               1,0,1;
               0,1,0];
               
  m(:,:,6)  = [0,1,0;
               1,1,1;
               0,1,0];
               
  m(:,:,7)  = [1,1,0;
               1,0,1;
               0,1,1];
               
  m(:,:,8)  = [0,1,1;
               1,1,1;
               1,1,0];
               
  m(:,:,9)  = [1,1,1;
               1,1,1;
               1,1,0];
               
  m(:,:,10) = [1,1,1;
               1,1,1;
               1,1,1];

  % get dimensions of input image
  X = size(img, 2);
  Y = size(img, 1);
  
  % create storage for output image
  r = zeros(3*Y,3*X);
  
  % create array of intensity levels
  e = uint8([0,28,56,84,112,140,168,196,224,252,256]);
  
  % for every pixel of the input image
  for j = 1:Y
    for i = 1:X
      % find the correct approximation matrix
      for c = 1:10
        if ((e(c) <= img(j,i)) && (img(j,i) < e(c+1)))
          % set approximation matrix to a 3x3 area of output image
          r(3*j-2:3*j,3*i-2:3*i) = m(:,:,c);
          break;
        endif
      endfor
    endfor
  endfor
  
  % convert to logical
  r = logical(r);
endfunction