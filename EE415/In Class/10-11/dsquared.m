function dsq = dsquared(p, q)
  [YY XX] = meshgrid(1:q, 1:p);
  dsq = (XX - p/2).^2 + (YY - q/2).^2;
endfunction