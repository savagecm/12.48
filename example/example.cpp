

#include "restServer/microsvc_controller.hpp"
#include "restServer/runtime_utils.hpp"
#include "restServer/usr_interrupt_handler.hpp"
#include "ePaper/epd12in48.hpp"
#include "logger/simple_logger.hpp"
int main()
{
    // set up logger
    std::unique_ptr<simpleLogger> simpleLoggerUptr(new simpleLogger());
    INIT_LOGGER(simpleLoggerUptr);
    SET_LOG_LEVEL(debug);

    if (CHECK_LOG_LEVEL(debug))
    {
        __LOG(debug, "now epaper init, logger init success");
    }
    // init epaper
    epd12in48::EPD_12in48B_Init();
    guiPaint::getInstance();
    // init rest server
    MicroserviceController server;
    http_listener_config server_config;
    server.setEndpoint("http://0.0.0.0:6502/v1/api", server_config);

    try
    {
        // wait for server initialization...
        server.accept().wait();
        std::cout << "Modern C++ Microservice now listening for requests at: " << server.endpoint() << '\n';

        InterruptHandler::waitForUserInterrupt();
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "receive Interrupt, now start to shutdown");
        }
        server.shutdown().wait();
        if (CHECK_LOG_LEVEL(debug))
        {
            __LOG(debug, "after shutdown, programm going to exit");
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "somehitng wrong happen! :( " << '\n';
    }
    catch (...)
    {
        RuntimeUtils::printStackTrace();
    }
}