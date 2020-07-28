#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;

using HostInetInfo = tcp::resolver::iterator;

class NetworkUtils
{
private:
   static HostInetInfo queryHostInetInfo()
   {
      io_service ios;
      tcp::resolver resolver(ios);
      tcp::resolver::query query(host_name(), "");
      return resolver.resolve(query);
   }
   static std::string hostIP(unsigned short family)
   {
      auto hostInetInfo = queryHostInetInfo();
      tcp::resolver::iterator end;
      while (hostInetInfo != end)
      {
         tcp::endpoint ep = *hostInetInfo++;
         sockaddr sa = *ep.data();
         if (sa.sa_family == family)
         {
            return ep.address().to_string();
         }
      }
      return nullptr;
   }

public:
   // gets the host IP4 string formatted
   static std::string hostIP4()
   {
      return hostIP(AF_INET);
   }

   // gets the host IP6 string formatted
   static std::string hostIP6()
   {

      return hostIP(AF_INET6);
   }
   static std::string hostName()
   {
      return ip::host_name();
   }
};
