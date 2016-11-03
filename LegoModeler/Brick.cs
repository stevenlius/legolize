
namespace LegoModeler
{
    public enum BrickType
    {
        B1x1,
        B2x2,
        B4x4
    }

    public enum BrickRotation
    {
        R0,
        R90,
        R180,
        R270
    }

    public struct Brick
    {
        public readonly BrickType BrickType;
        public readonly int X, Y, Z;        
        public readonly BrickRotation BrickRotation;

        public Brick(BrickType brickType, int x, int y, int z, BrickRotation brickRotation)
        {
            BrickType = brickType;
            X = x;
            Y = y;
            Z = z;
            BrickRotation = brickRotation;
        }
    }    
}
