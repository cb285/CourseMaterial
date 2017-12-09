function H = notch(P, Q, D0, n, u0, v0)
  
  % create pair of highpass butterworth filters centered at (u0,v0) and (-u0,-v0)
  H = butterworth(P, Q, D0, n, u0, v0).*butterworth(P, Q, D0, n, -u0, -v0);
  
endfunction