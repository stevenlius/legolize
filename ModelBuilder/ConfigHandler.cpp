#include "ConfigHandler.h"
#include <sstream>
#include "HeightMapHandler.h"

void ConfigHandler::createDefaultConfig_HeightMapHandler(std::ostream & stream)
{  
   stream
      << std::endl << ";;;HeightMapHandler settings;;;" << std::endl << std::endl
      << "WIDTH_MIN " << HeightMapHandler::WIDTH_MIN << "\t\t; Left extreme" << std::endl
      << "WIDTH_MAX " << HeightMapHandler::WIDTH_MAX << "\t\t; Right extreme" << std::endl
      << "HEIGHT_MIN " << HeightMapHandler::HEIGHT_MIN << "\t\t; Top extreme" << std::endl
      << "HEIGHT_MAX " << HeightMapHandler::HEIGHT_MAX << "\t\t; Bottom extreme" << std::endl
      << "DEPTH_MIN " << HeightMapHandler::DEPTH_MIN << "\t\t; Near extreme" << std::endl
      << "DEPTH_MAX " << HeightMapHandler::DEPTH_MAX << "\t\t; Far extreme" << std::endl
      ;
}

bool ConfigHandler::readFromConfig_HeightMapHandler(std::istream & stream, std::string const & label)
{

   if (label == "WIDTH_MIN")
   {
      stream >> HeightMapHandler::WIDTH_MIN;
      return true;
   }

   if (label == "WIDTH_MAX")
   {
      stream >> HeightMapHandler::WIDTH_MAX;
      return true;
   }

   if (label == "HEIGHT_MIN")
   {
      stream >> HeightMapHandler::HEIGHT_MIN;
      return true;
   }

   if (label == "HEIGHT_MAX")
   {
      stream >> HeightMapHandler::HEIGHT_MAX;
      return true;
   }

   if (label == "DEPTH_MIN")
   {
      stream >> HeightMapHandler::DEPTH_MIN;
      return true;
   }

   if (label == "DEPTH_MAX")
   {
      stream >> HeightMapHandler::DEPTH_MAX;
      return true;
   }

   return false;
}

void ConfigHandler::readFromConfig(std::istream & stream)
{
   // walk config line by line
   std::string oneLine;
   while(std::getline(stream,oneLine))
   {
      // extract name
      std::stringstream lineCont(oneLine);
      lineCont >> oneLine;

      // assign to correct variable //

      // HALT
      if (oneLine == "HALT")
         exit(0);

      // Configuration readers
      bool hit(readFromConfig_HeightMapHandler(lineCont, oneLine));;

      // unknown (filter comment/blank)
      if(!hit && !oneLine.empty() && oneLine[0] != ';')
         std::cout << "unparseable config line: " << oneLine << std::endl;
   }
}

void ConfigHandler::createDefaultConfig(std::ostream & stream)
{
   createDefaultConfig_HeightMapHandler(stream);
}

void ConfigHandler::ProcessConfig(char const * fileName)
{
   // try to open config
   std::fstream config(fileName,std::ios::in);

   // Process config if present
   if (config)
   {
      readFromConfig(config);
      config.close();
   }
   // Create config
   else
   {
      config.open(fileName,std::ios::out);
      createDefaultConfig(config);
      config.close();

      std::cout << "CONFIG MISSING, attempted to create a default one" << std::endl;
   }
}