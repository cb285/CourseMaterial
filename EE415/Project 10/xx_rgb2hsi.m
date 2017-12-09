%% Convert RGB image to HSI components
%%
%%   hsi = xx_rgb2hsi(rgb)
%%
%% Will accept RGB image of either uint8 or double class.
%% It returns HSI components (double).  All components are
%% normalized to the range 0 to 1.

function hsi = xx_rgb2hsi(rgb)
  
  if strcmp(class(rgb), 'uint8')
    rgb = double(rgb)/255;
  end
  
  hsi = zeros(size(rgb));
  
  % Compute hue
  hue = zeros(size(rgb(:,:,1)));
  den = sqrt((rgb(:,:,1)-rgb(:,:,2)).^2 + ...
             (rgb(:,:,1)-rgb(:,:,3)).*(rgb(:,:,2)-rgb(:,:,3)));
  num = 0.5*((rgb(:,:,1)-rgb(:,:,2))+(rgb(:,:,1)-rgb(:,:,3)));
  
  % Skip den == 0 points
  ind = den ~= 0;
  hue(ind) = acos(num(ind)./den(ind))/(2*pi);
  
  % acosd returns values between 0 and 180, adjust for full range
  ind = rgb(:,:,3) > rgb(:,:,2);
  hue(ind) = 1 - hue(ind);
  
  hsi(:,:,1) = hue;
  
  % Compute intensity (value)
  val = sum(rgb, 3)/3;
  hsi(:,:,3) = val;
  
  % Compute saturation
  sat = zeros(size(hue));
  ind = (val ~= 0);
  mrgb = min(rgb,[],3);
  sat(ind) = 1 - mrgb(ind) ./ val(ind);
  hsi(:,:,2) = sat;
  
end