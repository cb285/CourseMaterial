function H = glpf(P, Q, D0)
  
  H = exp(-dsquared(P, Q)/((2*D0)^2));
  
endfunction