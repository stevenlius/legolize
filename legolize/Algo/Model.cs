using System;
using System.Collections;
using System.Text;

namespace Legolize.Algo
{
    class Model : IModel
    {
        private readonly int _xSize;
        private readonly int _ySize;
        private readonly int _zSize;

        private ulong[] _vals;

        private readonly int _xTy;

        public Model(int xSize, int ySize, int zSize)
        {
            _xSize = xSize;
            _ySize = ySize;
            _zSize = zSize;

            _xTy = xSize * ySize;

            _vals = new ulong[(_xTy * _zSize + 63) / 64];
        }

        private Model(Model from)
        {
            _xSize = from._xSize;
            _ySize = from._ySize;
            _zSize = from._zSize;

            _xTy = from._xTy;

            _vals = new ulong[from._vals.Length];
            Array.Copy(from._vals, _vals, _vals.Length);
        }

        private bool this[int pos]
        {
            get
            {
                return ((_vals[pos >> 6] >> (pos & 63)) & 0x1) == 0x1;
            }

            set
            {
                if(value)
                    _vals[pos >> 6] |= 1ul << (pos & 63);
                else
                    _vals[pos >> 6] &= ~(1ul << (pos & 63));
            }
        }

        public bool this[int x, int y, int z]
        {
            get 
            {
                return this[z * _xTy + y * _xSize + x];
            }

            set
            {
                this[z * _xTy + y * _xSize + x] = value;
            }
        }

        public bool Can(Brick brick)
        {
            // todo faster algo
            for (var iz = brick.LeftLowNear.Z; iz < brick.RightUpFar.Z; iz++)
                for (var iy = brick.LeftLowNear.Y; iy < brick.RightUpFar.Y; iy++)
                    for (var ix = brick.LeftLowNear.X; iz < brick.RightUpFar.X; ix++)
                        if (!this[ix, iy, iz])
                            return false;

            return true;            
        }

        public void Set(Brick brick, bool value)
        {
            // todo faster algo
            for (var iz = brick.LeftLowNear.Z; iz < brick.RightUpFar.Z; iz++)
                for (var iy = brick.LeftLowNear.Y; iy < brick.RightUpFar.Y; iy++)
                    for (var ix = brick.LeftLowNear.X; iz < brick.RightUpFar.X; ix++)
                        this[ix, iy, iz] = value;
        }

        public IModel DeepClone() => new Model(this);

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
