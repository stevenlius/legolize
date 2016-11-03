using System.Collections.Generic;

namespace Legolize.Algo
{
    interface IModel
    {
        bool this[int x, int y, int z] { get; set; }
    }

    interface Brick
    {
        Point LeftLowNear { get; }
        Point RightUpFar { get; }
    }

    interface IBrickStack
    {
        void Push(Brick brick);
        Brick Pop();
        Brick Peek();
    }

    struct Slot
    {
        public int Priority { get; private set; }
        public void IncreasePriority() => Priority++;                    
        public Brick Brick { get; }
    }

    interface SlotPriorityQueue : IList<Slot>
    {
        SlotPriorityQueue DeepCopy();
        void IncreasePriority(int iSlot);        
    }

    interface ModelMaster
    {
        SlotPriorityQueue Slots { get; }

        IBrickStack Bricks { get; }
        IModel Model { get; }

        // remove slot from SlotPriorityQueue
        // add brick to brick queue
        // remove invalid slots from SlotPriorityQueue
        // update slot priorities
        // update model to remove occupied possitions
        void SlotToBrick(int iSlotPosition);

        // Take latest added Brick
        // Add new slots introduces by Brick
        void CreateNewSlots();

        // clone SlotPriorityQueue
        // references Model
        // references BrickStack
        ModelMaster Clone();

        // clone SlotPriorityQueue
        // clone Model
        // clone BrickStack
        ModelMaster DeepClone();

        // Move forward
        // Clone(), SlotToBrick, CreateNewSlots

        // pop brick, update Model
        void MoveBack();        
    }

}
