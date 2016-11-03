using System;
using System.Collections.Generic;
using System.Text;

namespace ConsoleApplication
{
    public static class BrickConvertor
    {
        /*
            3005.dat  = 1x1 brick 
            3004.dat = 1x2 brick 
            3010.dat = 1x4 brick
            3001.dat = 2x4 brick
            44237.dat = 2x6 brick
        */
        public static string ConvertBrickTypeToLdrType(BrickType type)
        {
            switch (type)
            {
                case BrickType.B1x1:
                    return "3005.dat";
                    break;
                case BrickType.B2x2:
                    return "3003.dat";
                    break;
                case BrickType.B4x4:
                    throw new NotImplementedException("Brick type " + type + "is not supported");
                    break;
                default:
                    throw new NotImplementedException("Brick type " + type + "is not supported");
                    break;
            }
        } 


        // orientation only on one dimension (y) , -90 and 90 on y should give you same result in our case
        public static string ConvertToLdrOrientation(BrickRotation orientation)
        {
            switch (orientation)
            {
                //orientation is in 
                case BrickRotation.R0:
                    return "1 0 0 0 1 0 0 0 1";
                    break; 
                case BrickRotation.R90:
                    return "0 0 -1 0 1 0 1 0 0";
                    break;
                case BrickRotation.R180:
                    throw new NotImplementedException("Brick orientation " + orientation + "is not supported");
                    break;
                case BrickRotation.R270:
                    throw new NotImplementedException("Brick orientation " + orientation + "is not supported");
                    break;
                default:
                    throw new NotImplementedException("Brick orientation " + orientation + "is not supported");
                    break;
            }
        }

        public static List<string> ConvertToLdr(Brick[] bricks)
        {
            var ldrFileContent = new List<string>();
            ldrFileContent.Add("0 Legolizer"); //file name indication
            
            foreach (var brick in bricks)
            {
                var subPartIndication = "1";
                //get color and position
                var positionBuilder = new StringBuilder("15 "); 
                positionBuilder.Append(brick.X + " ");
                positionBuilder.Append(brick.Y + " ");
                positionBuilder.Append(brick.Z);

                //get orientation 
                var orientationBuilder = new StringBuilder(); 
                orientationBuilder.Append(BrickConvertor.ConvertToLdrOrientation(brick.BrickRotation));

                //convert brick type tp ldr brick id
                var type = BrickConvertor.ConvertBrickTypeToLdrType(brick.BrickType);

                ldrFileContent.Add(subPartIndication + " " + positionBuilder.ToString() + " " + orientationBuilder.ToString() + " " + type);
            }

            return ldrFileContent;
        }
    }
}
