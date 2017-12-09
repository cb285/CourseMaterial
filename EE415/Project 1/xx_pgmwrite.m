function xx_pgmwrite(img, fname)
  
  % check for corrent number of arguments
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
  if (ndims(img) != 2)
    error("invalid image dimensions. needs to have 2 dimensions.")
    return
  endif
  
  % open file for writing
  fid = fopen(fname, "w");
  
  % write magic number
  fprintf(fid, "P5\n");
  
  % write width, height
  fprintf(fid, "%d %d\n", size(img, 2), size(img, 1));
  
  % write mav value
  fprintf(fid, "255\n");
  
  % write image data
  fwrite(fid, img');
  
  % close file
  fclose(fid);
  
endfunction
