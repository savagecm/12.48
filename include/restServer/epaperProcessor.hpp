#pragma once

#include "ePaper/epd12in48.hpp"
#include "ePaper/guiPaint.hpp"
#include "cpprest/json.h"
#include <string>
enum class epaperRet
{
    SUCCESS = 0,
    SUCCESS_CREATED,
    NOT_SUPPORT,
    BAD_REQUEST

};

using namespace std;
class epaperProcessor
{
public:
    //void Paint_DrawLine(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, LINE_STYLE Line_Style, DOT_PIXEL Dot_Pixel)
    static epaperRet processLine(web::json::value jValue)
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in the line case");
        }
        //    void Paint_DrawLine(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, LINE_STYLE Line_Style, DOT_PIXEL Dot_Pixel)
        //{"color":"red","positionx":[0,0],"positiony":[100,100],"lineStyle":"solid","dotPixel":"1*1"}
        auto posx = getPosition(jValue, "positionx");
        auto posy = getPosition(jValue, "positiony");

        guiPaint::getInstance()->Paint_DrawLine(posx.first, posx.second, posy.first, posy.second, getColor(jValue), getLineStyle(jValue), getDotPixel(jValue));
        return epaperRet::SUCCESS;
    }
    //  void printString(std::string inStr, int font, int posx, int posy, int colour, int bcolour, int maxWidth = EPD_12in48B_MAX_WIDTH, int maxHeight = EPD_12in48B_MAX_HEIGHT)
    static epaperRet processString(web::json::value jValue)
    {
        //{\"bcolor\":\"white\",\"fcolor\":\"red\",\"position\":[0,0],\"font\":40,\"data\":\"string detail\"}
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in the string case");
        }
        auto pos = getPosition(jValue);
        guiPaint::getInstance()->printString(getStringField(jValue, "data"), getIntField(jValue, "font"), pos.first, pos.second, getColor(jValue, "fcolor"), getColor(jValue, "bcolor"));
        return epaperRet::SUCCESS;
    }

    static epaperRet processCircle(web::json::value jValue)
    {
        //  void Paint_DrawCircle(UWORD X_Center, UWORD Y_Center, UWORD Radius, UWORD Color, DRAW_FILL Draw_Fill, DOT_PIXEL Dot_Pixel)
        //{\"color\":\"red\",\"position\":[0,0],\"radius\":40,\"fill\":\"full\",\"dotPixel\":\"1*1\"}
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in the circle case");
        }
        auto pos = getPosition(jValue);
        guiPaint::getInstance()->Paint_DrawCircle(pos.first, pos.second, getIntField(jValue, "radius"), getColor(jValue), getDrawFill(jValue), getDotPixel(jValue));
        return epaperRet::SUCCESS;
    }

    static epaperRet processRectangle(web::json::value jValue)
    {
        //    void Paint_DrawRectangle(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, DRAW_FILL Filled, DOT_PIXEL Dot_Pixel)
        // {"color":"red","positionx":[0,0],"positiony":[100,100],\"fill\":\"full\",\"dotPixel\":\"1*1\"}
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in the rectangle case");
        }
        auto posx = getPosition(jValue, "positionx");
        auto posy = getPosition(jValue, "positiony");

        guiPaint::getInstance()->Paint_DrawRectangle(posx.first, posx.second, posy.first, posy.second, getColor(jValue), getDrawFill(jValue), getDotPixel(jValue));
        return epaperRet::SUCCESS;
    }
    static epaperRet processPoint(web::json::value jValue)
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in the point case");
        }
        //  void Paint_DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color, DOT_PIXEL Dot_Pixel, DOT_STYLE DOT_STYLE)
        // {"color":"red","position":[0,0],"dotStyle":"fill_around","dotPixel":"1*1"}
        auto pos = getPosition(jValue);
        guiPaint::getInstance()->Paint_DrawPoint(pos.first, pos.second, getColor(jValue), getDotPixel(jValue), getDotStyle(jValue));
        return epaperRet::SUCCESS;
    }
    static epaperRet processImage(web::json::value jValue)
    {
        // {"location":"dir/xxx.bmp","position":[0,0]}
        //UBYTE GUI_ReadBmp(const char *path, UWORD Xstart, UWORD Ystart)
        std::string path;
        if (jValue.has_field("location"))
        {
            path = jValue.at("location").as_string();
        }
        else
        {
            return epaperRet::BAD_REQUEST;
        }
        auto pos = getPosition(jValue);
        guiPaint::getInstance()->GUI_ReadBmp(path.c_str(), pos.first, pos.second);
        return epaperRet::SUCCESS;
    }

    /*
    [{
    "point": {
        "color": "red",
        "position": [0, 0],
        "dotStyle": "fill_around",
        "dotPixel": "1*1"
    }
    }, {
        "point": {
            "color": "red",
            "position": [0, 0],
            "dotStyle": "fill_around",
            "dotPixel": "1*1"
        }
    }]
    */

    static epaperRet processGroup(web::json::value jValue)
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in the group case");
        }

        if (jValue.is_array())
        {
            auto oArray = jValue.as_array();

            auto arraySize = oArray.size();
            for (unsigned int i = 0; i < arraySize; arraySize++)
            {
                processItem(oArray.at(i));
            }
        }
        else
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "group should be array!");
            }
        }
        return epaperRet::SUCCESS;
    }
    // point, line,string,rectangle,circle,
    static void processItem(web::json::value jValue)
    {

        if (jValue.has_field("point"))
        {
            processPoint(jValue.at("point"));
        }
        else if (jValue.has_field("line"))
        {
            processLine(jValue.at("line"));
        }
        else if (jValue.has_field("string"))
        {
            processString(jValue.at("string"));
        }
        else if (jValue.has_field("rectangle"))
        {
            processRectangle(jValue.at("rectangle"));
        }
        else if (jValue.has_field("circle"))
        {
            processCircle(jValue.at("circle"));
        }
        else if (jValue.has_field("rotate"))
        {
            processRotate(jValue.at("rotate"));
        }
    }

    static epaperRet processRotate(web::json::value jValue)
    {
        // {"rotate":90}
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in the rotate case");
        }
        if (jValue.has_field("rotate"))
        {
            int rotate = jValue.at("rotate").as_integer();
            guiPaint::getInstance()->Paint_SetRotate(rotate);
        }
        return epaperRet::SUCCESS;
    }
    // optional
    static UWORD getColor(web::json::value jValue, string colorStr = "color")
    {
        UWORD Color = WHITE;
        if (jValue.has_field(colorStr))
        {
            string color = jValue.at(colorStr).as_string();
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "parse color string is : " + color);
            }
            if (!color.compare("red"))
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "red color");
                }
                Color = RED;
            }
            else if (!color.compare("white"))
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "white color");
                }
                Color = WHITE;
            }
            else if (!color.compare("black"))
            {

                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "black color");
                }
                Color = BLACK;
            }
            else
            {
                if (CHECK_LOG_LEVEL(error))
                {
                    __LOG(error, "unsupported color : " + color);
                }
            }
        }
        return Color;
    }

    static LINE_STYLE getLineStyle(web::json::value jValue)
    {
        LINE_STYLE style = LINE_STYLE_SOLID;
        if (jValue.has_field("lineStyle"))
        {
            string lineStyle = jValue.at("lineStyle").as_string();
            if (!lineStyle.compare("solid"))
            {
                style = LINE_STYLE_SOLID;
            }
            else if (!lineStyle.compare("dotted"))
            {
                style = LINE_STYLE_DOTTED;
            }
            else
            {
                if (CHECK_LOG_LEVEL(error))
                {
                    __LOG(error, "unsupport line style" + lineStyle);
                }
            }
        }
        return style;
    }

    /*
enum DOT_STYLE
{
    DOT_FILL_AROUND = 1, // dot pixel 1 x 1
    DOT_FILL_RIGHTUP,    // dot pixel 2 X 2
};*/

    static DOT_STYLE getDotStyle(web::json::value jValue)
    {
        DOT_STYLE style = DOT_FILL_AROUND;
        if (jValue.has_field("dotStyle"))
        {
            string dotStyle = jValue.at("dotStyle").as_string();
            if (!dotStyle.compare("fill_around"))
            {
                style = DOT_FILL_AROUND;
            }
            else if (!dotStyle.compare("fill_rightup"))
            {
                style = DOT_FILL_RIGHTUP;
            }
            else
            {
                if (CHECK_LOG_LEVEL(error))
                {
                    __LOG(error, "unsupport dote style" + dotStyle);
                }
            }
        }
        return style;
    }

    static DOT_PIXEL getDotPixel(web::json::value jValue)
    {
        DOT_PIXEL dot = DOT_PIXEL_1X1;
        if (jValue.has_field("dotPixel"))
        {
            string dotStr = jValue.at("dotPixel").as_string();

            /*  DOT_PIXEL_1X1 = 1, // 1 x 1
                DOT_PIXEL_2X2,     // 2 X 2
                DOT_PIXEL_3X3,     // 3 X 3
                DOT_PIXEL_4X4,     // 4 X 4
                DOT_PIXEL_5X5,     // 5 X 5
                DOT_PIXEL_6X6,     // 6 X 6
                DOT_PIXEL_7X7,     // 7 X 7
                DOT_PIXEL_8X8,
            */

            if (!dotStr.compare("1*1"))
            {
                dot = DOT_PIXEL_1X1;
            }
            else if (!dotStr.compare("2*2"))
            {
                dot = DOT_PIXEL_2X2;
            }
            else if (!dotStr.compare("3*3"))
            {
                dot = DOT_PIXEL_3X3;
            }
            else if (!dotStr.compare("4*4"))
            {
                dot = DOT_PIXEL_4X4;
            }
            else if (!dotStr.compare("5*5"))
            {
                dot = DOT_PIXEL_5X5;
            }
            else if (!dotStr.compare("6*6"))
            {
                dot = DOT_PIXEL_6X6;
            }
            else if (!dotStr.compare("7*7"))
            {
                dot = DOT_PIXEL_7X7;
            }
            else if (!dotStr.compare("8*8"))
            {
                dot = DOT_PIXEL_8X8;
            }
            else
            {
                if (CHECK_LOG_LEVEL(error))
                {
                    __LOG(error, "unsupport dot style" + dotStr);
                }
            }
        }
        return dot;
    }

    static DRAW_FILL getDrawFill(web::json::value jValue)
    {
        DRAW_FILL fill = DRAW_FILL_EMPTY;
        if (jValue.has_field("fill"))
        {
            string fillType = jValue.at("fill").as_string();
            if (!fillType.compare("empty"))
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "fill empty");
                }
                fill = DRAW_FILL_EMPTY;
            }
            else if (!fillType.compare("full"))
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "fill full");
                }
                fill = DRAW_FILL_FULL;
            }
            else
            {
                if (CHECK_LOG_LEVEL(error))
                {
                    __LOG(error, "unsupport fill type" + fillType);
                }
            }
        }
        return fill;
    }

    // mandatory
    static int getIntField(web::json::value jValue, std::string key)
    {
        int ret;
        if (jValue.has_field(key))
        {
            ret = jValue.at(key).as_integer();
        }
        else
        {
            // to do
            // throw
        }
        return ret;
    }
    static std::string getStringField(web::json::value jValue, std::string key)
    {
        std::string ret;
        if (jValue.has_field(key))
        {
            ret = jValue.at(key).as_string();
        }
        else
        {
            // to do
            // throw
        }
        return ret;
    }

    static std::pair<int, int> getPosition(web::json::value jValue, std::string key = "position")
    {
        int x, y;
        if (jValue.has_field(key))
        {
            json::array pos = jValue.at(key).as_array();
            if (pos.size() != 2)
            {
                // to do
                // throw
            }
            else
            {
                x = pos.at(0).as_integer();
                y = pos.at(1).as_integer();
            }
        }
        return std::make_pair(x, y);
    }
};