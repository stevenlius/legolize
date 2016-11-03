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
            // remove slot from SlotPriorityQueue
            var slot = Slots[iSlotPosition];
            Slots.RemoveAt(iSlotPosition);

            var brick = slot.Brick;
            // add brick to brick queue
            Bricks.Push(brick);

            // update model to remove occupied possitions
            Model.Set(brick, false);

            // remove invalid slots from SlotPriorityQueue
            var n = Slots.Count;
            for(var i = 0; i < n; i++)
                if(Slots[i].Brick.InCollision(brick))
                {
                    Slots.RemoveAt(i);
                    i--;
                    n--;
                }

            // update slot priorities
            for (var i = 0; i < n; i++)
                if (Slots[i].Brick.InTouch(brick))
                {
                    Slots.IncreasePriority(i);
                }
        }

        private static Brick[] AllNewBricksFor(Brick brick)
        {
            // hardcoding all bricks 

            // 1x1
            //for(var ix = brick; ix )
            return Array.Empty<Brick>();
        }

        // Take latest added Brick
        // Add new slots introduces by Brick
        public void CreateNewSlots()
        {
            var brick = Bricks.Peek();
            var potentialBricks = AllNewBricksFor(brick);

        }

        private ModelMaster(ModelMaster from, bool deepClone)
        {
            Slots = from.Slots.DeepClone();
            if (!deepClone)
            {
                Bricks = from.Bricks;
                Model = from.Model;
            }
            else
            {
                Bricks = new Stack<Brick>(from.Bricks);
                Model = from.Model.DeepClone();
            }
        }

        public IModelMaster Clone()
        {
            return new ModelMaster(this, false);
        }

        public IModelMaster DeepClone()
        {
            return new ModelMaster(this, true);
        }
        
        public void MoveBack()
        {
            // pop brick, update Model
            Model.Set(Bricks.Pop(), true);
        }        
    }
}
