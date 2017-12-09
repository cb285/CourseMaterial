function b = xx_imthresh(img)
  img_avg = mean(mean(img));
  b = 255*(img > img_avg);
  
end