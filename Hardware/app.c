#include "app.h"

uint8_t humidity=0;
uint8_t tempreture=0;
uint32_t CO2=0;
uint8_t motor_running=0;
uint8_t temp_threshold=30;
uint32_t co2_threshold=3000;
uint16_t Cherry=0;

extern uint8_t Motor_time;
extern uint8_t CO2_Data[6];

void oled_show(void);
void key_deal(void);
void relay_control(void);

void app_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//1设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LED_Init();
	usart1_init(9600);
	usart3_init(9600);
	TIM1_Int_Init(9,7199);
//	TIM4_Int_Init(10000-1,7200-1);

	userInit();
	gizwitsInit();//机智云初始化
	
	OLED_Init();
	Key_Init();
	DHT11_Init();
	Motor_Init();
	
	Init_HX711pin();
	Get_Maopi();				//称毛皮重量
	Delay_ms(1000);
	Delay_ms(1000);
	Get_Maopi();
}

void app_run(void)
{
	oled_show();
	key_deal();
	relay_control();
	if(KEY1==RESET)
	{
		Delay_ms(1000);
		LED_ON();
		gizwitsSetMode(WIFI_AIRLINK_MODE);
	}

	userHandle();//数据上行
	gizwitsHandle((dataPoint_t *)&currentDataPoint);
}

void Parameter_collection(void)
{
	DHT11_Read_Data(&tempreture,&humidity);
	Get_Weight();
}

void oled_show(void)
{
	char arr[20];
	Parameter_collection();
	
	OLED_ShowString(0,0,"Temp:",16);
	OLED_ShowNum(40,0,tempreture,2,16);
	OLED_ShowString(0,2,"Humi:",16);
	OLED_ShowNum(40,2,humidity,2,16);
	
	OLED_ShowString(0,4,"CO2:",16);
	if(CO2_Data[5]==(uint8_t)(CO2_Data[0]+CO2_Data[1]+CO2_Data[2]+CO2_Data[3]+CO2_Data[4]))
	{
		CO2=CO2_Data[1]*256+CO2_Data[2];
		OLED_ShowNum(40,4,CO2,5,16);
  }
	else
	{
		OLED_ShowString(40,4,"ERROR",16);
	}

	Cherry=Weight_Shiwu/15;
	sprintf(arr,"Cherry:%.3d",Cherry);
	OLED_ShowString(0,6,arr,16);
}

void key_deal(void)
{
	uint8_t keynum=Key_GetNum();
	
	if(keynum)
		motor_running=1;
	
	if(keynum==1)
		Motor_SetSpeed(100);
	else if(keynum==2)
		Motor_SetSpeed(-100);
	
	if(motor_running)
	{
		Motor_time++;
		if(Motor_time>50)
		{
			Motor_SetSpeed(0);
			motor_running=0;
			Motor_time=0;
		}
	}
}

void relay_control(void)
{
	if(tempreture>temp_threshold||CO2>co2_threshold)
	{
		Buzzer_ON();
		if(tempreture>temp_threshold)
			REFRIG_ON();
		if(CO2>co2_threshold)
			Fan_ON();
	}
	else
	{
		Buzzer_OFF();
		Fan_OFF();
		REFRIG_OFF();
	}
}
