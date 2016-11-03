namespace legolize
{
    struct Point
    {
        public readonly int X, Y, Z;

        public Point(int x, int y, int z)
        {
            X = x;
            Y = y;
            Z = z;
        }

    }

    class PointCloud
    {
        public readonly Point[] Cloud;

        public PointCloud(Point[] cloud)
        {
            Cloud = cloud;
        }
    }
}
