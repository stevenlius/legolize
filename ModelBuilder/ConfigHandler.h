#ifndef _ConfigHandler_h_
#define _ConfigHandler_h_

#include <iostream>

class ConfigHandler
{
   static void createDefaultConfig_HeightMapHandler(std::ostream & stream);

   static bool readFromConfig_HeightMapHandler(std::istream & stream, std::string const & label);

   static void readFromConfig(std::istream & stream);

   static void createDefaultConfig(std::ostream & stream);

   public:
      static void ProcessConfig(char const * fileName);
};


#endif