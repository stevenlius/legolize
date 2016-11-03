#include "Locker.h"
#include <assert.h>

/////////////////
// SIMPLE LOCK //
/////////////////

Locker::Locker(HANDLE mutex) : mutex(mutex), locked(true)
{
   WaitForSingleObject(mutex,INFINITE);
}

void Locker::EarlyUnlock()
{
   assert(locked);
   locked = false;

   ReleaseMutex(mutex);
}

Locker::~Locker()
{
   if (locked)
      ReleaseMutex(mutex);
}

  ////////////////////////
 // READER/WRITER LOCK //
////////////////////////

RWLocker::RWLocker(PSRWLOCK lock, bool writer) : lock(lock), writer(writer), locked(true)
{
   if (writer)
      AcquireSRWLockExclusive(lock);
   else
      AcquireSRWLockShared(lock);
}

void RWLocker::EarlyUnlock()
{
   assert(locked);
   locked = false;

   if (writer)
      ReleaseSRWLockExclusive(lock);
   else
      ReleaseSRWLockShared(lock);
}

RWLocker::~RWLocker()
{
   if (locked)
   {
      if (writer)
         ReleaseSRWLockExclusive(lock);
      else
         ReleaseSRWLockShared(lock);
   }
}