#ifndef _CommonClassMacros_h_
#define _CommonClassMacros_h_

#define NO_COPY_CONSTRUCT(Class) Class(Class const &) = delete
#define NO_MOVE_CONSTRUCT(Class) Class(Class &&) = delete
#define NO_COPY_ASSIGN(Class) Class & operator=(Class const &) = delete
#define NO_MOVE_ASSIGN(Class) Class & operator=(Class &&) = delete

#define NO_COPY(Class) NO_COPY_CONSTRUCT(Class); NO_COPY_ASSIGN(Class)
#define NO_MOVE(Class) NO_MOVE_CONSTRUCT(Class); NO_MOVE_ASSIGN(Class)

#define NO_COPYMOVE(Class) NO_COPY(Class); NO_MOVE(Class)

#define DEFAULT_COPY_CONSTRUCT(Class) Class(Class const &) = default
#define DEFAULT_MOVE_CONSTRUCT(Class) Class(Class &&) = default
#define DEFAULT_COPY_ASSIGN(Class) Class & operator=(Class const &) = default
#define DEFAULT_MOVE_ASSIGN(Class) Class & operator=(Class &&) = default

#define DEFAULT_COPY(Class) DEFAULT_COPY_CONSTRUCT(Class); DEFAULT_COPY_ASSIGN(Class)
#define DEFAULT_MOVE(Class) DEFAULT_MOVE_CONSTRUCT(Class); DEFAULT_MOVE_ASSIGN(Class)

#define DEFAULT_COPYMOVE(Class) DEFAULT_COPY(Class); DEFAULT_MOVE(Class)

#define DEFAULT_CONSTRUCT(Class) Class() = default
#define DEFAULT_DESTROY(Class) ~Class() = default

#endif