function dsq = dsquared(P, Q, u0=0, v0=0)
  
  % create mesh grid
  [YY XX] = meshgrid(1:Q, 1:P);
  
  % create dsquared matrix shirfted to u0, v0
  dsq = (XX - (P/2) - u0).^2 + (YY - (Q/2) - v0).^2;

endfunction