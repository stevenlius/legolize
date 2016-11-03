using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Legolize;
using PointCloudGen;

namespace Legolizer
{
    class Program
    {
        static void Main(string[] args)
        {
            Doer.Do(PointCloudGen.Generator.Cube(10, 8, 1));
        }
    }
}
