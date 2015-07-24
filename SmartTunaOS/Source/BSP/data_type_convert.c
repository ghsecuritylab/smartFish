
/*
*write by wsg 2015-1-7
*/
unsigned char change_dd(signed char d_c)
{
   unsigned char dd_c;
   if((d_c>=0)&&(d_c<=7))
   {
     dd_c=d_c;
   }
   if((d_c>=7)&&(d_c<=14))
   {
     dd_c=14-d_c;//7-(d_c-7);//
   }
   if(d_c==15)
   {
     dd_c=0;
   }
return dd_c;
}
unsigned char change_flag(unsigned char d_c)
{
   unsigned char flag_c;
   if(d_c<=6)
   {
   flag_c=1;
   }
   else
   {
   flag_c=0;
   }
   return flag_c;
}
//********************��unsigned char����ת��Ϊint����**************************
//flag=1ʱתΪ��������ת��flag=0ʱתΪ��������ת
int char2int(unsigned char a_char,unsigned char a_flag)
{
  int a_int;
  if(a_flag==0)
  {
     a_int=a_char;
  }
  if(a_flag==1)
  {
     a_int=a_char;
	 a_int=-a_int;
  }
return a_int;
}



//*********************һ��16λ���飬��Ϊ2��8λ����****************************
//������Ϊ16λ������data_init[16]���2������Ϊ8������
//�ֱ�Ϊdata_a[8],data_b[8]
//unsigned char��
void one2two(unsigned char *data_init,unsigned char *data_a,unsigned char *data_b)
{
  unsigned char s = 0;
  for(s=0;s<=7;s++)
   {
   data_a[s]=data_init[s];
   }
  for(s=8;s<=15;s++)
   {
   data_b[s-8]=data_init[s];
   }
}

//*******************��2��8λ������˳�����һ��16λ����************************
//��2������Ϊ8������data_aa[8],data_bb[8]
//˳�����һ������Ϊ16λ������data_inits[16]
//unsigned char��
void two2one(unsigned char *data_inits,unsigned char *data_aa,unsigned char *data_bb)
{
  unsigned char s = 0;
  for(s=0;s<=7;s++)
   {
   data_inits[s]=data_aa[s];//*��data_aa��ֵ˳�θ�ֵ��data_inits
   }
  for(s=0;s<=7;s++)
   {
   data_inits[s+8]=data_bb[s];//*��data_bb��ֵ˳�θ�ֵ��data_bb�ĸ߰�λ
   }
}

//*********************һ��24λ���飬��Ϊ3��8λ����****************************
//������Ϊ32λ������data_init[24]���3������Ϊ8������
//�ֱ�Ϊdata_a[8],data_b[8],data_c[8]
//unsigned char��
void one2three(unsigned char *data_init,unsigned char *data_a,unsigned char *data_b,unsigned char *data_c)
{
  unsigned char s = 0;
  for(s=0;s<=7;s++)
   {
   data_a[s]=data_init[s];//*��data_init[]��ǰ��λ�γ�һ���µ�����data_a[]
   }
  for(s=8;s<=15;s++)
   {
   data_b[s-8]=data_init[s];//*��data_init[]�ھ�λ����ʮ��λ�γ�һ���µ�����data_b[]
   }
  for(s=16;s<=23;s++)
   {
   data_c[s-16]=data_init[s];//*��data_init[]��ʮ��λ�����ڶ�ʮ��λ�γ�һ���µ�����data_c[]
   }
}

//*******************��3��8λ������˳�����һ��24λ����************************
//��3������Ϊ24������data_aa[8],data_bb[8],data_cc[8]
//˳�����һ������Ϊ32λ������data_inits[32]
//unsigned char��
void three2one(unsigned char *data_inits,unsigned char *data_aa,unsigned char *data_bb,unsigned char *data_cc)
{
  unsigned char s = 0;
  for(s=0;s<=7;s++)
   {
   data_inits[s]=data_aa[s];
   }
  for(s=0;s<=7;s++)
   {
   data_inits[s+8]=data_bb[s];
   }
  for(s=0;s<=7;s++)
   {
   data_inits[s+16]=data_cc[s];
   }
}


//*********************һ��48λ���飬��Ϊ6��8λ����****************************
//������Ϊ48λ������data_init[48]���6������Ϊ8������
//�ֱ�Ϊdata_a[8],data_b[8],data_c[8],data_d[8],data_e[8],data_f[8]
//unsigned char��
void one2six(unsigned char *data_init,unsigned char *data_a,unsigned char *data_b,unsigned char *data_c,unsigned char *data_d,unsigned char *data_e,unsigned char *data_f)
{
  unsigned char s = 0;
  for(s=0;s<=7;s++)
   {
   data_a[s]=data_init[s];
   }
  for(s=8;s<=15;s++)
   {
   data_b[s-8]=data_init[s];
   }
  for(s=16;s<=23;s++)
   {
   data_c[s-16]=data_init[s];
   }
  for(s=24;s<=31;s++)
   {
   data_d[s-24]=data_init[s];
   }
  for(s=32;s<=39;s++)
   {
   data_e[s-32]=data_init[s];
   }
  for(s=40;s<=47;s++)
   {
   data_f[s-40]=data_init[s];
   }
}

//*******************��6��8λ������˳�����һ��48λ����************************
//��6������Ϊ8������data_aa[8],data_bb[8],data_cc[8],data_dd[8]
//˳�����һ������Ϊ48λ������data_inits[48]
//unsigned char��
void six2one(unsigned char *data_inits,unsigned char *data_aa,unsigned char *data_bb,unsigned char *data_cc,unsigned char *data_dd,unsigned char *data_ee,unsigned char *data_ff)
{
  unsigned char s = 0;
  for(s=0;s<=7;s++)
   {
   data_inits[s]=data_aa[s];
   }
  for(s=0;s<=7;s++)
   {
   data_inits[s+8]=data_bb[s];
   }
  for(s=0;s<=7;s++)
   {
   data_inits[s+16]=data_cc[s];
   }
  for(s=0;s<=7;s++)
   {
   data_inits[s+24]=data_dd[s];
   }
  for(s=0;s<=7;s++)
   {
   data_inits[s+32]=data_ee[s];
   }
  for(s=0;s<=7;s++)
   {
   data_inits[s+40]=data_ff[s];
   }
}
//buf;
unsigned char* Hex2String(unsigned char *pSrc,unsigned char nL)  
{  
    static unsigned char *buf; //= (char*)alloc(nL*2,sizeof(char));  
     
    signed char hb;  
    signed char lb;  
 
    for(int i=0;i<nL;i++)  
   {  
       hb=(pSrc[i]&0xf0)>>4;  
 
        if( hb>=0 && hb<=9 )  
           hb += 0x30;  
       else if( hb>=10 &&hb <=15 )  
           hb = hb -10 + 'A';  
        lb = pSrc[i]&0x0f;  
        if( lb>=0 && lb<=9 )  
            lb += 0x30;  
       else if( lb>=10 && lb<=15 )  
           lb = lb - 10 + 'A';  
     
	    buf[i*2]   = hb;  
        buf[i*2+1] = lb;  
    } 
   return buf; 
}  
