function xx_pbmwrite(img, fname)
      
  % check for corrent number of arguments
  if (nargin != 2)
    error("invalid arguments. usage: xx_pbmwrite(img, fname)")
    return
  endif
  
  % check for correct image class
  if (~isa(img, "logical"))
    error("invalid image class type. needs to be logical.")
    return
  endif
  
  % check for correct image dimensions
  if (ndims(img) != 2)
    error("invalid image dimensions. needs to have 2 dimensions.")
    return
  endif
  
  % get image width and height
  width = size(img, 2);
  height = size(img, 1);
  
  % inverse image so that it displays correctly
  img = ~img;
  
  % create matrix of zeros to hold binary data
  bin_img = uint8(zeros(size(img,1), idivide(size(img,2),8)));
  
  % calculate number of columns needed to add
  buff_cols = 0;
  
  if (width < 8)
    buff_cols = 8 - width;
  elseif (mod(width,8) != 0)
    buff_cols = 8-(mod(columns(img),8));
  endif
   
  % add buffer columns to image  
  img = logical([img ones(height, buff_cols)]);
  
  % convert matrix of bits to uint8 types
  bin_img = bitpack(flipud(fliplr(img))', "uint8");
  
  % open file for writing
  fid = fopen(fname, "w");
  
  % write magic number
  fprintf(fid, "P4\n");
  
  % write width, height
  fprintf(fid, "%d %d\n", width, height);
  
  % write image data
  fwrite(fid, flipud(fliplr(bin_img)));
  
  % close file
  fclose(fid);
  
endfunction
