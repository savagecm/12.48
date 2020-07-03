#pragma once

#include "fonts/fonts.hpp"
#include "util.hpp"

class guiPaint
{

public:
    guiPaint()
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

    ~guiPaint()
    {
        free(this->BlackImage);
        free(this->RedImage);
    }

    static guiPaint *getInstance()
    {
        static guiPaint *ret = new guiPaint();
        return ret;
    }

 
    /******************************************************************************
function:	Select Image Rotate
parameter:
    Rotate   :   0,90,180,270
******************************************************************************/
    void Paint_SetRotate(UWORD Rotate)
    {
        if (Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270)
        {
            Debug("Set image Rotate %d\r\n", Rotate);
            Paint.Rotate = Rotate;
        }
        else
        {
            Debug("rotate = 0, 90, 180, 270\r\n");
            exit(0);
        }
    }

    /******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :       Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
    void Paint_SetMirroring(UBYTE mirror)
    {
        if (mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL ||
            mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN)
        {
            Debug("mirror image x:%s, y:%s\r\n", (mirror & 0x01) ? "mirror" : "none", ((mirror >> 1) & 0x01) ? "mirror" : "none");
            Paint.Mirror = mirror;
        }
        else
        {
            Debug("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL, \
        MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
            exit(0);
        }
    }

    /******************************************************************************
function:	Draw Pixels
parameter:
    Xpoint  :   At point X
    Ypoint  :   At point Y
    Color   :   Painted colors
******************************************************************************/
    void Paint_SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color)
    {
        if (Xpoint > Paint.Width || Ypoint > Paint.Height)
        {
            Debug("Exceeding display boundaries\r\n");
            return;
        }
        UWORD X, Y;

        switch (Paint.Rotate)
        {
            // #if (MIRROR_IMAGE_DFT && MIRROR_NONE)
        case 0:
            X = Xpoint;
            Y = Ypoint;
            break;
        case 90:
            X = Paint.WidthMemory - Ypoint - 1;
            Y = Xpoint;
            break;
        case 180:
            X = Paint.WidthMemory - Xpoint - 1;
            Y = Paint.HeightMemory - Ypoint - 1;
            break;
        case 270:
            X = Ypoint;
            Y = Paint.HeightMemory - Xpoint - 1;
            break;
            // #elif (MIRROR_IMAGE_DFT && MIRROR_HORIZONTAL)
            // case 0:
            // X = Paint.WidthMemory - Xpoint - 1;
            // Y = Ypoint;
            // break;
            // case 90:
            // X = Ypoint;
            // Y = Xpoint;
            // break;
            // case 180:
            // X = Xpoint;
            // Y = Paint.HeightMemory - Ypoint - 1;
            // break;
            // case 270:
            // X = Paint.WidthMemory - Ypoint - 1;
            // Y = Paint.HeightMemory - Xpoint - 1;
            // break;
            // #elif (MIRROR_IMAGE_DFT && MIRROR_VERTICAL)
            // case 0:
            // X = Xpoint;
            // Y = Paint.HeightMemory - Ypoint;
            // break;
            // case 90:
            // X = Paint.WidthMemory - Ypoint - 1;
            // Y = Paint.HeightMemory - Xpoint;
            // break;
            // case 180:
            // X = Paint.WidthMemory - Xpoint - 1;
            // Y = Ypoint;
            // break;
            // case 270:
            // X = Ypoint;
            // Y = Xpoint;
            // break;
            // #endif
        default:
            return;
        }

        switch (Paint.Mirror)
        {
        case MIRROR_NONE:
            break;
        case MIRROR_HORIZONTAL:
            X = Paint.WidthMemory - X - 1;
            break;
        case MIRROR_VERTICAL:
            Y = Paint.HeightMemory - Y - 1;
            break;
        case MIRROR_ORIGIN:
            X = Paint.WidthMemory - X - 1;
            Y = Paint.HeightMemory - Y - 1;
            break;
        default:
            return;
        }

        // printf("x = %d, y = %d\r\n", X, Y);
        if (X > Paint.WidthMemory || Y > Paint.HeightMemory)
        {
            Debug("Exceeding display boundaries\r\n");
            return;
        }

        UDOUBLE Addr = X / 8 + Y * Paint.WidthByte;
        UBYTE Rdata = Paint.Image[Addr];
        if (Color == BLACK)
        {
            // for black need to set the bit to 0
            Paint.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
        }
        else if (Color == RED)
        {
            // for red need to set the bit to 1
            Paint.RImage[Addr] = Rdata | (0x80 >> (X % 8));
        }
        else
        {
            // here there is some tricky
            // need to set BW/RW both
            Paint.RImage[Addr] = Rdata & ~(0x80 >> (X % 8));
            Paint.Image[Addr] = Rdata | (0x80 >> (X % 8));
        }
    }

    /******************************************************************************
function:	Clear the color of the picture
parameter:
    Color   :   Painted colors
******************************************************************************/
    void Paint_Clear(UWORD Color = WHITE)
    {
        // Debug("x = %d, y = %d\r\n", Paint.WidthByte, Paint.Height);
        for (UWORD Y = 0; Y < Paint.HeightByte; Y++)
        {
            for (UWORD X = 0; X < Paint.WidthByte; X++)
            { //8 pixel =  1 byte
                UDOUBLE Addr = X + Y * Paint.WidthByte;
                if (Color == BLACK || Color == WHITE)
                {
                    Paint.Image[Addr] = Color;
                }
                else if (Color == RED)
                {
                    Paint.RImage[Addr] = 0xFF;
                }
                else
                {
                    // unsupport colour
                }
            }
        }
    }

    /******************************************************************************
function:	Clear the color of a window
parameter:
    Xstart :   x starting point
    Ystart :   Y starting point
    Xend   :   x end point
    Yend   :   y end point
******************************************************************************/
    void Paint_ClearWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color)
    {
        UWORD X, Y;
        for (Y = Ystart; Y < Yend; Y++)
        {
            for (X = Xstart; X < Xend; X++)
            { //8 pixel =  1 byte
                Paint_SetPixel(X, Y, Color);
            }
        }
    }

    /******************************************************************************
function:	Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		:   The Xpoint coordinate of the point
    Ypoint		:   The Ypoint coordinate of the point
    Color		:   Set color
    Dot_Pixel	:	point size
******************************************************************************/
    void Paint_DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color,
                         DOT_PIXEL Dot_Pixel, DOT_STYLE DOT_STYLE)
    {
        if (Xpoint > Paint.Width || Ypoint > Paint.Height)
        {
            Debug("Paint_DrawPoint Input exceeds the normal display range\r\n");
            return;
        }

        int16_t XDir_Num, YDir_Num;
        if (DOT_STYLE == DOT_FILL_AROUND)
        {
            for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++)
            {
                for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++)
                {
                    if (Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                        break;
                    // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                    Paint_SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
                }
            }
        }
        else
        {
            for (XDir_Num = 0; XDir_Num < Dot_Pixel; XDir_Num++)
            {
                for (YDir_Num = 0; YDir_Num < Dot_Pixel; YDir_Num++)
                {
                    Paint_SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
                }
            }
        }
    }

    /******************************************************************************
function:	Draw a line of arbitrary slope
parameter:
    Xstart ：Starting Xpoint point coordinates
    Ystart ：Starting Xpoint point coordinates
    Xend   ：End point Xpoint coordinate
    Yend   ：End point Ypoint coordinate
    Color  ：The color of the line segment
******************************************************************************/
    void Paint_DrawLine(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                        UWORD Color, LINE_STYLE Line_Style, DOT_PIXEL Dot_Pixel)
    {
        if (Xstart > Paint.Width || Ystart > Paint.Height ||
            Xend > Paint.Width || Yend > Paint.Height)
        {
            Debug("Paint_DrawLine Input exceeds the normal display range\r\n");
            return;
        }

        UWORD Xpoint = Xstart;
        UWORD Ypoint = Ystart;
        int dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
        int dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

        // Increment direction, 1 is positive, -1 is counter;
        int XAddway = Xstart < Xend ? 1 : -1;
        int YAddway = Ystart < Yend ? 1 : -1;

        //Cumulative error
        int Esp = dx + dy;
        char Dotted_Len = 0;

        for (;;)
        {
            Dotted_Len++;
            //Painted dotted line, 2 point is really virtual
            if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0)
            {
                //Debug("LINE_DOTTED\r\n");
                Paint_DrawPoint(Xpoint, Ypoint, IMAGE_BACKGROUND, Dot_Pixel, DOT_STYLE_DFT);
                Dotted_Len = 0;
            }
            else
            {
                Paint_DrawPoint(Xpoint, Ypoint, Color, Dot_Pixel, DOT_STYLE_DFT);
            }
            if (2 * Esp >= dy)
            {
                if (Xpoint == Xend)
                    break;
                Esp += dy;
                Xpoint += XAddway;
            }
            if (2 * Esp <= dx)
            {
                if (Ypoint == Yend)
                    break;
                Esp += dx;
                Ypoint += YAddway;
            }
        }
    }

    /******************************************************************************
function:	Draw a rectangle
parameter:
    Xstart ：Rectangular  Starting Xpoint point coordinates
    Ystart ：Rectangular  Starting Xpoint point coordinates
    Xend   ：Rectangular  End point Xpoint coordinate
    Yend   ：Rectangular  End point Ypoint coordinate
    Color  ：The color of the Rectangular segment
    Filled : Whether it is filled--- 1 solid 0：empty
******************************************************************************/
    void Paint_DrawRectangle(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                             UWORD Color, DRAW_FILL Filled, DOT_PIXEL Dot_Pixel)
    {
        if (Xstart > Paint.Width || Ystart > Paint.Height ||
            Xend > Paint.Width || Yend > Paint.Height)
        {
            Debug("Input exceeds the normal display range\r\n");
            return;
        }

        if (Filled)
        {
            UWORD Ypoint;
            for (Ypoint = Ystart; Ypoint < Yend; Ypoint++)
            {
                Paint_DrawLine(Xstart, Ypoint, Xend, Ypoint, Color, LINE_STYLE_SOLID, Dot_Pixel);
            }
        }
        else
        {
            Paint_DrawLine(Xstart, Ystart, Xend, Ystart, Color, LINE_STYLE_SOLID, Dot_Pixel);
            Paint_DrawLine(Xstart, Ystart, Xstart, Yend, Color, LINE_STYLE_SOLID, Dot_Pixel);
            Paint_DrawLine(Xend, Yend, Xend, Ystart, Color, LINE_STYLE_SOLID, Dot_Pixel);
            Paint_DrawLine(Xend, Yend, Xstart, Yend, Color, LINE_STYLE_SOLID, Dot_Pixel);
        }
    }

    /******************************************************************************
function:	Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  ：Center X coordinate
    Y_Center  ：Center Y coordinate
    Radius    ：circle Radius
    Color     ：The color of the ：circle segment
    Filled    : Whether it is filled: 1 filling 0：Do not
******************************************************************************/
    void Paint_DrawCircle(UWORD X_Center, UWORD Y_Center, UWORD Radius,
                          UWORD Color, DRAW_FILL Draw_Fill, DOT_PIXEL Dot_Pixel)
    {
        if (X_Center > Paint.Width || Y_Center >= Paint.Height)
        {
            Debug("Paint_DrawCircle Input exceeds the normal display range\r\n");
            return;
        }

        //Draw a circle from(0, R) as a starting point
        int16_t XCurrent, YCurrent;
        XCurrent = 0;
        YCurrent = Radius;

        //Cumulative error,judge the next point of the logo
        int16_t Esp = 3 - (Radius << 1);

        int16_t sCountY;
        if (Draw_Fill == DRAW_FILL_FULL)
        {
            while (XCurrent <= YCurrent)
            { //Realistic circles
                for (sCountY = XCurrent; sCountY <= YCurrent; sCountY++)
                {
                    Paint_DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); //1
                    Paint_DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); //2
                    Paint_DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); //3
                    Paint_DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); //4
                    Paint_DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); //5
                    Paint_DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); //6
                    Paint_DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT); //7
                    Paint_DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
                if (Esp < 0)
                    Esp += 4 * XCurrent + 6;
                else
                {
                    Esp += 10 + 4 * (XCurrent - YCurrent);
                    YCurrent--;
                }
                XCurrent++;
            }
        }
        else
        { //Draw a hollow circle
            while (XCurrent <= YCurrent)
            {
                Paint_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Dot_Pixel, DOT_STYLE_DFT); //1
                Paint_DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Dot_Pixel, DOT_STYLE_DFT); //2
                Paint_DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Dot_Pixel, DOT_STYLE_DFT); //3
                Paint_DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Dot_Pixel, DOT_STYLE_DFT); //4
                Paint_DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Dot_Pixel, DOT_STYLE_DFT); //5
                Paint_DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Dot_Pixel, DOT_STYLE_DFT); //6
                Paint_DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Dot_Pixel, DOT_STYLE_DFT); //7
                Paint_DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Dot_Pixel, DOT_STYLE_DFT); //0

                if (Esp < 0)
                    Esp += 4 * XCurrent + 6;
                else
                {
                    Esp += 10 + 4 * (XCurrent - YCurrent);
                    YCurrent--;
                }
                XCurrent++;
            }
        }
    }

private:
private:
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

    UBYTE *BlackImage;
    UBYTE *RedImage;

    PAINT Paint;
};