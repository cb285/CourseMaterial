% Expand/shrink a gray-level image by the nearest neighbor method.
%
% This method forms two index matrixes (that are the same size as the output
% matrix).  One matrix contains the nearest neighbor rows of the input (all
% columns of this matrix are identical).  The other matric contains the 
% nearest neighbor columns of the input (all rows of this matrix are identical).
%
% The two matrixes are added to create indices into the vector equivalent of
% the input image.
%
% Compare the execution time of this function to that of xx_imnearest1.

function b = xx_imnearest2(a, mag)
  [ma na] = size(a);
  mb = round(mag*ma);
  nb = round(mag*na);

  % Generate row and column index vectors for the output
  brow = 1:mb;
  bcol = 1:nb;
  
  % Calculate nearest neighbor indexes of the input matrix
  a_rind = round((brow-1)*(ma-1)/(mb-1))+1;
  a_cindm1 = round((bcol-1)*(na-1)/(nb-1));
  
  % Create indexes into vector form of input matrix
  a_mind = a_rind'*ones(1, nb)+ones(mb,1)*a_cindm1*ma;
  b = a(a_mind);  
end
