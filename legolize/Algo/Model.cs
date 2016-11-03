using System.Collections;
using System.Text;

namespace Legolize.Algo
{
    class Model : IModel
    {
        private readonly int _xSize;
        private readonly int _ySize;
        private readonly int _zSize;

        private BitArray _vals;

        private readonly int _xTy;

        public Model(int xSize, int ySize, int zSize)
        {
            _xSize = xSize;
            _ySize = ySize;
            _zSize = zSize;

            _xTy = xSize * ySize;

            _vals = new BitArray(_xTy * _zSize, false);
        }

        private Model(Model from)
        {
            _xSize = from._xSize;
            _ySize = from._ySize;
            _zSize = from._zSize;

            _xTy = from._xTy;

            _vals = new BitArray(from._vals);
        }

        public bool this[int x, int y, int z]
        {
            get 
            {
                return _vals[z * _xTy + y * _xSize + x];
            }

            set
            {
                _vals[z * _xTy + y * _xSize + x] = value;
            }
        }

        public IModel DeepCopy() => new Model(this);

        public override string ToString()
        {
            var builder = new StringBuilder();

            for (var iz = 0; iz < _zSize; iz++)
            {
                builder.AppendLine($"Level {iz}");
                for (var iy = 0; iy < _ySize; iy++)
                {
                    for (var ix = 0; ix < _xSize; ix++)
                        builder.Append(this[ix, iy, iz] ? "1" : "0");
                    builder.AppendLine();
                }
            }

            return builder.ToString();
        }
    }
}
