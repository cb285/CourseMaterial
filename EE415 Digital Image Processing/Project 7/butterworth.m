function H = butterworth(P, Q, D0, n, u0=0, v0=0)
  
  % get shifted distance squared function
  dsq = dsquared(P, Q, u0, v0);
  
  % create filter
  H = (1 ./ (1 + (D0.^2./dsq).^n));
  
endfunction