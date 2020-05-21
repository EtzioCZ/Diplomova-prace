clear variables
close all

[filename, pathname] = uigetfile( ...
  {'*.jpg', 'Obrazove soubory jpg';...
  '*.*', 'Vsechny soubory'},...
  'Vyber obrazovy soubor');
[RGB] = imread( fullfile( pathname, filename)); 

RGB = im2double(RGB);
%% definice tonu
tony=[16.35 17.32 18.35 19.45 20.60 21.83 23.12 24.50 25.96 27.50 29.14 30.87 32.70 34.65 36.71 38.89...
41.20 43.65 46.25 49.00 51.91 55.00 58.27 61.74 65.41 69.30 73.42 77.78 82.41 87.31 92.50 98.00 103.83 110.00...
116.54 123.47 130.81 138.59 146.83 155.56 164.81 174.61 185.00 196.00 207.65 220.00 233.08 246.94 261.63 277.18...
293.66 311.13 329.63 349.23 369.99 392.00 415.30 440.00 466.16 493.88 523.25 554.37 587.33 622.25 659.26 698.46...
739.99 783.99...
830.61 880.00 932.33 987.77 1046.50 1108.73 1174.66 1244.51 1318.51 1396.91 1479.98 1567.98 1661.22 1760.00...
1864.66 1975.53 2093.00 2217.46 2349.32 2489.02 2637.02 2793.83 2959.96 3135.96 3322.44 3520.00 3729.31...
3951.07 4186.01 4434.92 4698.64 4978.03 5274.04 5587.65 5919.91 6271.93 6644.88 7040.00 7458.62 7902.13]; 

nazev_tonu = ["Cn0", "Cs0", "Dn0" "Ds0" "En0" "Fn0" "Fs0" "Gn0" "Gs0" "An0" "As0"...
"Bn0" "Cn1" "Cs1" "Dn1" "Ds1" "En1" "Fn1" "Fs1" "Gn1" "Gs1" "An1" "As1" "Bn1" "Cn2"...
"Cs2" "Dn2" "Ds2" "En2" "Fn2" "Fs2" "Gn2" "Gs2" "An2" "As2" "Bn2" "Cn3" "Cs3" "Dn3"...
"Ds3" "En3" "Fn3" "Fs3" "Gn3" "Gs3" "An3" "As3" "Bn3" "Cn4" "Cs4" "Dn4" "Ds4" "En4"...
"Fn4" "Fs4" "Gn4" "Gs4" "An4" "As4" "Bn4" "Cn5" "Cs5" "Dn5" "Ds5" "En5" "Fn5" "Fs5"...
"Gn5" "Gs5" "An5" "As5" "Bn5" "Cn6" "Cs6" "Dn6" "Ds6" "En6" "Fn6" "Fs6" "Gn6" "Gs6"...
"An6" "As6" "Bn6" "Cn7" "Cs7" "Dn7" "Ds7" "En7" "F7n" "Fs7" "Gn7" "Gs7" "An7" "As7" "Bn7"...
"Cn8" "Cs8" "Dn8" "Ds8" "En8" "Fn8" "Fs8" "Gn8" "Gs8" "An8" "As8" "B8"];
    

%% new downsample
[x,y,z]=size(RGB);
newsize = 5;                 %èíso urèuje, na jakou hodnotu se má zaokrouhlit

if (newsize>x || newsize>y)== 1
  disp('nesmíte zvolit vyšší hodnotu než je stávající obrázek')
    return
end

while x~=newsize
    
    x = round(x,-1);
    xx=lcm(x,newsize);
    mult = xx/x;
    divide = xx/newsize;
   R = resample(RGB(:,:,1),mult,divide,0);     
   G = resample(RGB(:,:,2),mult,divide,0);    
   B = resample(RGB(:,:,3),mult,divide,0);
   RGB = cat(3,R,G,B);
 
   mR = mod(size(R,1),newsize);
   R = R(1:end-mR,:);
   mG = mod(size(G,1),newsize);
   G = G(1:end-mG,:);
   mB = mod(size(B,1),newsize);
   B = B(1:end-mB,:);
    RGB = cat(3,R,G,B);
    RGB = permute(RGB,[2 1 3]);
   [x,y,z]=size(RGB); 
 
end
%imagesc(RGB);
%% pøevody 
%HSV
RGB = uint8(RGB);
hsv = (rgb2hsv(RGB));
H =hsv(:,:,1);          %0 - 1 hue odstín - pouze barva; H=0 odstíny šedi 
S =hsv(:,:,2);          %sytost - pod 20% je to stupeò šedi, 
V =hsv(:,:,3);          %hodnota jasu - množství bílého svìtla - 0.9<= bílá, 5>èerná,

%YCbCr
YCbCr = rgb2ycbcr(RGB);
Y = YCbCr(:,:,1);
Cb = YCbCr(:,:,2);
Cr = YCbCr(:,:,3);

%% kvantizace hodnot
pocet_ruznych_tonu_v_solu = 12;
max_harm = 20;

treshS = linspace(0,1,max_harm+1);
treshS = treshS(2:end-1);
Sq= imquantize(S,treshS);               %S = poèet harmonických
treshH = 0.5;
Hq = imquantize(H,treshH);              %H = více sudé/všechny
Hq = Hq-1;                              % 0 a 1
treshV = linspace(0,1,pocet_ruznych_tonu_v_solu+1);
treshV = treshV(2:end-1);
Vq = imquantize(V,treshV);              %V = výška tónu
treshY = linspace(0,1,3+1);             %5 pro šestnáctiny, 3 pro ètvrtiny, 1 pro celé
treshY = treshY(2:end-1);               %Y = trvání noty
Yq = imquantize(Y,treshY);              %R = èervená - amplituda vyšších harm
                                        %B = modrá - amplituda prvních
                                        %harmonickýhc
Yq = Yq-1;                              %G - zelená - amplituda støedních harmonickýh

%% transformace matic do vektoru
Value = spiralTransform(Vq,0);
Red = spiralTransform(R,0);
Green = spiralTransform(G,0);
Blue = spiralTransform(B,0);
Saturation = spiralTransform(Sq,0);
Hue = spiralTransform(Hq,0);
Y = spiralTransform(Yq,0);

%% definice promìnných pro tvorbu zvuku
fs = 44100;
ts = 1/fs;
trvanigranule = 200 ;                    %v ms
velikostokna=round((fs/1000)*(trvanigranule));
n = 0:velikostokna-1;                   %poèet vzorkù pro jednu granuly 
t = n/fs;

%% zvuk
%     sx=ones(1,velikostokna);
%     % hannovo okno
%     Hann = hann(velikostokna,'periodic'); %,'periodic'
%     Hann = Hann';
sy=0;
%% obálka pro granule

tic;
for i = 1: length(Value)  

    velikostokna=round((fs/1000)*(trvanigranule/(2.^Y(i))));
    n = 0:velikostokna-1;                   %poèet vzorkù pro jednu granuly 
    t = n/fs;
    sx=ones(1,velikostokna);
    
    % hannovo okno
    Hann = hann(velikostokna,'periodic'); %,'periodic'
    Hann = Hann';
    
    
    if Saturation(i)>=17
         for j = 1:8 
         s =  Blue(i)*cos(2*pi*tony(46+2*Value(i))*j*t);
            if mod(Saturation(i),2)==1 && Hue(i)==0
                s = s*0.5;             
            end
         sx = sx + s;
         end
         for j = 8:16
         s =  Green(i)*cos(2*pi*tony(46+2*Value(i))*j*t);
            if mod(Saturation(i),2)==1 && Hue(i)==0
                s = s*0.5;             
            end
         sx = sx + s;
         end
         for j = 17:Saturation(i) 
         s =  Red(i)*cos(2*pi*tony(46+2*Value(i))*j*t);
            if mod(Saturation(i),2)==1 && Hue(i)==0
                s = s*0.5;             
            end
         sx = sx + s;
         end
         
    elseif Saturation(i)>=8
         for j = 1:8
         s =  Blue(i)*cos(2*pi*tony(46+2*Value(i))*j*t);
            if mod(Saturation(i),2)==1 && Hue(i)==0
                s = s*0.5;             
            end
         sx = sx + s;
         end
         for j = 8:Saturation(i) 
         s =  Green(i)*cos(2*pi*tony(46+2*Value(i))*j*t);
            if mod(Saturation(i),2)==1 && Hue(i)==0
                s = s*0.5;             
            end
         sx = sx + s;
         end
         
    else
         for j = 1:Saturation(i)
         s =  Blue(i)*cos(2*pi*tony(46+2*Value(i))*j*t);
            if mod(Saturation(i),2)==1 && Hue(i)==0
                s = s*0.5;             
            end 
         sx = sx + s;
          end
                 
    end
    sx = sx/Saturation(i);
    sx = sx.*Hann;
    sy = horzcat(sy,sx); 
    
end
    
toc; 
sy = sy(1,2:end);
sy = sy/max(sy)/sqrt(2);
sound(sy,fs)

N = 2048;
[yy,f,t,p] = spectrogram(sy,hanning(N),N/2,N,fs,'yaxis');
surf(t,f,10*log10(abs(p*fs)),'EdgeColor','none','marker','square'); 
xlabel('{\itt} (s) \rightarrow');
ylabel('{\itf} (Hz) \rightarrow');
ylim ([20 20000]);
caxis([-50 0]);
axis tight; colormap(jet); view(0,90);
colorbar
set(gca,'layer','top','box','on','ylim',[20 20000])
%'yscale','log',
