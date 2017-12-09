% Clayton Brutus
% EE415
% Project 3
function r = xx_imbilinear(img, mag)
  % get size of input image
  Mx = size(img, 2);
  My = size(img, 1);
  
  % calculate size of output image
  Nx = size(img, 2)*mag;
  Ny = size(img, 1)*mag;
  
  % pad input image with duplicate row and column on bottom right
  % and convert it to double type
  img = double([img img(:,end); img(end,:) img(end,end)]);
  
  % create matrix to store output
  r = zeros(Ny,Nx);
  
  % loop over every pixel of output image
  for y = 0:(Ny-1)
    for x = 0:(Nx-1)
      
      % get corresponding input image pixel
      x0 = x*((Mx-1)/(Nx-1));
      y0 = y*((My-1)/(Ny-1));
      
      % get four neighboring pixel locations and intensities:
      % top left
      x1 = floor(x0);
      y1 = floor(y0);
      i1 = img(y1+1,x1+1);
      % bottom left
      x2 = floor(x0);
      y2 = floor(y0)+1;
      i2 = img(y2+1,x2+1);
      % top right
      x3 = floor(x0)+1;
      y3 = floor(y0);
      i3 = img(y3+1,x3+1);
      % bottom right
      x4 = floor(x0)+1;
      y4 = floor(y0)+1;
      i4 = img(y4+1,x4+1);
      
      % create matricies for solving equation of form A*B=C, where B is coefficients a,b,c, and d
      A = double([x1 y1 x1*y1 1; x2 y2 x2*y2 1; x3 y3 x3*y3 1; x4 y4 x4*y4 1]);
      C = double([i1; i2; i3; i4]);
      
      % solve for coefficients
      B = linsolve(A,C);
      
      % use coefficients to find output image intensity
      r(y+1,x+1) = round(B(1)*x0+B(2)*y0+B(3)*x0*y0+B(4));
      
    endfor
  endfor

  % convert back to uint8
  r = uint8(r);
end
