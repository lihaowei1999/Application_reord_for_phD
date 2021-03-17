%%
% Art_fcm_avg, Vein_fcm_avg, Liv_fcm_avg
Liv_t10=500;
Art_t10=1525;
Vein_t10=1503;
tr=2.08;
te=0.82;
fa=12*pi/180;
E0_liv=exp(-tr/Liv_t10);
E0_art=exp(-tr/Art_t10);
E0_vein=exp(-tr/Vein_t10);

R_liv=-1/tr*(log((Liv_fcm_avg+1)*(1-E0_liv)/(1-cos(fa)*E0_liv)-1)-log(cos(fa)*(Liv_fcm_avg+1)*(1-E0_liv)/(1-cos(fa)*E0_liv)-1));
R_art=-1/tr*(log((Art_fcm_avg+1)*(1-E0_art)/(1-cos(fa)*E0_art)-1)-log(cos(fa)*(Art_fcm_avg+1)*(1-E0_art)/(1-cos(fa)*E0_art)-1));
R_vein=-1/tr*(log((Vein_fcm_avg+1)*(1-E0_vein)/(1-cos(fa)*E0_vein)-1)-log(cos(fa)*(Vein_fcm_avg+1)*(1-E0_vein)/(1-cos(fa)*E0_vein)-1));

dr_liv=R_liv-R_liv(1);
dr_art=R_art-R_art(1);
dr_vein=R_vein-R_vein(1);
figure
plot([1:200],dr_liv,[1:200],dr_vein,[1:200],dr_art,'linewidth',2)
legend('Ct','PIF','AIF')
pp=gca;
set(pp,'linewidth',2,'FontSize',15);
%% 
options = optimoptions(@fminunc,'Display','iter','Algorithm','quasi-newton','MaxIter',1000);
x_al_r=zeros(21,21,4);
fv_al_r=zeros(21,21);
Init_value=[5,5,6,80];
for i=1:21
    for j=1:21
        [x_al_r(i,j,:),fv_al_r(i,j)]=fminunc(@residu,Init_value,options,dr_art,dr_vein,dr_liv,i-1,j-1);
    end
end
[a,b]=find(fv_al_r==min(min(fv_al_r)));
res=depic(x_al_r(a(1),b(1),:),dr_art,dr_vein,dr_liv,a(1),b(1));
aaaa=reshape(x_al_r(a(1),b(1),:),[1,4]);
dddd=fv_al_r(a(1),b(1));
%%
options = optimoptions(@fminunc,'Display','iter','Algorithm','quasi-newton','MaxIter',1000);
x_al_r=zeros(21,21,5);
fv_al_r=zeros(21,21);
Init_value=[5,7,6,70,12];
for i=1:21
    for j=1:21
        [x_al_r(i,j,:),fv_al_r(i,j)]=fminunc(@residu_2,Init_value,options,dr_art,dr_vein,dr_liv,i-1,j-1);
    end
end
[a,b]=find(fv_al_r==min(min(fv_al_r)));
res=depic_2(x_al_r(a(1),b(1),:),dr_art,dr_vein,dr_liv,a(1),b(1));
aaaa=reshape(x_al_r(a(1),b(1),:),[1,5]);
dddd=fv_al_r(a(1),b(1));
%% 

options = optimoptions(@fminunc,'Display','iter','Algorithm','quasi-newton','MaxIter',1000,'OptimalityTolerance',1e-20);
x_al_r=zeros(21,21,5);
fv_al_r=zeros(21,21);
Init_value=[7,6,5,0.2,30];
for i=1:21
    for j=1:21
        [x_al_r(i,j,:),fv_al_r(i,j)]=fminunc(@residu_3,Init_value,options,dr_art,dr_vein,dr_liv,i-1,j-1);
    end
end
[a,b]=find(fv_al_r==min(min(fv_al_r)));
res=depic_3(x_al_r(a(1),b(1),:),dr_art,dr_vein,dr_liv,a(1),b(1));
aaaa=reshape(x_al_r(a(1),b(1),:),[1,5]);
dddd=fv_al_r(a(1),b(1));
%%
options = optimoptions(@fminunc,'Display','iter','Algorithm','quasi-newton','MaxIter',1000,'OptimalityTolerance',1e-20);
x_al_r=zeros(21,21,5);
fv_al_r=zeros(21,21);
Init_value=[3,2,0.1,0.1,0.3];
for i=1:21
    for j=1:21
        [x_al_r(i,j,:),fv_al_r(i,j)]=fminunc(@residu_4,Init_value,options,dr_art,dr_vein,dr_liv,i-1,j-1);
    end
end
[a,b]=find(fv_al_r==min(min(fv_al_r)));
res=depic_4(x_al_r(a(1),b(1),:),dr_art,dr_vein,dr_liv,a(1),b(1));
aaaa=reshape(x_al_r(a(1),b(1),:),[1,5]);
dddd=fv_al_r(a(1),b(1));
%%
figure
plot([1:200],dr_liv,[1:200],dr_art,[1:200],dr_vein,[1:200],res)

%% 
[dr_liv_emd,residual_liv,~]=emd(medfilt1(dr_liv,3));
l_edm_liv=size(dr_liv_emd,2);
liv_emd_sum=dr_liv_emd(:,l_edm_liv)+dr_liv_emd(:,l_edm_liv-1)+dr_liv_emd(:,l_edm_liv-2)+dr_liv_emd(:,l_edm_liv-3)+dr_liv_emd(:,l_edm_liv-4)+residual_liv;
% figure
% plot([1:200],liv_emd_sum,[1:200],dr_liv)

[dr_art_emd,residual_art,~]=emd(dr_art);
l_edm_art=size(dr_art_emd,2);
art_emd_sum=dr_art_emd(:,l_edm_art)+dr_art_emd(:,l_edm_art-1)+dr_art_emd(:,l_edm_art-2)+dr_art_emd(:,l_edm_art-3)+residual_art;
figure
plot([1:200],art_emd_sum,[1:200],dr_art)




[dr_vein_emd,residual_vein,~]=emd(dr_vein);
l_edm_vein=size(dr_vein_emd,2);
vein_emd_sum=dr_vein_emd(:,l_edm_vein)+dr_vein_emd(:,l_edm_vein-1)+dr_vein_emd(:,l_edm_vein-2)+dr_vein_emd(:,l_edm_vein-3)+residual_vein;
figure
plot([1:200],vein_emd_sum,[1:200],dr_vein)
% figure
% plot(sum(dr_liv_emd'))


%% 

options = optimoptions(@fminunc,'Display','iter','Algorithm','quasi-newton','MaxIter',1000);
x_al_emd=zeros(21,21,4);
fv_al_emd=zeros(21,21);
for i=1:21
    for j=1:21
        [x_al_emd(i,j,:),fv_al_emd(i,j)]=fminunc(@residu,Init_value,options,art_emd_sum',l_edm_vein',liv_emd_sum',i-1,j-1);
    end
end
[a,b]=find(fv_al_emd==min(min(fv_al_emd)));
res=depic(x_al_emd(a(1),b(1),:),art_emd_sum',l_edm_vein',liv_emd_sum',a(1),b(1));














