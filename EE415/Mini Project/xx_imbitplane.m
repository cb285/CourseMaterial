function r = xx_imbitplane(img, b)
  
  r = logical(bitand(img, 128/(2**(8-b))));

endfunction