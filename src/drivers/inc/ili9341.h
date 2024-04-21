#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/dma.h>
#include <stdlib.h>

#ifndef ILI9341
#define ILI9341

// gpio接口

#define IL_SPI SPI1
#define IL_DMA DMA1
#define IL_DMA_CHANNEL 3
#define IL_PORT GPIOA
#define IL_RST GPIO4
#define IL_SDA GPIO7
#define IL_SCL GPIO5
#define IL_BLK GPIO3
#define IL_CS GPIO6
#define IL_DC GPIO2

// dc rst cs 操作

#define IL_DC_CMD() GPIO_BRR(IL_PORT) = IL_DC
#define IL_DC_DAT() GPIO_BSRR(IL_PORT) = IL_DC

#define IL_RST_SET() GPIO_BRR(IL_PORT) = IL_RST
#define IL_RST_CLR() GPIO_BSRR(IL_PORT) = IL_RST

#define IL_CS_SET() GPIO_BRR(IL_PORT) = IL_CS
#define IL_CS_CLR() GPIO_BSRR(IL_PORT) = IL_CS

#define CS_LOCK(x)     \
	{                  \
		IL_CS_SET();   \
		x IL_CS_CLR(); \
	}

/**
 * gpio时钟设置
 */
#define IL_CONFIG_GPIO_CLOCK()              \
	{                                       \
		rcc_periph_clock_enable(RCC_GPIOA); \
		rcc_periph_clock_enable(RCC_AFIO);  \
		rcc_periph_clock_enable(RCC_SPI1);  \
		rcc_periph_clock_enable(RCC_DMA1);  \
	}

/**
 * gpio设置
 */
#define IL_CONFIG_GPIO()                                                                                            \
	{                                                                                                               \
		gpio_set_mode(IL_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, IL_SCL | IL_SDA);           \
		gpio_set_mode(IL_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, IL_DC | IL_BLK | IL_RST | IL_CS); \
		gpio_set(IL_PORT, IL_BLK | IL_RST | IL_DC | IL_CS);                                                         \
		AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST;                                                          \
	}

/**
 * spi设置
 *
 * 分频为FPCLK/2
 * 时钟极性为时钟空闲时为高电平（CPOL=1）
 * 时钟相位为第一个时钟跳变沿（CPHA=0）
 * 数据帧格式为8位（DFF=8BIT
 * 数据传输的高位先传输（MSBFIRST）
 */
#define IL_CONFIG_SPI()                                                                                                                                            \
	{                                                                                                                                                              \
		spi_reset(IL_SPI);                                                                                                                                         \
		SPI_I2SCFGR(IL_SPI) = 0;                                                                                                                                   \
		spi_init_master(IL_SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_2, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST); \
		spi_enable_software_slave_management(IL_SPI);                                                                                                              \
		spi_set_nss_high(IL_SPI);                                                                                                                                  \
		spi_enable(IL_SPI);                                                                                                                                        \
	}

/**
 * spi 使用的dma通道设置
 */
#define IL_CONFIG_SPI_DMA()                                                         \
	{                                                                               \
		DMA_CPAR(IL_DMA, IL_DMA_CHANNEL) = (uint32_t) & SPI_DR(IL_SPI);             \
		DMA_CMAR(IL_DMA, IL_DMA_CHANNEL) = 0;                                       \
		DMA_CNDTR(IL_DMA, IL_DMA_CHANNEL) = 0;                                      \
		DMA_CCR(IL_DMA, IL_DMA_CHANNEL) = DMA_CCR_PL_HIGH;                          \
		DMA_CCR(IL_DMA, IL_DMA_CHANNEL) |= DMA_CCR_DIR;                             \
		DMA_CCR(IL_DMA, IL_DMA_CHANNEL) &= ~DMA_CCR_CIRC;                           \
		DMA_CCR(IL_DMA, IL_DMA_CHANNEL) &= ~DMA_CCR_PINC;                           \
		DMA_CCR(IL_DMA, IL_DMA_CHANNEL) |= DMA_CCR_MINC;                            \
		DMA_CCR(IL_DMA, IL_DMA_CHANNEL) |= DMA_CCR_PSIZE_8BIT | DMA_CCR_MSIZE_8BIT; \
	}

/**
 * spi发送一个8位数据
 */
#define IL_WRITE_8BIT(d)               \
	do                                 \
	{                                  \
		SPI_DR(IL_SPI) = (uint8_t)(d); \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
		__asm__("nop");                \
	} while (0)

/**
 * spi使用dma发送数据
 */
__attribute__((always_inline)) static inline void _il_write_spi_dma(void *data_addr, uint16_t length)
{
	DMA_CMAR(IL_DMA, IL_DMA_CHANNEL) = (uint32_t)data_addr;
	DMA_CNDTR(IL_DMA, IL_DMA_CHANNEL) = length;
	DMA_CCR(IL_DMA, IL_DMA_CHANNEL) |= DMA_CCR_EN;
	SPI_CR2(IL_SPI) |= SPI_CR2_TXDMAEN;
	while (DMA_CNDTR(IL_DMA, IL_DMA_CHANNEL))
		;
	while (!(SPI_SR(IL_SPI) & SPI_SR_TXE))
		;
	while (SPI_SR(IL_SPI) & SPI_SR_BSY)
		;
	SPI_CR2(IL_SPI) &= ~SPI_CR2_TXDMAEN;
	DMA_CCR(IL_DMA, IL_DMA_CHANNEL) &= ~DMA_CCR_EN;
}

/**
 * spi命令发送
 */
__attribute__((always_inline)) static inline void _il_write_command_8bit(uint8_t comamnd)
{
	CS_LOCK(
		IL_DC_CMD();
		IL_WRITE_8BIT(comamnd););
}

/**
 * spi数据发送
 */
__attribute__((always_inline)) static inline void _il_write_data_8bit(uint8_t data)
{
	CS_LOCK(
		IL_DC_DAT();
		IL_WRITE_8BIT(data);)
}

/**
 * spi数据发送16bit
 */
__attribute__((always_inline)) static inline void _il_write_data_16bit(uint16_t data)
{
	CS_LOCK(
		IL_DC_DAT();
		IL_WRITE_8BIT((uint8_t)(data >> 8));
		IL_WRITE_8BIT((uint8_t)data););
}

// size
#define IL_WIDTH 240
#define IL_HEIGHT 320

// functions define

/**
 * @brief il9341初始化
 */
void il_init(void);

/**
 * @brief 设置屏幕滚动区域
 */
void il_set_scroll_area(uint16_t tfa, uint16_t bfa);

/**
 * @brief 设置屏幕滚动值
 */
void il_set_scroll_add(uint16_t vsp);

/**
 * @brief 准备一个矩形地址用于写入颜色
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void il_set_address_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * @brief 发送颜色数据
 */
void il_fill_color_array(uint16_t color, uint32_t len);

/**
 * @brief 以dma方式发送颜色数据
 */
void il_fill_color_array_dma(uint16_t color, uint32_t len);

/**
 * @brief 填充整个屏幕
 */
void il_fill_screen(uint16_t color);

/**
 * @brief 填充一个矩形
 */
void il_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * @brief 用dma填充一个矩形
 */
void il_fill_rect_dma(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

#endif
