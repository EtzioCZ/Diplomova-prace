clear variables
close all

[filename, pathname] = uigetfile( ...
  {'*.jpg', 'Obrazove soubory jpg';...
  '*.*', 'Vsechny soubory'},...
  'Vyber obrazovy soubor');
[RGB] = imread( fullfile( pathname, filename)); 

hsv = rgb2hsv(RGB);
RGB = im2double(RGB);
%% downsample
[x,y,z]=size(RGB);
newsize = 50;                 %èíso urèuje, na jakou hodnotu se má zaokrouhlit

if (newsize>x || newsize>y)== 1
  disp('nesmíte zvolit vyšší hodnotu než je stávající obrázek')
    return
end

%% new downsample
while x~=newsize
    
    x = round(x,-1);
    xx=lcm(x,newsize);
    mult = xx/x;
    divide = xx/newsize;
   R = resample(RGB(:,:,1),mult,divide);     
   G = resample(RGB(:,:,2),mult,divide);    
   B = resample(RGB(:,:,3),mult,divide);
   
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

%% pøevody

%HSV
hsv = rgb2hsv(RGB);
H =hsv(:,:,1); %0 - 1 hue odstín - pouze barva; H=0 odstíny šedi 
S =hsv(:,:,2); %sytost - pod 20% je to stupeò šedi, 
V =hsv(:,:,3); %hodnota jasu - množství bílého svìtla - 0.9<= bílá, 5>èerná,

%YCbCr
YCbCr = rgb2ycbcr(RGB);
Y = YCbCr(:,:,1);
imshow(RGB);

% imshow(hsv);


%% pøepoèet velièin na použitelné èísla
Red = 1500+R*10000;
Green =1000+G*2000;
Blue = 50+B*200;
Saturation = 400+S*500;
Hue = 100+H*15000;
Value = V;

%% definice promìnných pro tvorbu zvuku
fs = 44100;
ts = 1/fs;

trvanigranule = 20;                    %v ms

velikostokna=(fs/1000)*trvanigranule;
n = 0:velikostokna-1;                   %poèet vzorkù pro jednu granuly 
t = n/fs;

%% obálka pro granule
% hannovo okno
Hann = hann(velikostokna,'periodic'); %,'periodic'
Hann = Hann';
% sinx/x
Sinc = sinc(t);
% pila
Saw = sawtooth(t);
% gauspuls
Gaus = gauspuls(t,0.01,0.5);
% dirichlet 
Dirich = diric(t,2);
% ètverec
square = square(t,50);
% bez obálky 
none = 1;

%% custom obálka 
% cASR = ones(1,velikostokna);
cASR = n;
% attack
attack = 2 ; % 10 ms
aASR = 0.01*cASR;
aASR = aASR(1,1:velikostokna/(trvanigranule/attack));
% sustain               
sustain = 5; % 30 ms
sASR = ones(1,length(cASR));
sASR = sASR(1,1:velikostokna/(trvanigranule/sustain));
% release               
release = 3 ; % 10 ms
rASR = 1-0.01*cASR;
rASR = rASR(1,1:velikostokna/(trvanigranule/release));
% výsledná obálka
CASR = horzcat(aASR,sASR,rASR);
%% používaná obálka 
ASR = Hann;

%% pøíprava matic pro tvorbu zvuku
Y = spiralTransform(Y,0);

Value = spiralTransform(Value,0);

Red = spiralTransform(Red,0);

Green = spiralTransform(Green,0);

Blue = spiralTransform(Blue,0);

Saturation = spiralTransform(Saturation,0);

Hue = spiralTransform(Hue,0);


%% plnìní matice pro zvuk
sx=0;
i=0;

for i = 1: length(Y)        
        s = Y(i)*cos(2*pi*Red(i)*t)...
           +Y(i)*cos(2*pi*Green(i)*t)...
           +Y(i)*cos(2*pi*Blue(i)*t)...
           +Y(i)*cos(2*pi*Saturation(i)*t)...
           +Y(i)*cos(2*pi*Hue(i)*t);
       %s = s.*ASR;
   sx = horzcat(sx,s);   
end
sx = sx(1, 2:end);
sx = sx';
sx = sx/max(abs(sx));

%% filtrování podle lidského sluchu
f = logspace(log10(20),log10(20000),length(sx));
TiQ_dB = 3.64*(f/1000).^(-0.8) - 6.5*exp(-0.6*(f/1000-3.3).^2) + 10.^(-3)*(f/1000).^4;
TiQ = 10.^(TiQ_dB/20);
TiQ_norm = TiQ./max(TiQ);
SX = fft(sx);
TIQ = fft(TiQ_norm');
newsx = ifft(SX+TIQ);

%% zvuk a zobrazení
newsx = newsx/max(abs(newsx))/sqrt(2);
Granulka1_1
% sound(newsx,fs)
% 
% figure(2)
% N = 2048;
% [yy,f,t,p] = spectrogram(newsx,hanning(N),N/2,N,fs,'yaxis');
% surf(t,f,10*log10(abs(p*fs)),'EdgeColor','none','marker','square'); 
% xlabel('{\itt} (s) \rightarrow');
% ylabel('{\itf} (Hz) \rightarrow');
% ylim ([20 20000]);
% caxis([-50 0]);
% axis tight; colormap(jet); view(0,90);
% colorbar
% set(gca,'layer','top','box','on','yscale','log','ylim',[20 20000])
% 
% pocetprehranychgranuly=length(sx)/velikostokna;