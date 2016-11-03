namespace Legolize
{
    public struct Point
    {
        public readonly int X, Y, Z;

        public Point(int x, int y, int z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public override string ToString()
        {
            return $"({X},{Y},{Z})";
        }
    }

    struct MutablePoint
    {
        public int X, Y, Z;

        public MutablePoint(int x, int y, int z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public override string ToString()
        {
            return $"({X},{Y},{Z})";
        }
    }

    public class PointCloud
    {
        public readonly Point[] Cloud;

        public PointCloud(Point[] cloud)
        {
            Cloud = cloud;
        }

        public override string ToString()
        {
            return string.Join("\n", Cloud);
        }
    }
}
