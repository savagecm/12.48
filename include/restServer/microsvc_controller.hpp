#pragma once

#include <cpprest/http_msg.h>
#include "restServer/basic_controller.hpp"
#include "restServer/epaperProcessor.hpp"
#include "logger/logger.hpp"
#include "ePaper/epd12in48.hpp"
#include "ePaper/guiPaint.hpp"

#define BOOST_SPIRIT_THREADSAFE // enable thread safety to json read on property tree!

#include <boost/format.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <regex>

class MicroserviceController : public BasicController, Controller
{
public:
    MicroserviceController() : BasicController() {}
    ~MicroserviceController() {}
    void handleGet(http_request message) override
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "get a get message : " << message.to_string());
        }
        auto path = requestPath(message);
        if (!path.empty())
        {
            if (path[0] == "service" && path[1] == "test")
            {
                auto response = json::value::object();
                response["version"] = json::value::string("0.1.1");
                response["status"] = json::value::string("ready!");
                message.reply(status_codes::OK, response);
            }
        }

        message.reply(status_codes::NotFound);
    }
    void handlePut(http_request message) override
    {
        handleMessage(message, methods::PUT);
    }

    void handlePost(http_request message) override
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "get a post message : "); //<< message.to_string());
        }
        handleMessage(message, methods::POST);
    }

    void handlePatch(http_request message) override
    {
        handleMessage(message, methods::PATCH);
    }
    void handleDelete(http_request message) override
    {
        message.reply(status_codes::NotImplemented, responseNotImpl(methods::DEL));
    }
    void handleHead(http_request message) override
    {
        message.reply(status_codes::NotImplemented, responseNotImpl(methods::HEAD));
    }
    void handleOptions(http_request message) override
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "get a option message : " << message.to_string());
        }
        http_response response(status_codes::OK);
        response.headers().add(U("Allow"), U("GET, POST, OPTIONS, PATCH"));
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, OPTIONS, PATCH"));
        response.headers().add(U("Access-Control-Allow-Headers"), U("Origin, X-Requested-With, Content-Type, Accept, content-type1"));
        message.reply(response);
    }
    void handleTrace(http_request message) override
    {
        message.reply(status_codes::NotImplemented, responseNotImpl(methods::TRCE));
    }
    void handleConnect(http_request message) override
    {
        message.reply(status_codes::NotImplemented, responseNotImpl(methods::CONNECT));
    }
    void handleMerge(http_request message) override
    {
        message.reply(status_codes::NotImplemented, responseNotImpl(methods::MERGE));
    }
    void initRestOpHandlers() override
    {
        _listener.support(methods::GET, std::bind(&MicroserviceController::handleGet, this, std::placeholders::_1));
        _listener.support(methods::PUT, std::bind(&MicroserviceController::handlePut, this, std::placeholders::_1));
        _listener.support(methods::POST, std::bind(&MicroserviceController::handlePost, this, std::placeholders::_1));
        _listener.support(methods::DEL, std::bind(&MicroserviceController::handleDelete, this, std::placeholders::_1));
        _listener.support(methods::PATCH, std::bind(&MicroserviceController::handlePatch, this, std::placeholders::_1));
        _listener.support(methods::OPTIONS, std::bind(&MicroserviceController::handleOptions, this, std::placeholders::_1));
    }

    // need path not empty
    epaperRet ProcessData(web::json::value jValue, std::vector<utility::string_t> path, const http::method &method)
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "in function ProcessData");
        }
        guiPaint *globPaint = guiPaint::getInstance();
        if (method == methods::POST || method == methods::PUT)
        {
            // for post, it means create
            // for put, it means update and user provide the whole info
            // so for these two methods, we need to clear first
            // globPaint->Paint_Clear(WHITE);
        }

        // if we go into this function. the path is not empty and the path start with image
        if (path[0] == "epaper")
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "path with epaper");
            }
            if (path[1] == "line")
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "path with epaper/line");
                }
                epaperProcessor::processLine(jValue);
            }
            else if (path[1] == "circle")
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "path with epaper/circle");
                }
                epaperProcessor::processCircle(jValue);
            }
            else if (path[1] == "rectangle")
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "path with epaper/rectangle");
                }
                epaperProcessor::processRectangle(jValue);
            }
            else if (path[1] == "point")
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "path with epaper/point");
                }
                epaperProcessor::processPoint(jValue);
            }
            else if (path[1] == "string")
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "path with epaper/string");
                }
                epaperProcessor::processString(jValue);
            }
            else if (path[1] == "image")
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "path with epaper/image");
                }
                epaperProcessor::processImage(jValue);
            }
            else if (path[1] == "group")
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "path with epaper/group");
                }
                epaperProcessor::processGroup(jValue);
            }
            else if (path[1] == "rotate")
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "path with epaper/picRotate");
                }
                epaperProcessor::processRotate(jValue);
            }
            else if (path[1] == "display")
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "path with epaper/display");
                }
                epd12in48::EPD_12in48B_Display(guiPaint::getInstance()->getBImage(), guiPaint::getInstance()->getRImage());
            }
            else if (path[1] == "clear")
            {
                globPaint->Paint_Clear(WHITE);
            }
            else
            {
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "not spuuort path : " << path[1]);
                }
                return epaperRet::NOT_SUPPORT;
            }

            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "display frame return");
            }
            return epaperRet::SUCCESS;
        }
        else
        {
            return epaperRet::NOT_SUPPORT;
        }
    }
    void handleMessage(http_request message, const http::method &method)
    {
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "enter functiuon handleMessage()");
        }
        auto path = requestPath(message);
        message.headers().add(U("Access-Control-Allow-Origin"), U("*"));
        if (!path.empty())
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "path size is : " << path.size());
            }
            web::json::value jValue;
            try
            {
                jValue = message.extract_json().get();
            }
            catch (const http_exception &e)
            {
                // Print error.
                if (CHECK_LOG_LEVEL(debug))
                {
                    __LOG(debug, "invalid json format, error is : " << e.what());
                }
                message.reply(status_codes::BadRequest);
                return;
            }
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "now try to process data with json value and method");
            }
            switch (ProcessData(jValue, path, method))
            {
            case epaperRet::SUCCESS:
                message.reply(status_codes::OK, jValue);
                break;
            case epaperRet::SUCCESS_CREATED:
                message.reply(status_codes::Created, jValue);
                break;
            case epaperRet::NOT_SUPPORT:
                message.reply(status_codes::NotFound);
                break;
            case epaperRet::BAD_REQUEST:
                message.reply(status_codes::BadRequest);
                break;
            default:
                message.reply(status_codes::BadRequest);
            }
        }
        else
        {
            if (CHECK_LOG_LEVEL(debug))
            {
                __LOG(debug, "path is empty");
            }
            message.reply(status_codes::NotFound);
        }
    }

private:
    static json::value responseNotImpl(const http::method &method)
    {
        auto response = json::value::object();
        response["serviceName"] = json::value::string("epaper: function not implement!");
        response["http_method"] = json::value::string(method);
        return response;
    }
};