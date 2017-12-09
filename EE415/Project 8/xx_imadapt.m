function r = xx_imadapt(a, max_winsize=7)
  
  % get image dimensions
  M = size(a, 1);
  N = size(a, 2);
  
  % create storage for tracking pixels status
  done = zeros(M, N);
  new_stage_b = zeros(M, N);
  total_num_stageb = 0;
  
  % create output image storage
  r = zeros(M, N);
  
  % for each window size
  for winsize = 3:2:max_winsize
    % get min, max, median of every pixel
    z_min = ordfiltn(a, 1, true(winsize), 'symmetric');
    z_max = ordfiltn(a, winsize*winsize, true(winsize), 'symmetric');
    z_med = ordfiltn(a, (winsize*winsize+1)/2, true(winsize), 'symmetric');
    
    % calculate A1, A2
    A1 = int8(z_med) - int8(z_min);
    A2 = int8(z_med) - int8(z_max);
    
    % Find all of the new stage b pixels
    new_stage_b(:) = 0;
    new_stage_b(~done) = and(A1(~done) > 0, A2(~done) < 0);
    
    % report number of pixels that have reached stage b
    new_num_stageb = numel(new_stage_b(new_stage_b==1));
    total_num_stageb += new_num_stageb;
    printf("windowsize: %d, stage b pixels: new %d (%0.2f%%), total %d (%0.2f%%)\n",winsize, new_num_stageb, 100*(new_num_stageb/(M*N)), total_num_stageb, 100*(total_num_stageb/(M*N)));
    fflush(stdout);
    
    % process new stage b pixels
    B1 = a - z_min;
    B2 = a - z_max;
    zxy_ind = and(new_stage_b, B1 > 0, B2 < 0);
    r(zxy_ind) = a(zxy_ind);
    
    zmed_ind = and(~zxy_ind, new_stage_b);
    r(zmed_ind) = z_med(zmed_ind);
    
    % Keep track of all pixels that have been processed
    done = or(done, zxy_ind, zmed_ind);
  endfor
  
  % set unprocessed pixels to zmed
  r(~done) = z_med(~done);
  
  % convert output image to uint8
  r = uint8(r);

endfunction
