function b = xx_imstretch(img)
  img_min = double(min(min(img)));
  img_max = double(max(max(img)));
  
  b = uint8(255*((double(img) - img_min) / (img_max - img_min)));
  
end