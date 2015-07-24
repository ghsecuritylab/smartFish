/**************************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
* File : APP.C
* Author: wangsg,Meano
* QQ:591650928,859592326
* Email:www_wsg@163.com,limy@lzrobot.com
**************************************************************************************************************/
#include  <includes.h>

//������

//uCOS�����ջ
OS_STK  AppTaskStartStk[OS_TASK_START_STK_SIZE];
OS_STK  AppTask1Stk[OS_TASK_1_STK_SIZE];
OS_STK  AppTask2Stk[OS_TASK_2_STK_SIZE]; 
OS_STK  AppTask3Stk[OS_TASK_3_STK_SIZE]; 

//uCOS�ź���
OS_EVENT* FishUartDataSem;
OS_EVENT* ALIGNSem;
OS_EVENT* TIMER3_Ovf_Sem;

//��־����
INT8U fReceive = 0;
INT8U ALIGN;

//�ⲿ��չ����
extern INT8U UART0_ARRY[4];
extern INT8U EX_SPEED;
extern INT8U Speed_tem_z[16];
extern INT8U Static_Offset_degree[3];
extern INT8U Static_Offset_degree_init[3];
extern INT8U FISHID[1];
extern int Joint_Angle_value [3]; 
extern INT8U EX_DIRECTION;
extern INT8U Dynamic_Offset_degree_origin[8][3];
extern INT8U Amplitude_degree_tem[16][3];
extern INT8U Neighbouring_Offset_degree[3];

//��������
static void  AppTaskStart(void *p_arg);
static void  AppTaskCreate(void);
static void  AppTask1(void *p_arg);
static void  AppTask2(void *p_arg);
static void  AppTask3(void *p_arg);

//������
int  main (void)
{
    #if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
        INT8U  err;
    #endif

    OSTaskStkSize     = OS_TASK_IDLE_STK_SIZE;                          //����Ĭ�϶�ջ��С
    OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;                          //����Ĭ��Ӳ����ջ��С
    OSInit();                                                           //��ʼ��"uC/OS-II, Ƭ��ʵʱ����ϵͳ" 
    OSTaskStkSize     = OS_TASK_START_STK_SIZE;                         //�����ܵĶ�ջ��С
    OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;                          //����Ĭ��Ӳ����ջ��С
    OSTaskCreateExt(AppTaskStart,                                       //������
                    (void *)0,                                          //���ݲ���
                    (OS_STK *)&AppTaskStartStk[OSTaskStkSize - 1],      //�����ջָ��
                    OS_TASK_START_PRIO,                                 //�������ȼ�
                    0,                                                  //���� id��չ
                    (OS_STK *)&AppTaskStartStk[OSTaskStkSizeHard],      //ջ��ָ��
                    OSTaskStkSize - OSTaskStkSizeHard,                  //��ջ��С
                    (void *)0,                                          //�ṹָ��
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);         //������������Ϣ

    #if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
        OSTaskNameSet(OS_TASK_START_PRIO, "Start Task", &err);          //������������ΪStart Task
    #endif
    OSStart();                                                          //ϵͳ��ʼ����
}

//�����ʼ��������ϵͳ������
static void  AppTaskStart (void *p_arg)
{
    INT8U error;                                                        //��ֹ���뾯��
    BSP_Init();                                                         //ʱ�ӳ�ʼ��
    TWI_Master_Initialise();                                            //IIC��ʼ��
    USART0Init();                                                       //����0��ʼ��
    Init_Devices();                                                     //Ƭ��Ӳ����ʼ��
    AppTaskCreate();                                                    //������������
    FishUartDataSem = OSSemCreate(1);                                   //���ڽ�������ź���
    TIMER3_Ovf_Sem= OSSemCreate(1);                                     //��ʱ��3������ź���
    ALIGNSem = OSSemCreate(1);                                          //���յ�ֱ�����ź���
    Init_RobotFish_Data();                                              //���������ݳ�ʼ���������ⲿ�洢���е�����
    EX_DIRECTION=0x07;                                                  //��ʼ״̬���м�λ��
    
    USART1_Init();
    
    while (1) {    	
        OSSemPend(FishUartDataSem,0,&error);                            //�ȴ�����0��������ź� 
        if((fReceive==2)&&((UART0_ARRY[1]&0x0F)==FISHID[0]))
        {
            INT8U temp= 0;
            switch (UART0_ARRY[2]&0Xf0)
            {	  
                case 0xd0:      //speed
                    EX_SPEED = (UART0_ARRY[2]&0x0f);//
                    break;
                case 0Xe0:      //direction
                    EX_DIRECTION = (UART0_ARRY[2]&0x0f);
                    break;
                case 0x00:      //align
                    ALIGN = (UART0_ARRY[2]&0x0f);
                    OSSemPost(ALIGNSem);
                    break;
                case 0X10:      // keep the align
                    if(UART0_ARRY[2]==0x1C)
                    {
                        fish_write_align_data();
                        for(INT8U i=0;i<3;i++)
                        {
                            USART0_Transmit(Static_Offset_degree[i]);
                        }
                    }
                    else if(UART0_ARRY[2]==0x18)
                    {
                        fish_reset_align_data();
                        for(INT8U i=0;i<3;i++)
                        {
                            USART0_Transmit(Static_Offset_degree[i]);
                        }
                    }
                    USART0_Transmit('\n');
                    break;
                case 0x20:      //change the fish id 
                    FISHID[0]=UART0_ARRY[2]&0x0f;
                    fish_write_id_data();
                    USART0_Transmit(FISHID[0]);
                    break;
                case 0x40:      //change the fish frequency ���Ļ������Ƶ��
                    fish_write_frequency((UART0_ARRY[2]&0x0f)-1);
                    break; 
                case 0xb0:
                    switch (UART0_ARRY[2]&0x0f)
                    {
                        case 0x01://�ı��Ӧ��λ���ٶ�ֵ�������ȷ����ٶ�ָ��
                            if(Speed_tem_z[EX_SPEED]<=30)
                            {
                                Speed_tem_z[EX_SPEED]++;
                            }else
                                Speed_tem_z[EX_SPEED]--;
                            break;
                        case 0x02://�ı��Ӧ��λ���ٶ�ֵ�������ȷ����ٶ�ָ��
                            if(Speed_tem_z[EX_SPEED]>=254)
                            {
                                Speed_tem_z[EX_SPEED]--;
                            }
                                Speed_tem_z[EX_SPEED]++;
                            break;
                        case 0x03://�ı��Ӧ����λ��ƫ�ƽǶȣ������ȷ��ͷ���λ
                            temp = EX_DIRECTION;
                            if(temp>7)
                            {
                                if (temp >14)
                                {
                                     temp = 14;
                                }
                                temp = 14-temp;
                            }
                            if(Dynamic_Offset_degree_origin[temp][0]>0)
                            Dynamic_Offset_degree_origin[temp][0]--;
                            break;
                        case 0x04://�ı��Ӧ����λ��ƫ�ƽǶȣ������ȷ��ͷ���λ
                            temp = EX_DIRECTION;
                            if(temp>7)
                            {
                                if (temp >14)
                                {
                                    temp = 14;
                                }
                                temp = 14-temp;
                            }
                            if(Dynamic_Offset_degree_origin[temp][0]<255)
                            Dynamic_Offset_degree_origin[temp][0]++;
                            break;
                        case 0x05://�ı��Ӧ����λ��ƫ�ƽǶȣ������ȷ��ͷ���λ
                            temp = EX_DIRECTION;
                            if(temp>7)
                            {
                                if (temp >14)
                                {
                                    temp = 14;
                                }
                                temp = 14-temp;
                            }
                            if(Dynamic_Offset_degree_origin[temp][0]>0)
                            Dynamic_Offset_degree_origin[temp][1]--;
                            break;
                        case 0x06://�ı��Ӧ����λ��ƫ�ƽǶȣ������ȷ��ͷ���λ
                            temp = EX_DIRECTION;
                            if(temp>7)
                            {
                                if (temp >14)
                                {
                                    temp = 14;
                                }
                                temp = 14-temp;
                            }
                            if(Dynamic_Offset_degree_origin[temp][0]<255)
                            Dynamic_Offset_degree_origin[temp][1]++;
                            break;
                        case 0x07://�ı��Ӧ����λ��ƫ�ƽǶȣ������ȷ��ͷ���λ
                            temp = EX_DIRECTION;
                            if(temp>7)
                            {
                                if (temp >14)
                                {
                                    temp = 14;
                               }
                               temp = 14-temp;
                            }
                            if(Dynamic_Offset_degree_origin[temp][0]<255)
                            Dynamic_Offset_degree_origin[temp][2]--;
                            break;
                        case 0x08://�ı��Ӧ����λ��ƫ�ƽǶȣ������ȷ��ͷ���λ
                            temp = EX_DIRECTION;
                            if(temp>7)
                            {
                                if (temp >14)
                                {
                                    temp = 14;
                                }
                                temp = 14-temp;
                            }
                            if(Dynamic_Offset_degree_origin[temp][0]<255)
                            Dynamic_Offset_degree_origin[temp][2]++;
                            break;
                        default:
                            break;
                    }
                    break;
                case 0xc0:
                    switch (UART0_ARRY[2]&0x0f)
                    {
                        case 0x01://�ı��Ӧ�ٶȵ�λ�ķ���ֵ�������ȷ����ٶ�ָ��
                            if( Amplitude_degree_tem[EX_SPEED][0]>0)
                            Amplitude_degree_tem[EX_SPEED][0]--;
                            break;
                        case 0x02://�ı��Ӧ�ٶȵ�λ�ķ���ֵ�������ȷ����ٶ�ָ��
                            if( Amplitude_degree_tem[EX_SPEED][0]<90)
                            Amplitude_degree_tem[EX_SPEED][0]++;
                            break;
                        case 0x03://�ı��Ӧ�ٶȵ�λ�ķ���ֵ�������ȷ����ٶ�ָ��
                            if( Amplitude_degree_tem[EX_SPEED][1]>0)
                            Amplitude_degree_tem[EX_SPEED][1]--;
                            break;
                        case 0x04://�ı��Ӧ�ٶȵ�λ�ķ���ֵ�������ȷ����ٶ�ָ��
                            if( Amplitude_degree_tem[EX_SPEED][1]<90)
                            Amplitude_degree_tem[EX_SPEED][1]++;
                            break;
                        case 0x05://�ı��Ӧ�ٶȵ�λ�ķ���ֵ�������ȷ����ٶ�ָ��
                            if( Amplitude_degree_tem[EX_SPEED][2]>0)
                            Amplitude_degree_tem[EX_SPEED][2]--;
                            break;
                        case 0x06://�ı��Ӧ�ٶȵ�λ�ķ���ֵ�������ȷ����ٶ�ָ��
                            if( Amplitude_degree_tem[EX_SPEED][2]<90)
                            Amplitude_degree_tem[EX_SPEED][2]++;
                            break;
                        case 0x07://�ı�ڶ��ؽ���λ��
                            if( Neighbouring_Offset_degree[1]>0)
                            Neighbouring_Offset_degree[1]--;
                            break;
                        case 0x08://�ı�ڶ��ؽ���λ��
                            if( Neighbouring_Offset_degree[1]<255)
                            Neighbouring_Offset_degree[1]++;
                            break;
                        case 0x09://�ı�����ؽ���λ��
                            if( Neighbouring_Offset_degree[2]>0)
                            Neighbouring_Offset_degree[2]--;
                            break;
                        case 0x00://�ı�����ؽ���λ��
                            if( Neighbouring_Offset_degree[2]<255)
                            Neighbouring_Offset_degree[2]++;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if(UART0_ARRY[1]==0x91)//�������
        {
            if(UART0_ARRY[2]==0x66)
            {
                 fish_write_parameter();//�����ٶȡ����ȡ�ƫ�ƽǶȲ���
            }
            if(UART0_ARRY[2]==0x99)
            {
                system_write_i2c_data();//�ָ���������
                system_read_i2c_data();
            }
        }
        fReceive=0;
        IO_Toggle(B,2);//֪ʶ����˸
    }
}


//���񴴽�����
static  void  AppTaskCreate (void)
{
#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    INT8U  err;
#endif
    /*---- Task initialization code goes HERE! --------------------------------------------------------*/
    OSTaskStkSize     = OS_TASK_1_STK_SIZE;        /* Setup the default stack size                     */
    OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;     /* Setup the default hardware stack size            */
    OSTaskCreateExt(AppTask1,
                    (void *)0,
                    (OS_STK *)&AppTask1Stk[OSTaskStkSize - 1],
                    OS_TASK_1_PRIO,
                    OS_TASK_1_PRIO,
                    (OS_STK *)&AppTask1Stk[OSTaskStkSizeHard],
                    OSTaskStkSize - OSTaskStkSizeHard,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_TASK_1_PRIO, "Task 1", &err);
#endif

    /*---- Task initialization code goes HERE! --------------------------------------------------------*/
    OSTaskStkSize     = OS_TASK_2_STK_SIZE;        /* Setup the default stack size                     */
    OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;     /* Setup the default hardware stack size            */
    OSTaskCreateExt(AppTask2,
                    (void *)0,
                    (OS_STK *)&AppTask2Stk[OSTaskStkSize - 1],
                    OS_TASK_2_PRIO,
                    OS_TASK_2_PRIO,
                    (OS_STK *)&AppTask2Stk[OSTaskStkSizeHard],
                    OSTaskStkSize - OSTaskStkSizeHard,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_TASK_2_PRIO, "Task 2", &err);
#endif
        /*---- Task initialization code goes HERE! --------------------------------------------------------*/
    OSTaskStkSize     = OS_TASK_3_STK_SIZE;        /* Setup the default stack size                     */
    OSTaskStkSizeHard = OS_TASK_STK_SIZE_HARD;     /* Setup the default hardware stack size            */
    OSTaskCreateExt(AppTask3,
                    (void *)0,
                    (OS_STK *)&AppTask3Stk[OSTaskStkSize - 1],
                    OS_TASK_3_PRIO,
                    OS_TASK_3_PRIO,
                    (OS_STK *)&AppTask3Stk[OSTaskStkSizeHard],
                    OSTaskStkSize - OSTaskStkSizeHard,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_TASK_3_PRIO, "Task 3", &err);
#endif
}

//����1����ֱ����
static void  AppTask1(void *p_arg) 
{
    (void)p_arg;
    INT8U error;
    while (1) {
        OSSemPend(ALIGNSem,0,&error);               //�ȴ���ֱ�ź���
        INT8U tem;
        tem = ALIGN&0X07;   
        if((ALIGN&0X08)==0x00)
        {
            if(Static_Offset_degree[tem]>= 120)
            {
                Static_Offset_degree[tem]--;		
            }
            Static_Offset_degree[tem]++;
        }
        else
        {
            if(Static_Offset_degree[tem] <= 0)
            {
                Static_Offset_degree[tem]++;		
            }
            Static_Offset_degree[tem]--;
        }
        USART0_Transmit(Static_Offset_degree[tem]);
    }
}

//����2������Ƕȼ�������
static void  AppTask2(void *p_arg)
{
    (void)p_arg;
    INT8U error;
    while (1) {
        OSSemPend(TIMER3_Ovf_Sem,0,&error);
        if(fReceive<2){
            fReceive++;
        }else if(fReceive>2){
            fReceive=2;
        }
        calculate_data();
        OCR1A=Joint_Angle_value[0];	
        OCR1B=Joint_Angle_value[1];
        OCR3A=Joint_Angle_value[2]; 
    }
}

//����3���û���չ����
static void  AppTask3(void *p_arg)
{
    (void)p_arg;
    while (1) {
        //�ڴ���չ����3������ʹ��OSTimeDly(delaytime)��Ϊ��ʱ�������Ҳ���ʹ����ѭ��
        while(USART1_Receive() != 0x91)OSTimeDly(1);
        unsigned char rec = USART1_Receive();
        switch (rec&0xF0)
        {
            case 0xD0:
                EX_SPEED = rec & 0x0F;
                break;
            case 0xE0:
                EX_DIRECTION = rec & 0x0F;
                break;
            default:
                break;
        }
        OSTimeDly(1);
    }
}
