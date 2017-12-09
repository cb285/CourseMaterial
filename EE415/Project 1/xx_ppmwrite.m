function xx_ppmwrite(img, fname)
  
  % check for correct number of arguments
  if (nargin != 2)
    error("invalid arguments. usage: xx_pgmwrite(img, fname)")
    return
  endif
  
  % check for correct image class
  if (~isa(img, "integer"))
    error("invalid image class type. needs to be an integer.")
    return
  endif
  
  % check for correct image dimensions
  if (ndims(img) != 3)
    error("invalid image dimensions. needs to have 3 dimensions.")
    return
  endif
  
  % open file for writing
  fid = fopen(fname, "w");
  
  % write magic number
  fprintf(fid, "P6\n");
  
  % write width height
  fprintf(fid, "%d %d\n", size(img, 2), size(img, 1));
  % write max value
  fprintf(fid, "255\n");
  
  % write img data
  fwrite(fid, permute(img, [3,2,1]));
  
  % close file
  fclose(fid);
  
endfunction
