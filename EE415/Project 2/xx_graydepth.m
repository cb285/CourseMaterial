% Clayton Brutus
% EE415 Project 2

function ret_img = xx_graydepth(img, levels)
  
  a = floor(256/levels);
  b = floor(256/(levels-1));

  ret_img = uint8(idivide(img,a)*b);
  
endfunction
