function resi=residu(ng,art,vein,liv,ta,tp)
    if ta>0
        art_pad=[zeros(1,ta),art(1:200-ta)];
    else
        art_pad=art;
    end
    
    if tp>0
        vein_pad=[zeros(1,tp),art(1:200-tp)];
    else
        vein_pad=vein;
    end
    fa=ng(1);
    fp=ng(2);
    ki=ng(3);
    ve=ng(4);
    f=fa+fp+ki;
    dt=1.8;
    Q=1-f*dt/ve;
    temp_l=200;
    seria=ones(1,temp_l)*ki*dt/f+(1-ki/f)/(1-f*dt/ve)*dt*Q.^[1:temp_l];
    seria(1)=seria(1)+(ki*dt-ve)/(ve-f*dt);
    g=fa*art_pad+fp*vein_pad;
    res_p=conv(g,seria);
    res=res_p(1:200);
    resi=norm(liv-res,2);
end