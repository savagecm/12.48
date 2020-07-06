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
    static epaperRet processLine(web::json::value jValue)
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in the line case");
        }

        //{"color":"red","positionx":[0,0],"positiony":[100,100],"lineStyle":"solid","dotPixel":"1*1"}

        json::array posx = jValue.at("positionx").as_array();
        json::array posy = jValue.at("positiony").as_array();
        if (posx.size() != 2 || posy.size() != 2)
        {
            return epaperRet::BAD_REQUEST;
        }
        int posxx = posx.at(0).as_integer();
        int posxy = posx.at(1).as_integer();
        int posyx = posy.at(0).as_integer();
        int posyy = posy.at(1).as_integer();

        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "the line info is : " << posxx << ":" << posxy << ":" << posyx << ":" << posyy << ", color is : " << getColor(jValue));
        }
        guiPaint::getInstance()->Paint_DrawLine(posxx, posxy, posyx, posyy, getColor(jValue), getLineStyle(jValue), getDotPixel(jValue));
        return epaperRet::SUCCESS;
    }
    //  void printString(std::string inStr, int font, int posx, int posy, int colour, int bcolour, int maxWidth = EPD_12in48B_MAX_WIDTH, int maxHeight = EPD_12in48B_MAX_HEIGHT)
    static epaperRet processString(web::json::value jValue)
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in the line case");
        }
        string data;
        if (jValue.has_field("data"))
        {
            data = jValue.at("data").as_string();
        }
        else
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "no string in the json data");
            }
            return epaperRet::BAD_REQUEST;
        }

        int font;

        if (jValue.has_field("font"))
        {
            font = jValue.at("font").as_integer();
        }
        else
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "no font in the json data");
            }
            return epaperRet::BAD_REQUEST;
        }

        int posx, posy;
        if (jValue.has_field("position"))
        {
            json::array pos = jValue.at("position").as_array();

            if (pos.size() != 2)
            {
                return epaperRet::BAD_REQUEST;
            }
            posx = pos.at(0).as_integer();
            posy = pos.at(1).as_integer();
        }
        else
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "no position in the json data");
            }
            return epaperRet::BAD_REQUEST;
        }

        //{"bcolor":"white","fcolor":"red","position":[0,0],"font":40,"data":"string detail"}
        guiPaint::getInstance()->printString(data, font, posx, posy, getColor(jValue, "bcolor"), getColor(jValue, "fcolor"));
        return epaperRet::SUCCESS;
    }


    static UWORD getColor(web::json::value jValue, string colorStr = "color")
    {
        UWORD Color = WHITE;
        if (jValue.has_field(colorStr))
        {
            string color = jValue.at(colorStr).as_string();
            if (!color.compare("red"))
            {
                Color = RED;
            }
            else if (!color.compare("white"))
            {
                Color = WHITE;
            }
            else if (!color.compare("black"))
            {
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
};