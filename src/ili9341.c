#include <ili9341.h>

#define DMA_BUFFER_SIZE 256

void il_init(void)
{
    IL_CONFIG_GPIO_CLOCK();
    IL_CONFIG_GPIO();
    IL_CONFIG_SPI_DMA();
    IL_CONFIG_SPI();

    // 片选
    IL_CS_SET();

    // reset
    IL_RST_CLR();
    IL_RST_SET();
    IL_RST_CLR();
    for (uint32_t i = 0; i < 2000000; i++)
        __asm__("nop");

    // 初始化命令

    _il_write_command_8bit(0xCF);
    _il_write_data_8bit(0x00);
    _il_write_data_8bit(0xD9); // 0xC1
    _il_write_data_8bit(0X30);

    _il_write_command_8bit(0xED);
    _il_write_data_8bit(0x64);
    _il_write_data_8bit(0x03);
    _il_write_data_8bit(0X12);
    _il_write_data_8bit(0X81);

    _il_write_command_8bit(0xE8);
    _il_write_data_8bit(0x85);
    _il_write_data_8bit(0x10);
    _il_write_data_8bit(0x7A);

    _il_write_command_8bit(0xCB);
    _il_write_data_8bit(0x39);
    _il_write_data_8bit(0x2C);
    _il_write_data_8bit(0x00);
    _il_write_data_8bit(0x34);
    _il_write_data_8bit(0x02);

    _il_write_command_8bit(0xF7);
    _il_write_data_8bit(0x20);

    _il_write_command_8bit(0xEA);
    _il_write_data_8bit(0x00);
    _il_write_data_8bit(0x00);

    _il_write_command_8bit(0xC0); // Power control
    _il_write_data_8bit(0x1B);    // VRH[5:0]
    _il_write_command_8bit(0xC1); // Power control
    _il_write_data_8bit(0x12);    // SAP[2:0];BT[3:0] 0x01
    _il_write_command_8bit(0xC5); // VCM control
    _il_write_data_8bit(0x08);    // 30
    _il_write_data_8bit(0x26);    // 30
    _il_write_command_8bit(0xC7); // VCM control2
    _il_write_data_8bit(0XB7);
    _il_write_command_8bit(0x3A); // Pixel Format Set(16bits)
    _il_write_data_8bit(0x55);
    _il_write_command_8bit(0xB1); // Frame Rate Control
    _il_write_data_8bit(0x00);
    _il_write_data_8bit(0x1A);
    _il_write_command_8bit(0xB6); // Display Function Control
    _il_write_data_8bit(0x0A);
    _il_write_data_8bit(0xA2);
    _il_write_command_8bit(0xF2); // 3Gamma Function Disable
    _il_write_data_8bit(0x00);
    _il_write_command_8bit(0x26); // Gamma curve selected
    _il_write_data_8bit(0x01);

    _il_write_command_8bit(0xE0); // Set Gamma
    _il_write_data_8bit(0x0F);
    _il_write_data_8bit(0x1D);
    _il_write_data_8bit(0x1A);
    _il_write_data_8bit(0x0A);
    _il_write_data_8bit(0x0D);
    _il_write_data_8bit(0x07);
    _il_write_data_8bit(0x49);
    _il_write_data_8bit(0X66);
    _il_write_data_8bit(0x3B);
    _il_write_data_8bit(0x07);
    _il_write_data_8bit(0x11);
    _il_write_data_8bit(0x01);
    _il_write_data_8bit(0x09);
    _il_write_data_8bit(0x05);
    _il_write_data_8bit(0x04);

    _il_write_command_8bit(0XE1); // Set Gamma
    _il_write_data_8bit(0x00);
    _il_write_data_8bit(0x18);
    _il_write_data_8bit(0x1D);
    _il_write_data_8bit(0x02);
    _il_write_data_8bit(0x0F);
    _il_write_data_8bit(0x04);
    _il_write_data_8bit(0x36);
    _il_write_data_8bit(0x13);
    _il_write_data_8bit(0x4C);
    _il_write_data_8bit(0x07);
    _il_write_data_8bit(0x13);
    _il_write_data_8bit(0x0F);
    _il_write_data_8bit(0x2E);
    _il_write_data_8bit(0x2F);
    _il_write_data_8bit(0x05);

    _il_write_command_8bit(0x11); // Exit Sleep
    // Delay_Ms(120);
    _il_write_command_8bit(0x29); // display on
    _il_write_command_8bit(0x36); // Memory Access Control
    _il_write_data_8bit(0xE8);
}

void il_set_address_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _il_write_command_8bit(0X2A);

    CS_LOCK(
        IL_DC_DAT();
        IL_WRITE_8BIT((uint8_t)(x1 >> 8));
        IL_WRITE_8BIT((uint8_t)x1);

        IL_WRITE_8BIT((uint8_t)(x2 >> 8));
        IL_WRITE_8BIT((uint8_t)x2);)

    _il_write_command_8bit(0X2B);

    CS_LOCK(
        IL_DC_DAT();
        IL_WRITE_8BIT((uint8_t)(y1 >> 8));
        IL_WRITE_8BIT((uint8_t)y1);

        IL_WRITE_8BIT((uint8_t)(y2 >> 8));
        IL_WRITE_8BIT((uint8_t)y2);)

    _il_write_command_8bit(0x2C);
}

void il_fill_color_array(uint16_t color, uint32_t len)
{
    IL_CS_SET();

    IL_DC_DAT();
    uint8_t color_high = color >> 8;
    uint8_t color_low = color;

    uint16_t blocks = (uint16_t)(len / 64); // 64 pixels/block
    uint8_t pass_count;

    // Write first pixel
    IL_WRITE_8BIT(color_high);
    IL_WRITE_8BIT(color_low);
    len--;

    while (blocks--)
    {
        pass_count = 16;
        while (pass_count--)
        {
            IL_WRITE_8BIT(color_high);
            IL_WRITE_8BIT(color_low);
            IL_WRITE_8BIT(color_high);
            IL_WRITE_8BIT(color_low); // 2
            IL_WRITE_8BIT(color_high);
            IL_WRITE_8BIT(color_low);
            IL_WRITE_8BIT(color_high);
            IL_WRITE_8BIT(color_low); // 4
        }
    }
    pass_count = len & 63;
    while (pass_count--)
    {
        // write here the remaining data
        IL_WRITE_8BIT(color_high);
        IL_WRITE_8BIT(color_low);
    }
    IL_CS_CLR();
}

void il_fill_color_array_dma(uint16_t color, uint32_t len)
{
    CS_LOCK(
        IL_DC_DAT();
        uint8_t color_high = color >> 8;
        uint8_t color_low = color;

        uint8_t disp_buffer[DMA_BUFFER_SIZE];
        for (int i = 0; i < DMA_BUFFER_SIZE; i = i + 2) {
            disp_buffer[i] = color_high;
            disp_buffer[i + 1] = color_low;
        }

        // len is pixel count. But each pixel is 2 bytes. So, multiply by 2
        uint32_t bytes_to_write = len * 2;
        uint16_t transfer_size = DMA_BUFFER_SIZE;
        while (bytes_to_write) {
            transfer_size = (bytes_to_write < transfer_size) ? bytes_to_write : transfer_size;
            _il_write_spi_dma(disp_buffer, transfer_size);
            bytes_to_write -= transfer_size;
        })
}

void il_fill_screen(uint16_t color)
{
    il_set_address_window(0, 0, IL_HEIGHT - 1, IL_WIDTH - 1);
    il_fill_color_array_dma(color, (IL_HEIGHT * IL_WIDTH));
}

void il_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    il_set_address_window(x, y, x + w - 1, y + h - 1);
    il_fill_color_array_dma(color, (uint32_t)w * (uint32_t)h);
}

void il_set_scroll_area(uint16_t tfa, uint16_t bfa)
{
    CS_LOCK(
        _il_write_command_8bit(0x33);
        // TFS
        _il_write_data_8bit(tfa >> 8);
        _il_write_data_8bit(tfa);
        // VSA
        _il_write_data_8bit((IL_HEIGHT - tfa - bfa) >> 8);
        _il_write_data_8bit(IL_HEIGHT - tfa - bfa);
        // FBA
        _il_write_data_8bit(bfa >> 8);
        _il_write_data_8bit(bfa);)
}

void il_set_scroll_add(uint16_t vsp)
{
    CS_LOCK(
        _il_write_command_8bit(0x37);
        _il_write_data_8bit(vsp >> 8);
        _il_write_data_8bit(vsp);)
}