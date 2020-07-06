#pragma once

#include "ePaper/util.hpp"

/*------------------------------------------------------------------------------------------------------*/
class piDriver
{
public:
    piDriver *getInstance()
    {
        static piDriver *ret = new piDriver();
        return ret;
    }
    /******************************************************************************
    function:	Write GPIO
    parameter:
    Info:   
    ******************************************************************************/
    void DEV_Digital_Write(UWORD Pin, UBYTE Value)
    {
#ifdef USE_BCM2835_LIB
        bcm2835_gpio_write(Pin, Value);
#elif USE_WIRINGPI_LIB
        digitalWrite(Pin, Value);
#elif USE_DEV_LIB
        SYSFS_GPIO_Write(Pin, Value);
#endif
    }

    /******************************************************************************
    function:	Read Write GPIO
    parameter:
    Info:   return  IO status
    ******************************************************************************/
    UBYTE DEV_Digital_Read(UWORD Pin)
    {
        UBYTE Read_value = 0;
#ifdef USE_BCM2835_LIB
        Read_value = bcm2835_gpio_lev(Pin);
#elif USE_WIRINGPI_LIB
        Read_value = digitalRead(Pin);
#elif USE_DEV_LIB
        Read_value = SYSFS_GPIO_Read(Pin);
#endif
        return Read_value;
    }

    /******************************************************************************
function:	Set GPIO mode
parameter:
Info:
******************************************************************************/
    void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
    {
#ifdef USE_BCM2835_LIB
        if (Mode == 0 || Mode == BCM2835_GPIO_FSEL_INPT)
        {
            bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_INPT);
        }
        else
        {
            bcm2835_gpio_fsel(Pin, BCM2835_GPIO_FSEL_OUTP);
        }
#elif USE_WIRINGPI_LIB
        if (Mode == 0 || Mode == INPUT)
        {
            pinMode(Pin, INPUT);
            pullUpDnControl(Pin, PUD_UP);
        }
        else
        {
            pinMode(Pin, OUTPUT);
            // Debug (" %d OUT \r\n",Pin);
        }
#elif USE_DEV_LIB
        SYSFS_GPIO_Export(Pin);
        if (Mode == 0 || Mode == SYSFS_GPIO_IN)
        {
            SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_IN);
            // Debug("IN Pin = %d\r\n",Pin);
        }
        else
        {
            SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_OUT);
            // Debug("OUT Pin = %d\r\n",Pin);
        }
#endif
    }

    /******************************************************************************
function:	Initialization pin
parameter:
Info:
******************************************************************************/
    void DEV_GPIOConfig(void)
    {
        //output
        DEV_GPIO_Mode(EPD_SCK_PIN, 1);
        DEV_GPIO_Mode(EPD_MOSI_PIN, 1);

        DEV_GPIO_Mode(EPD_M1_CS_PIN, 1);
        DEV_GPIO_Mode(EPD_S1_CS_PIN, 1);
        DEV_GPIO_Mode(EPD_M2_CS_PIN, 1);
        DEV_GPIO_Mode(EPD_S2_CS_PIN, 1);

        DEV_GPIO_Mode(EPD_M1S1_DC_PIN, 1);
        DEV_GPIO_Mode(EPD_M2S2_DC_PIN, 1);

        DEV_GPIO_Mode(EPD_M1S1_RST_PIN, 1);
        DEV_GPIO_Mode(EPD_M2S2_RST_PIN, 1);

        //intput
        DEV_GPIO_Mode(EPD_M1_BUSY_PIN, 0);
        DEV_GPIO_Mode(EPD_S1_BUSY_PIN, 0);
        DEV_GPIO_Mode(EPD_M2_BUSY_PIN, 0);
        DEV_GPIO_Mode(EPD_S2_BUSY_PIN, 0);

        DEV_Digital_Write(EPD_SCK_PIN, 0);
        DEV_Digital_Write(EPD_MOSI_PIN, 0);

        DEV_Digital_Write(EPD_M1_CS_PIN, 1);
        DEV_Digital_Write(EPD_S1_CS_PIN, 1);
        DEV_Digital_Write(EPD_M2_CS_PIN, 1);
        DEV_Digital_Write(EPD_S2_CS_PIN, 1);

        DEV_Digital_Write(EPD_M2S2_RST_PIN, 0);
        DEV_Digital_Write(EPD_M1S1_RST_PIN, 0);
        DEV_Digital_Write(EPD_M2S2_DC_PIN, 1);
        DEV_Digital_Write(EPD_M1S1_DC_PIN, 1);
    }

    /******************************************************************************
function:	Module Initialize, the BCM2835 library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
    UBYTE DEV_ModuleInit(void)
    {
#ifdef USE_BCM2835_LIB
        if (!bcm2835_init())
        {
            printf("bcm2835 init failed  !!! \r\n");
            return 1;
        }
        else
        {
            printf("bcm2835 init success !!! \r\n");
        }
#elif USE_WIRINGPI_LIB
        //if(wiringPiSetup() < 0)//use wiringpi Pin number table
        if (wiringPiSetupGpio() < 0)
        { //use BCM2835 Pin number table
            printf("set wiringPi lib failed	!!! \r\n");
            return 1;
        }
        else
        {
            printf("set wiringPi lib success !!! \r\n");
        }
#elif USE_DEV_LIB
        printf("Write and read GPIO  \r\n");
#endif
        //software spi configure
        software_spi.SCLK_PIN = EPD_SCK_PIN;
        software_spi.MOSI_PIN = EPD_MOSI_PIN;
        software_spi.Mode = Mode0;
        software_spi.Type = Master;
        software_spi.Clock = 10;

        DEV_GPIOConfig();
        return 0;
    }

    /******************************************************************************
function:	Microsecond delay
parameter:
Info:
******************************************************************************/
    void DEV_Delay_us(UWORD xus)
    {
        UWORD i;
        while (xus)
        {
            for (i = 0; i < software_spi.Clock; i++)
                ;
            xus--;
        }
    }

    /**
 * delay x ms
**/
    void DEV_Delay_ms(UDOUBLE xms)
    {
#ifdef USE_BCM2835_LIB
        bcm2835_delay(xms);
#elif USE_WIRINGPI_LIB
        delay(xms);
#elif USE_DEV_LIB
        UDOUBLE i;
        for (i = 0; i < xms; i++)
        {
            usleep(1000);
        }
#endif
    }

    /******************************************************************************
function:	SPI Mode 0
parameter:
Info:
******************************************************************************/
    void DEV_SPI_WriteByte(UBYTE value)
    {
        char i;
        DEV_Delay_us(6);
        switch (software_spi.Mode)
        {
        case Mode0: /* Clock Polarity is 0 and Clock Phase is 0 */
            DEV_Digital_Write(software_spi.SCLK_PIN, 0);
            for (i = 0; i < 8; i++)
            {
                DEV_Digital_Write(software_spi.SCLK_PIN, 0);
                DEV_Delay_us(5);
                if (value & 0x80)
                {
                    DEV_Digital_Write(software_spi.MOSI_PIN, 1);
                }
                else
                {
                    DEV_Digital_Write(software_spi.MOSI_PIN, 0);
                }
                value = value << 1;
                DEV_Delay_us(5);
                DEV_Digital_Write(software_spi.SCLK_PIN, 1);
                DEV_Delay_us(5);
            }
            break;
        case Mode1: /* Clock Polarity is 0 and Clock Phase is 1 */
            DEV_Digital_Write(software_spi.SCLK_PIN, 0);
            for (i = 0; i < 8; i++)
            {
                DEV_Digital_Write(software_spi.SCLK_PIN, 1);
                if ((value << i) & 0x80)
                {
                    DEV_Digital_Write(software_spi.MOSI_PIN, 1);
                }
                else
                {
                    DEV_Digital_Write(software_spi.MOSI_PIN, 0);
                }
                DEV_Delay_us(5);
                DEV_Digital_Write(software_spi.SCLK_PIN, 0);
                DEV_Delay_us(5);
            }
            DEV_Digital_Write(software_spi.SCLK_PIN, 0);
            break;
        case Mode2: /* Clock Polarity is 1 and Clock Phase is 0 */
            DEV_Digital_Write(software_spi.SCLK_PIN, 1);
            for (i = 0; i < 8; i++)
            {
                DEV_Digital_Write(software_spi.SCLK_PIN, 1);
                if ((value << i) & 0x80)
                {
                    DEV_Digital_Write(software_spi.MOSI_PIN, 1);
                }
                else
                {
                    DEV_Digital_Write(software_spi.MOSI_PIN, 0);
                }
                DEV_Delay_us(5);
                DEV_Digital_Write(software_spi.SCLK_PIN, 0);
                DEV_Delay_us(5);
            }
            DEV_Digital_Write(software_spi.SCLK_PIN, 1);
            break;
        case Mode3: /* Clock Polarity is 1 and Clock Phase is 1 */
            DEV_Digital_Write(software_spi.SCLK_PIN, 1);
            for (i = 0; i < 8; i++)
            {
                DEV_Digital_Write(software_spi.SCLK_PIN, 0);
                if ((value << i) & 0x80)
                {
                    DEV_Digital_Write(software_spi.MOSI_PIN, 1);
                }
                else
                {
                    DEV_Digital_Write(software_spi.MOSI_PIN, 0);
                }
                DEV_Delay_us(5);
                DEV_Digital_Write(software_spi.SCLK_PIN, 1);
                DEV_Delay_us(5);
            }
            DEV_Digital_Write(software_spi.SCLK_PIN, 1);
            break;
        default:
            break;
        }
    }

    UBYTE DEV_SPI_ReadByte(UBYTE Reg)
    {
        unsigned char i, j;
        //set mosi pin intput
        DEV_GPIO_Mode(software_spi.MOSI_PIN, 0);
        j = 0;
        DEV_Delay_us(5);
        for (i = 0; i < 8; i++)
        {
            DEV_Digital_Write(software_spi.SCLK_PIN, 0);
            DEV_Delay_us(5);
            j = j << 1;
            if (DEV_Digital_Read(software_spi.MOSI_PIN) == 1)
                j |= 0x01;
            else
                j &= 0xfe;
            DEV_Delay_us(5);
            DEV_Digital_Write(software_spi.SCLK_PIN, 1);
            DEV_Delay_us(5);
        }

        //set mosi pin output
        DEV_GPIO_Mode(software_spi.MOSI_PIN, 1);
        return j;
    }

    /******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
    void DEV_ModuleExit(void)
    {
        DEV_Digital_Write(EPD_M1S1_RST_PIN, 0);
        DEV_Digital_Write(EPD_M2S2_RST_PIN, 0);
        DEV_Digital_Write(EPD_M1S1_RST_PIN, 0);
        DEV_Digital_Write(EPD_M2S2_DC_PIN, 0);
        DEV_Digital_Write(EPD_M1S1_DC_PIN, 0);
        DEV_Digital_Write(EPD_S1_CS_PIN, 1);
        DEV_Digital_Write(EPD_S2_CS_PIN, 1);
        DEV_Digital_Write(EPD_M1_CS_PIN, 1);
        DEV_Digital_Write(EPD_M2_CS_PIN, 1);
#ifdef USE_BCM2835_LIB
        bcm2835_close();
#elif USE_WIRINGPI_LIB

#elif USE_DEV_LIB

#endif
    }

    int SYSFS_GPIO_Export(int Pin)
    {
        char buffer[NUM_MAXBUF];
        int len;
        int fd;

        fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd < 0)
        {
            SYSFS_GPIO_Debug("Export Failed: Pin%d\n", Pin);
            return -1;
        }

        len = snprintf(buffer, NUM_MAXBUF, "%d", Pin);
        write(fd, buffer, len);

        SYSFS_GPIO_Debug("Export: Pin%d\r\n", Pin);

        close(fd);
        return 0;
    }

    int SYSFS_GPIO_Unexport(int Pin)
    {
        char buffer[NUM_MAXBUF];
        int len;
        int fd;

        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd < 0)
        {
            SYSFS_GPIO_Debug("unexport Failed: Pin%d\n", Pin);
            return -1;
        }

        len = snprintf(buffer, NUM_MAXBUF, "%d", Pin);
        write(fd, buffer, len);

        SYSFS_GPIO_Debug("Unexport: Pin%d\r\n", Pin);

        close(fd);
        return 0;
    }

    int SYSFS_GPIO_Direction(int Pin, int Dir)
    {
        const char dir_str[] = "in\0out";
        char path[DIR_MAXSIZ];
        int fd;

        snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/direction", Pin);
        fd = open(path, O_WRONLY);
        if (fd < 0)
        {
            SYSFS_GPIO_Debug("Set Direction failed: Pin%d\n", Pin);
            return -1;
        }

        if (write(fd, &dir_str[Dir == SYSFS_GPIO_IN ? 0 : 3], Dir == SYSFS_GPIO_IN ? 2 : 3) < 0)
        {
            SYSFS_GPIO_Debug("failed to set direction!\r\n");
            return -1;
        }

        if (Dir == SYSFS_GPIO_IN)
        {
            SYSFS_GPIO_Debug("Pin%d:intput\r\n", Pin);
        }
        else
        {
            SYSFS_GPIO_Debug("Pin%d:Output\r\n", Pin);
        }

        close(fd);
        return 0;
    }

    int SYSFS_GPIO_Read(int Pin)
    {
        char path[DIR_MAXSIZ];
        char value_str[3];
        int fd;

        snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/value", Pin);
        fd = open(path, O_RDONLY);
        if (fd < 0)
        {
            SYSFS_GPIO_Debug("Read failed Pin%d\n", Pin);
            return -1;
        }

        if (read(fd, value_str, 3) < 0)
        {
            SYSFS_GPIO_Debug("failed to read value!\n");
            return -1;
        }

        close(fd);
        return (atoi(value_str));
    }

    int SYSFS_GPIO_Write(int Pin, int value)
    {
        const char s_values_str[] = "01";
        char path[DIR_MAXSIZ];
        int fd;

        snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/value", Pin);
        fd = open(path, O_WRONLY);
        if (fd < 0)
        {
            SYSFS_GPIO_Debug("Write failed : Pin%d,value = %d\n", Pin, value);
            return -1;
        }

        if (write(fd, &s_values_str[value == SYSFS_GPIO_LOW ? 0 : 1], 1) < 0)
        {
            SYSFS_GPIO_Debug("failed to write value!\n");
            return -1;
        }

        close(fd);
        return 0;
    }

private:
    SOFTWARE_SPI software_spi;
};