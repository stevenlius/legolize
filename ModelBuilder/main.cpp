#include "ConfigHandler.h"
#include "HeightMapHandler.h"

int main(int argc, char * argv[])
{
   // Process config
   ConfigHandler::ProcessConfig("HeightMapLoader.conf");

   // Invalid arguments, print usage
   if (argc<3)
   {
      std::cout << "Usage standard: " << std::endl;
      std::cout << "program.exe <inputFile_heightMap> <outputFile_triangleFile>" << std::endl;
      std::cout << "Usage High Definition: " << std::endl;
      std::cout << "program.exe <inputFile_heightMap> <outputFile_triangleFile> HiDef" << std::endl;
      return 1;
   }

   // Prepare heightMapHandler
   HeightMapHandler heightMap;
   
   // Load BMP file
   bool loadSucceeded(heightMap.LoadBmpFile(argv[1]));
   if (!loadSucceeded)
   {
      std::cout << "File load failed" << std::endl;
      return 1;
   }
   else
      std::cout << "File loaded" << std::endl;

   // Save triangle file
   bool saveSucceeded(heightMap.SaveTriangleFile(argv[2], (argc>3 && !strcmp(argv[3],"HiDef"))));
   if (!saveSucceeded)
   {
      std::cout << "File save failed" << std::endl;
      return 1;
   }
   else
      std::cout << "File saved" << std::endl;

   // Return OK
   return 0;
}