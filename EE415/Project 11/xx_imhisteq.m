function r = xx_imhisteq(img)
  r = zeros(size(img), 'uint8');
  
  MN = size(img,1)*size(img,2);
  
  skold = 0;
  for k = 0:255
    ind = (img == k);
    sk = (255/MN)*sum(sum(ind)) + skold;
    r(ind) = uint8(sk);
    skold = sk;
  endfor
endfunction