using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Legolize.Algo;

namespace Legolize
{
    public static class Doer
    {
        public static void Do(PointCloud cloud)
        {
            Console.Write($"PointCloud with {cloud.Cloud.Length} points");

            // 1.) create model 
            var min = new MutablePoint(int.MaxValue, int.MaxValue, int.MaxValue);
            var max = new MutablePoint(int.MinValue, int.MinValue, int.MinValue);

            foreach(var point in cloud.Cloud)
            {
                min.X = Math.Min(min.X, point.X);
                min.Y = Math.Min(min.Y, point.Y);
                min.Z = Math.Min(min.Z, point.Z);

                max.X = Math.Max(max.X, point.X);
                max.Y = Math.Max(max.Y, point.Y);
                max.Z = Math.Max(max.Z, point.Z);
            }

            var model = new Model(max.X - min.X + 1, max.Y - min.Y + 1, max.Z - min.Z+1);
            foreach (var point in cloud.Cloud)
                model[point.X - min.X, point.Y - min.Y, point.Z - min.Z] = true;

            Console.Write(model);
        }
    }
}
