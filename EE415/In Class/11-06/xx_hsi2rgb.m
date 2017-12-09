%% Convert HSI components to RGB image.
%%
%%   rgb = xx_hsi2rgb(hsi)
%%
%% Will accept HSI matrix of either uint8 or double class.
%% It returns RGB image of class double.

function rgb = xx_hsi2rgb(hsi)
  
  if strcmp(class(hsi), 'uint8')
    hsi = double(hsi)/255;
  end
  
  m = size(hsi,1);
  n = size(hsi,2);
  
  red = zeros(m,n);
  grn = zeros(m,n);
  blu = zeros(m,n);
  
  % Extract hue, sat, val
  hue = hsi(:,:,1)*360;
  sat = hsi(:,:,2);
  val = hsi(:,:,3);
  
  % RG Sector
  ind = and(hue>=0,hue<120);
  blu(ind) = val(ind).*(1 - sat(ind));
  red(ind) = val(ind).*(1 + sat(ind).*cosd(hue(ind)) ./ cosd(60 - hue(ind)));
  grn(ind) = 3*val(ind) - red(ind) - blu(ind);
  
  % GB Sector (hue = hue - 120)
  ind = and(hue>=120,hue<240);
  red(ind) = val(ind).*(1 - sat(ind));
  grn(ind) = val(ind).*(1 + sat(ind).*cosd(hue(ind)-120) ./ cosd(180 - hue(ind)));
  blu(ind) = 3*val(ind) - red(ind) - grn(ind);

  % BR Sector (hue = hue - 240)
  ind = and(hue>=240,hue<=360);
  grn(ind) = val(ind).*(1 - sat(ind));
  blu(ind) = val(ind).*(1 + sat(ind).*cosd(hue(ind)-240) ./ cosd(300 - hue(ind)));
  red(ind) = 3*val(ind) - grn(ind) - blu(ind);
  
  rgb = reshape([red grn blu],[size(red) 3]);
  
end