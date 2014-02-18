#ifndef _H_SYMPTOGEN_SINGLETON_H_
#define _H_SYMPTOGEN_SINGLETON_H_

#include <stdlib.h>

namespace Symp {

template <typename T>
class Singleton
{
protected:
    /**
    *   Constructor
    */
    Singleton() { }

    /**
    *   Destructor
    */
    ~Singleton () { }

public:
    
    /**
    *   Return the singleton
    *   @return T* : the instance of the singleton T
    *   @see getInstance()
    *   @see removeInstance()
    */
    static T* getInstance () {
         if(mSingleton == NULL) {
            mSingleton = new T;
        }

        return (static_cast<T*>(mSingleton));
    }

    /**
    *   Remove the singleton
    *   @see getInstance()
    *   @see removeInstance()
    */
    static void removeInstance() {
         if(mSingleton != NULL) {
            delete mSingleton;
            mSingleton = NULL;
        }
    }

private:
    static T *mSingleton;

};


template <typename T>
T *Singleton<T>::mSingleton = NULL;

}

#endif // _H_SYMPTOGEN_SINGLETON_H_