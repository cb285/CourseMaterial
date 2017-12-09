function H = homomorphic(P, Q, gamL, gamH, c, D0)
  
  H =(gamH - gamL)*(1 - exp(-c*(dsquared(P,Q)./D0^2))) + gamL;
  
endfunction