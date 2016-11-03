using System;
using System.Collections.Generic;

namespace Legolize.Algo
{
    class ModelMaster : IModelMaster
    {
        public SlotPriorityQueue Slots { get; }
        public Stack<Brick> Bricks { get; }
        public IModel Model { get; }
        

        public void SlotToBrick(int iSlotPosition)
        {
            throw new NotImplementedException();
        }

        public void CreateNewSlots()
        {
            throw new NotImplementedException();
        }

        public IModelMaster Clone()
        {
            throw new NotImplementedException();
        }

        public IModelMaster DeepClone()
        {
            throw new NotImplementedException();
        }

        public void MoveBack()
        {
            throw new NotImplementedException();
        }
    }
}
