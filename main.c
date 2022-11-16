#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void Delay_us(uint32_t us);    // Ham delay theo thoi gian thuc, don vi: us (micro seconds)
void Delay_ms(uint32_t ms);    // Ham delay theo thoi gian thuc, don vi: ms (mili seconds)
void DelayInit(void);          // Khoi tao cac ham delay
void GPIO_Configuration(void); // Khai bao, thiet lap cac chan su dung
void SysTick_Handler();        // Khoi tao cho cac ham delay
uint8_t check_button();        // Neu nut duoc nhan thi tang thu tu hieu ung va tra ve 1
void load_effect();             // Chay hieu ung tuong ung
void effect_1(uint32_t ms);      // Nhap nhay tat ca cac LED
void effect_2(uint32_t ms);      // Hieu ung sang don
void snake_eff(uint32_t ms);     // Hieu ung ran chay
void butterfly_eff(uint32_t ms); // Hieu ung canh buom
void one_led_eff(uint32_t ms);   // Tung LED sang theo thu tu

static __IO uint32_t nTicks;  // Su dung cho ham delay
uint8_t i = 0;                // Su dung cho cac vong for
uint8_t get_state_button = 0; // Luu trang thai hien tai cua nut nhan
uint8_t old_sate_button = 0;  // Luu trang thai truoc do cua nut nhan
uint8_t effect = 0;           // Thu tu hieu ung
uint32_t delay_eff = 100;     // Thoi gian delay trong cac hieu ung

uint16_t eff_1[2] = {0x0000, 0xFFFF};
uint16_t butterfly_a[14] = {0x0, 0x40, 0xe0, 0x1f0, 0x3f8, 0x7fc, 0xffe, 0x1fff, 0xffe, 0x7fc, 0x3f8, 0x1f0, 0xe0, 0x40};
uint16_t snake_a[16] = {0x0, 0x1, 0x3, 0x7, 0xe, 0x1c, 0x38, 0x70, 0xe0, 0x1c0, 0x380, 0x700, 0xe00, 0x1c00, 0x9800, 0x9000};
uint16_t butterfly_b[14] = {0x0, 0x200, 0x700, 0xf80, 0x1fc0, 0x3fe0, 0x7fe2, 0xffe3, 0x7fe2, 0x3fe0, 0x1fc0, 0xf80, 0x700, 0x200};
uint16_t snake_b[16] = {0x0, 0x1, 0x3, 0x23, 0x62, 0xe0, 0x1c0, 0x380, 0x700, 0xe00, 0x1c00, 0x3800, 0x7000, 0xe000, 0xc000, 0x8000};
uint16_t one_led_a[26] = {0x0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x800, 0x400, 0x200, 0x100, 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};
uint16_t one_led_b[26] = {0x0, 0x1, 0x2, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x4000, 0x2000, 0x1000, 0x800, 0x400, 0x200, 0x100, 0x80, 0x40, 0x20, 0x2, 0x1};
uint16_t eff_2_a[26] = {0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff, 0x1ff, 0x3ff, 0x7ff, 0xfff, 0x1fff, 0xfff, 0x7ff, 0x3ff, 0x1ff, 0xff, 0x7f, 0x3f, 0x1f, 0xf, 0x7, 0x3, 0x1};
uint16_t eff_2_b[26] = {0x0, 0x1, 0x3, 0x23, 0x63, 0xe3, 0x1e3, 0x3e3, 0x7e3, 0xfe3, 0x1fe3, 0x3fe3, 0x7fe3, 0xffe3, 0x7fe3, 0x3fe3, 0x1fe3, 0xfe3, 0x7e3, 0x3e3, 0x1e3, 0xe3, 0x63, 0x23, 0x3, 0x1};

int main(void)
{
    DelayInit();               // ham khoi tao du lieu cho cac ham delay theo thoi gian thuc
    GPIO_Configuration();      // ham cau hinh ngo vao ra cho cac PORT
    GPIO_Write(GPIOA, 0x0000); // ban dau tat het LED
    while (1)
    {
       check_button();
       load_effect();
    }
}

void load_effect()
{
	 switch (effect) // chay cac hieu ung tuong ung voi bien effect
        {
        case 0:
            effect_1(delay_eff);
            break;
        case 1:
            effect_2(delay_eff);
            break;
        case 2:
            one_led_eff(delay_eff);
            break;
        case 3:
            butterfly_eff(delay_eff);
            break;
        case 4:
            snake_eff(delay_eff);
            break;
        }
}
void snake_eff(uint32_t ms)
{
    for (i = 0; i < 16; i++)
    {
        GPIO_Write(GPIOB, snake_b[i]); // Ghi bit tuong ung vao PORTA
        GPIO_Write(GPIOA, snake_a[i]); // Ghi bit tuong ung vao PORTB
        if (check_button())            // Kiem tra xem button co duoc nhan hay khong
            break;                     // Neu co thi dung hieu ung hien tai de chuyen den hieu ung tiep theo
        Delay_ms(ms);                  // Delay chuong trinh voi gia tri cho truoc
    }
}

void effect_1(uint32_t ms)
{
    for (i = 0; i < 2; i++)
    {
        GPIO_Write(GPIOA, eff_1[i]);
        GPIO_Write(GPIOB, eff_1[i]);
        if (check_button())
            break;
        Delay_ms(ms);
    }
}

void effect_2(uint32_t ms)
{

    for (i = 0; i < 26; i++)
    {
        GPIO_Write(GPIOA, eff_2_a[i]);
        GPIO_Write(GPIOB, eff_2_b[i]);
        if (check_button())
            break;
        Delay_ms(ms);
    }
}

void one_led_eff(uint32_t ms)
{
    for (i = 0; i < 26; i++)
    {
        GPIO_Write(GPIOA, one_led_a[i]);
        GPIO_Write(GPIOB, one_led_b[i]);
        if (check_button())
            break;
        Delay_ms(ms);
    }
}

void butterfly_eff(uint32_t ms)
{
    for (i = 0; i < 14; i++)
    {
        GPIO_Write(GPIOA, butterfly_a[i]);
        GPIO_Write(GPIOB, butterfly_b[i]);
        if (check_button())
            break;
        Delay_ms(ms);
    }
}

uint8_t check_button()
{
    uint8_t check = 0;                                            // Bien tra ve trang thai cua nut nhan
    get_state_button = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13); // Lay trang thai cua chan PC13
    if (get_state_button != old_sate_button)                      // Neu trang thai hien tai khac voi trang thai truoc do thi thuc hien lenh trong if
    {
        old_sate_button = get_state_button; // Gan trang thai hien tai cho bien old_sate_button de so sanh voi lan nhan tiep theo
        if (get_state_button == 1)          // Neu nut duoc nhan(tra ve bit 1) thi thuc hien lech trong if
        {
            check = !check;  // Doi trang thai cua bien check vi nut duoc nhan
            if (effect == 4) // Neu thu tu hieu ung da la cao nhat thi cho chay lai tu hieu ung dau tien
            {
                effect = 0;
            }
            else // Neu hien tai chua phai hieu ung cuoi thi nhay den hieu ung tiep theo
            {
                effect++;
            }
        }
    }
    return check; // Tra ve trang thai cua nut nhan(neu nut duoc nhan thi tra ve 1, nguoc lai 0)
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
    while (n--)
    {
        // Delay 1ms
        Delay_us(1000);
    }
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //==========================PORT====================================//

    // Khai bao su dung PORT-A, PORT-B va PORT-C
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    //=================================OUTPUT========================================
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;       // Khai bao su dung tat ca cac chan cua PORT-A
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // Che do chan la output
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // Thiet lap toc do ngo ra cac chan
    GPIO_Init(GPIOA, &GPIO_InitStructure);            // Tien hanh nap cac cai dat o tren

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;       // Khai bao su dung tat ca cac chan cua PORT-B
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // Che do chan la output
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // Thiet lap toc do ngo ra cac chan
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // Tien hanh nap cac cai dat o tren

    //================Button=======================//
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;    // Khai bao su dung chan 13 cua PORT-C
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // Che do chan la nhan tin hieu
    GPIO_Init(GPIOC, &GPIO_InitStructure);        // Tien hanh nap cac cai dat o tren
}