#pragma once

// Color definition
#define EPDPAINT_BLACK 0x0
#define EPDPAINT_RED 0x4
#define EPDPAINT_WHITE 0x3

#include "fonts/fonts.hpp"
#include "GUI/GUI_Paint.h"
#include "logger/simple_logger.hpp"
#include "epd1248b.hpp"

#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

class epd1248
{
public:
    epd1248()
    {
        UDOUBLE Imagesize = (((EPD_12in48B_MAX_WIDTH % 8 == 0) ? (EPD_12in48B_MAX_WIDTH / 8) : (EPD_12in48B_MAX_WIDTH / 8 + 1)) * EPD_12in48B_MAX_HEIGHT);
        if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL)
        {
            printf("Failed to apply for black memory...\r\n");
            exit(0);
        }
        if ((RedImage = (UBYTE *)malloc(Imagesize)) == NULL)
        {
            printf("Failed to apply for red memory...\r\n");
            exit(0);
        }
    }
    ~epd1248()
    {
        EPD_12in48B_Sleep();
        DEV_ModuleExit();
        free(this->BlackImage);
        free(this->RedImage);
    }

    int init()
    {
        printf("12.48inch e-Paper B demo\r\n");
        DEV_ModuleInit();

        if (EPD_12in48B_Init() != 0)
        {
            printf("e-Paper init failed\r\n");
        }
        else
        {
            printf("e-Paper init...\r\n");
        }
        EPD_12in48B_Clear();

        std::unique_ptr<simpleLogger> simpleLoggerUptr(new simpleLogger());
        INIT_LOGGER(simpleLoggerUptr);
        SET_LOG_LEVEL(debug);
        return 0;
    }

    static epd1248 *getInstance()
    {
        static epd1248 *ins = new epd1248();
        return ins;
    }

    void DrawCharAt(int x, int y, char ascii_char, const sFONT *font, int color, int bcolour = EPDPAINT_WHITE)
    {
        int i, j;
        unsigned int char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
        const unsigned char *ptr = &font->table[char_offset];

        for (j = 0; j < font->Height; j++)
        {
            for (i = 0; i < font->Width; i++)
            {
                if (*ptr & (0x80 >> (i % 8)))
                {
                    Paint_SetPixel(x + i, y + j, color);
                }
                if (i % 8 == 7)
                {
                    ptr++;
                }
            }
            if (font->Width % 8 != 0)
            {
                ptr++;
            }
        }
    }

    void DrawStringAt(int x, int y, const char *text, const sFONT *font, int color)
    {
        const char *p_text = text;
        unsigned int counter = 0;
        int refcolumn = x;

        /* Send the string character by character on EPD */
        while (*p_text != 0)
        {
            /* Display one character on EPD */
            DrawCharAt(refcolumn, y, *p_text, font, color);
            /* Decrement the column position by 16 */
            refcolumn += font->Width;
            /* Point on the next character */
            p_text++;
            counter++;
        }
    }

    int do_convert(iconv_t cd, const char *from, size_t from_size, std::string *to)
    {
        char *in_buf_ptr = const_cast<char *>(from);
        size_t in_bytes_left = from_size;
        size_t out_bytes = in_bytes_left * 3 + 1;
        size_t out_bytes_left = out_bytes;
        std::string out(out_bytes_left, '\0');
        char *out_buf_start = const_cast<char *>(out.c_str());
        char *out_buf_ptr = out_buf_start;

        int bytes = iconv(cd, &in_buf_ptr, &in_bytes_left, &out_buf_ptr, &out_bytes_left);
        if (-1 == bytes)
            return errno;

        to->assign(out_buf_start, out_bytes - out_bytes_left);
        return 0;
    }

    std::string convert(const std::string &from_charset, const std::string &to_charset,
                        const std::string &from,
                        bool ignore_error, bool skip_error)
    {
        std::string result;
        char *in_buf = const_cast<char *>(from.c_str());
        size_t in_bytes = from.size();
        size_t in_bytes_left = in_bytes;
        iconv_t cd = iconv_open(to_charset.c_str(), from_charset.c_str());

        if ((iconv_t)(-1) == cd)
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "iconv_open return error, error number is : " << errno);
            }
            return "";
        }
        while (in_bytes_left > 0)
        {
            int errcode;
            size_t out_bytes = in_bytes_left * 3 + 1;
            size_t out_bytes_left = out_bytes;
            std::string out(out_bytes_left, '\0');
            char *out_buf = const_cast<char *>(out.c_str());
            char *out_buf_start = out_buf;
            char *in_buf_start = in_buf;

            int bytes = iconv(cd, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left);
            if (bytes != -1)
            {
                result.append(out_buf_start, out_bytes - out_bytes_left);
                break;
            }
            else if (!ignore_error)
            {
                errcode = errno;
                iconv_close(cd);
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "iconv return error, error number is : " << errno);
                }
                return result;
            }
            else
            {
                // EILSEQ An invalid multibyte sequence has been encountered in the input.
                // EINVAL An incomplete multibyte sequence has been encountered in the input.
                if ((errno != EINVAL) &&
                    (errno != EILSEQ))
                {
                    // E2BIG  There is not sufficient room at *outbuf.
                    errcode = errno;
                    iconv_close(cd);
                    if (CHECK_LOG_LEVEL(debug))
                    {
                        __LOG(debug, "iconv return error, error number is : " << errno);
                    }
                    return result;
                }
                else
                {

                    if (in_buf != in_buf_start)
                    {
                        std::string str;
                        errcode = do_convert(cd, in_buf_start, in_buf - in_buf_start, &str);
                        if (errcode != 0)
                        {
                            iconv_close(cd);
                            if (CHECK_LOG_LEVEL(debug))
                            {
                                __LOG(debug, "do_convert return error, error number is : " << errno);
                            }
                            return result;
                        }

                        result.append(str);
                    }

                    if (!skip_error)
                    {
                        result.append(in_buf, 1);
                    }

                    --in_bytes_left;
                    ++in_buf;
                }
            }
        }

        if (-1 == iconv_close(cd))
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "iconv_close return error, error number is : " << errno);
            }
            return result;
        }

        return result;
    }

    std::string utf8_to_gb2312(const std::string &from, bool ignore_error = false, bool skip_error = false)
    {
        return convert("utf-8", "gb2312", from, ignore_error, skip_error);
    }
    // note: the maxWidth default is EPD_12in48B_MAX_WIDTH without rotate or mirror....
    // which means the max width, when hit the max width, will change another line
    // same meaning with maxHeight
    void printString(std::string inStr, int font, int posx, int posy, int colour, int bcolour, int maxWidth = EPD_12in48B_MAX_WIDTH, int maxHeight = EPD_12in48B_MAX_HEIGHT)
    {
        std::string outStr = utf8_to_gb2312(inStr);

        for (unsigned int i = 0; i < outStr.length();)
        {
            if (outStr[i] <= 0x7F)
            {
                printf("receive ASCII code : %c\n", outStr[i]);
                if (font == 8)
                {
                    display_word(&outStr[i], &Font8, false, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 12)
                {
                    display_word(&outStr[i], &Font12, false, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 16)
                {
                    display_word(&outStr[i], &Font16, false, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 20)
                {
                    display_word(&outStr[i], &Font20, false, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 24)
                {
                    display_word(&outStr[i], &Font24, false, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else
                {
                    if (CHECK_LOG_LEVEL(debug))
                    {
                        __LOG(debug, "unsupported font, font is : " << font);
                    }
                    i++;
                    continue;
                }
                i++;
            }
            else
            {
                if (font == 12)
                {
                    display_word(&outStr[i], &HZFont12, true, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 14)
                {
                    display_word(&outStr[i], &HZFont14, true, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 16)
                {
                    display_word(&outStr[i], &HZFont16, true, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 24)
                {
                    display_word(&outStr[i], &HZFont24, true, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 32)
                {
                    display_word(&outStr[i], &HZFont32, true, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 40)
                {
                    display_word(&outStr[i], &HZFont40, true, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else if (font == 48)
                {
                    display_word(&outStr[i], &HZFont48, true, posx, posy, colour, bcolour, maxWidth, maxHeight);
                }
                else
                {
                    if (CHECK_LOG_LEVEL(debug))
                    {
                        __LOG(debug, "unsupported font, font is : " << font);
                    }

                    i += 2;
                    continue;
                }
                i += 2;
            }
        }
    }

    void display_word(char *oneChar, const sFONT *font, bool isCH, int &posx, int &posy, int colour, int bcolour, int maxWidth, int maxHeight)
    {
        if ((posy + font->Height) <= maxHeight)
        {
            if ((posx + font->Width) <= maxWidth)
            {
                if (isCH)
                {
                    Bytes_Display(oneChar, font, posx, posy, colour, bcolour);
                }
                else
                {
                    epd1248::getInstance()->DrawCharAt(posx, posy, *oneChar, font, colour, bcolour);
                }
                posx += font->Width;
            }
            else
            {

                if ((posy + 2 * font->Height) <= maxHeight)
                {
                    posx = 0;
                    posy += font->Height;
                    if (isCH)
                    {
                        Bytes_Display(oneChar, font, posx, posy, colour, bcolour);
                    }
                    else
                    {
                        epd1248::getInstance()->DrawCharAt(posx, posy, *oneChar, font, colour, bcolour);
                    }
                    posx += font->Width;
                }
                else
                {
                    return;
                }
            }
        }
        else
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "out of boundry");
            }
        }
    }

    void Bytes_Read_from_HZK(unsigned char *s, char *const chs, const sFONT *font)
    {
        FILE *fp;
        unsigned long offset;
        std::string fileName = "HZK" + std::to_string(font->Height);
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "open HZK lib with name : " << fileName);
        }
        if (font->Height == 12)
        {
            offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 24;
        }
        else if (font->Height == 14)
        {
            offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 28;
        }
        else if (font->Height == 16)
        {
            offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 32;
        }
        else if (font->Height == 24)
        {
            offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 72;
        }
        else if (font->Height == 32)
        {
            offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 128;
        }
        else if (font->Height == 40)
        {
            offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 200;
        }
        else if (font->Height == 48)
        {
            offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 288;
        }
        else
        {
            // default 16
            offset = ((s[0] - 0xa1) * 94 + (s[1] - 0xa1)) * 32;
        }

        if ((fp = fopen(fileName.c_str(), "r")) == NULL)
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "open " << fileName << " fail");
            }
            return;
        }
        fseek(fp, offset, SEEK_SET);
        fread(chs, ((font->Height) / 8 + 1) * (font->Width), 1, fp);
        fclose(fp);
    }

    void Bytes_Display(char *const s, const sFONT *font, int positionx, int positiony, int fcolour, int bcolour)
    {
        char *chs = (char *)malloc(((font->Width) / 8 + 1) * (font->Height));
        Bytes_Read_from_HZK((unsigned char *)(s), chs, font);

        int charPos = 0;
        for (uint16_t i = 0; i < (font->Height); i++)
        {
            for (uint16_t j = 0; j < (font->Height); j++)
            {
                char tmpChar = chs[charPos + j / 8];
                if (tmpChar & (0x1 << (7 - (j % 8))))
                {
                    Paint_SetPixel(positionx + j, positiony + i, fcolour);
                }
                else
                {
                    Paint_SetPixel(positionx + j, positiony + i, bcolour);
                }
            }
            charPos += (font->Width / 8);
        }
        free(chs);
    }

private:
    UBYTE *BlackImage;
    UBYTE *RedImage;
};
