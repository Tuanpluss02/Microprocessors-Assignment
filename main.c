#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void DelayInit(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void GPIO_Configuration(void);
void SysTick_Handler();

void snake_eff_a(uint32_t ms);
void butterfly_eff_a(uint32_t ms);
void effect_1(uint32_t ms);
void effect_2(uint32_t ms);

static __IO uint32_t nTicks;

uint16_t delay_ms[5] = {100,200,500,1000,2000};

uint16_t eff_1[2] = {0x0000, 0xFFFF};
uint16_t eff_2[14] = { 0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff, 0x1ff, 0x3ff, 0x7ff, 0xfff, 0x1fff};
uint16_t butterfly_a[14] = { 0x0, 0x40, 0xe0, 0x1f0, 0x3f8, 0x7fc, 0xffe, 0x1fff, 0xffe, 0x7fc, 0x3f8, 0x1f0, 0xe0, 0x40};
uint16_t snake_a[17] = { 0x0, 0x1, 0x3, 0x7, 0xe, 0x1c, 0x38, 0x70, 0xe0, 0x1c0, 0x380, 0x700, 0xe00, 0x1c00, 0x9800, 0x9000, 0x8000};
	
	
uint8_t i = 0;
uint16_t x = 0x0000;
uint8_t stt_led = 0,  delay_level = 0;
uint8_t get_state_button = 0, old_sate_button = 0;


int main(void)
{
	DelayInit();											 // goi ham khoi tao du lieu cho cac ham delay
	GPIO_Configuration();									 // goi ham cau hinh ngo vao ra cho cac PORT
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // bo che do nap JTAGD
	GPIO_Write(GPIOA, 0x0000);								 // ban dau tat het LED
	x = 0X70;
	while (1)
	{	/*
		// CHONG NHIEU
		get_state_button = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
	if(get_state_button != old_sate_button)
		{
			old_sate_button = get_state_button;
			if(get_state_button == 1)
			{
				if(delay_level == 5)
					delay_level =0;
				else
					delay_level++;
			}
		}*/
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){
			if(delay_level == 5)
					delay_level = 0;
				else
					delay_level++;
		}
				effect_1(delay_ms[delay_level]);
				effect_2(delay_ms[delay_level]);
				snake_eff_a(delay_ms[delay_level]);
				butterfly_eff_a(delay_ms[delay_level]);
	}
}

void effect_1(uint32_t ms){
	for(i = 0;i < 2; i++){
		GPIO_Write(GPIOA, eff_1[i]);
		Delay_ms(ms);
}}
	
void effect_2(uint32_t ms){
	for(i = 0;i < 14; i++){
		GPIO_Write(GPIOA, eff_2[i]);
		Delay_ms(ms);}
}


void snake_eff_a(uint32_t ms){
		for(i = 0; i < 17;i++){
		GPIO_Write(GPIOA, snake_a[i]);
		Delay_ms(ms);
		}
}

void butterfly_eff_a(uint32_t ms){
		for(i = 0; i < 14;i++){
		GPIO_Write(GPIOA, butterfly_a[i]);
		Delay_ms(ms);
		}
}

void SysTick_Handler()
{
	if (nTicks != 0)
	{
		nTicks--;
	}
}

void DelayInit()
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000000); // cau hinh thoi gian ngat
}
void Delay_us(uint32_t n)
{
	nTicks = n;
	while (nTicks)
		;
}

void Delay_ms(uint32_t n)
{
	// Wait until ms reach zero
	while (n--)
	{
		// Delay 1ms
		Delay_us(1000);
	}
}
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// Enable clock AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/*==================================PORTB==========================================*/
	// Enable clock GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	//_________________________________OUTPUT__________________________________________
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // ngõ ra kieu day kéo
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // thiet lap toc do ngo ra cac chan
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/
	
	//==========Button=======================//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

