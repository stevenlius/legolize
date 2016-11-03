#ifndef _HeightMapHandler_h_
#define _HeightMapHandler_h_

struct Triple;
class HeightMapHandler;

#include <windows.h>
#include <fstream>

struct Triple
{
   float X;
   float Y;
   float Z;

   Triple operator-(Triple const & other) const;

   static Triple CrossProduct(Triple const & left, Triple const & right);

   static float DotProduct(Triple const & left, Triple const & right);

   static Triple ComputeNormal(Triple const & a, Triple const & b, Triple const & c);

   Triple(float x, float y, float z);
   
   void Serialize(std::ostream & stream) const;
};

class HeightMapHandler
{    
   friend class ConfigHandler;

   static float WIDTH_MIN;    //< Left
   static float WIDTH_MAX;    //< Right
   static float HEIGHT_MIN;   //< Top
   static float HEIGHT_MAX;   //< Bottom
   static float DEPTH_MIN;    //< Near
   static float DEPTH_MAX;    //< Far

   /// File header (first header in BMP)
   BITMAPFILEHEADER fileHeader;

   /// Info header (second header in BMP)
   BITMAPINFOHEADER infoHeader;

   /// Maximal value on height map
   int maxDepth;

   /// Minimal value on height map
   int minDepth;

   /// Core data of bitmap (0 if file not valid)
   unsigned char * coreData;

   /// Crawles bitMap for max & min values
   void setMaxMin();

   /// Store given triangle into provided stream
   /// \param [in] stream Stream where to write the triangle
   /// \param [in] a Point A of triangle
   /// \param [in] b Point B of triangle
   /// \param [in] c Point C of triangle
   /// \return whether operation succeeded
   bool storeTriangle(std::ostream & stream, Triple const & a, Triple const & b, Triple const & c) const;

   /// Extracts value from given position (respects bit depth), also advances pointer to next value
   /// \param [in,out] firstByte First byte of desired value, will be advanced to next value
   /// \return Height value
   int extractValue(unsigned char const * & firstByte) const;

   /// Extracts value from given position as X Y coordinates
   /// \param [in] x X coord
   /// \param [in] y Y coord
   int extractValue(int x, int y) const;

   /// Normalize values into triple
   /// \param [in] x X coordinate
   /// \param [in] y Y coordinate
   /// \param [in] z Z coordinate
   Triple normalizeValues(int x, int y, int z) const;

   public:
      HeightMapHandler();

      /// Load height map from BMP file
      /// \param [in] fileName File with BMP heightmap
      /// \return Whether operation succeeded
      bool LoadBmpFile(char const * fileName);      

      /// Save trianges corresponding to loaded height map with additional normalization
      /// \param [in] fileName File where to store triangle map
      /// \param [in] hiDef Whether to use higher definition Triangulation
      /// \return Whether operation succeeded
      bool SaveTriangleFile(char const * fileName, bool hiDef);

      ~HeightMapHandler();
};

#endif