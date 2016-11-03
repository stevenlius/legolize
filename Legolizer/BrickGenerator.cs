using System;

namespace ConsoleApplication
{
    public class GenerateStaticBricks : IGenerator
    {
        //Generate some bricks
        public static Brick[] make()
        {
            var ran1 = new Brick(BrickType.B2x2, -20,-24,20, BrickRotation.R0);
            var ran2 = new Brick(BrickType.B2x2, -20,-24,-20, BrickRotation.R0);
            var ran3 = new Brick(BrickType.B2x2, -20,-24,60, BrickRotation.R0);
            var ran4 = new Brick(BrickType.B2x2, -20,-48,60, BrickRotation.R0);
            var ran5 = new Brick(BrickType.B2x2, -20,-72,60, BrickRotation.R0);
            return new []{ran1, ran2,ran3, ran4, ran5};
        }

        public void OutputToLdr(Brick[] bricks)
        {
            var ldrResult = BrickConvertor.ConvertToLdr(bricks);

            //output to file
            System.IO.StreamWriter file = new System.IO.StreamWriter("c:\\legolize.ldr");
            foreach (var item in ldrResult)
               file.WriteLine(item);
            file.Close();
        }
    }
}
