#ifndef __Locker_h_
#define __Locker_h_

#include <Windows.h>
#include <cstddef>

/// Class for locking mechanism
class Locker
{
   HANDLE mutex;

   /// Whether lock is still locked (may be early unlocked)
   bool locked;

   Locker(Locker const &);             //< Disable Copy Constructor
   Locker(Locker &&);                  //< Disable Move Constructor
   Locker & operator=(Locker const &); //< Disable Assign Operator
   Locker & operator=(Locker &&);      //< Disable Move Operator
   void *operator new(std::size_t);    //< Disable Dynamic Allocation

   public:
      Locker(HANDLE mutex);

      void EarlyUnlock();

      ~Locker();
};

/// Class for reader/writer locking
class RWLocker
{
   PSRWLOCK lock;

   /// Whether the owner is writer
   bool writer;

   /// Whether lock is still locked (may be early unlocked)
   bool locked;

   RWLocker(RWLocker const &);               //< Disable Copy Constructor
   RWLocker(RWLocker &&);                    //< Disable Move Constructor
   RWLocker & operator=(RWLocker const &);   //< Disable Assign Operator
   RWLocker & operator=(RWLocker &&);        //< Disable Move Operator
   void *operator new(std::size_t);          //< Disable Dynamic Allocation

   public:
      /// Constructor
      /// Locks the lock
      /// \param [in] mutex Mutex to be locked
      /// \param [in] writer Set to true if locking exclusive
      RWLocker(PSRWLOCK lock, bool writer = false);

      void EarlyUnlock();

      ~RWLocker();
};

#endif