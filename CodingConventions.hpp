#ifndef _H_SYMPTOGEN_MODULE_FILENAME_H_
#define _H_SYMPTOGEN_MODULE_FILENAME_H_

// Tab indents, with tabstop at four spaces.


/* *************************************************************************************** */
/* NAMING */
/* *************************************************************************************** */

// Constants : uppercase with underscores separators
const int SOME_KLUDGY_CONSTANT_NAME;

// Classes and Structs : CamelCase starting with uppercase. No underscore separators.
class MyClass { /* ... */ };
struct MyStruct { /* ... */ };

// Class member variable : prefix with "m_" then CamelCase starting with lowercase. No underscore separators.
// For basic types (int, float, double, boolean, char), use a prefix corresponding to the type ('i', 'f', 'd', 'b', 'c')
// For pointers, use prefix "m_p" and do not use type prefix.
int m_iFoo;
bool m_bIsFooOnMyPlate;
char* m_pFooBar;

// Class methods : CamelCase starting with lowercase. No underscore separators
void aRealGreatMethod();

// Local variable : CamelCase starting with lowercase. No underscore separators
char* someVariableName;

// Global variables : In general you should avoid global variables.
// If it can't be avoided, perfix with 'g_' then CamelCase starting with lowercase. No underscore separators. 
int g_someGlobalVariable;

// Special Keywords
// "TODO" : marks incomplete code, or things that could be done better but are left for the future.


/* *************************************************************************************** */
/* HUGGING BRACES */
/* *************************************************************************************** */

// Braces in your code should look like the following example:
for (int i = 0; i < t; i++) {
    // Do something
}
 
if (j < k) {
    // Do something
}
else if (j > k) {
    // Do something
}
else {
    // Do something
}
 
class Dummy {
    // ...
};

/* *************************************************************************************** */
/* WHITESPACES */
/* *************************************************************************************** */

// Conventional operators surrounded by a space character
a = (b + c) * d;

// C++ reserved words separated from opening parentheses by a white space
while (true) {}

// Commas followed by a white space
someFunction(a, b, c);
int d, e;

// Semicolons followed by a space character, if there is more on a line
for (int a = 0; b < c; d++) {}

// When declaring class inheritance and in a ? construct, colons should be surrounded by white space
class BusWheel : public RubberInflatable {};
(isNight) ? colorMeDark() : colorMeBright();

// Indentation level is not increased after namespace clause
namespace Symptogen {
 
byte Actor::kInvalidBox = 0;
 
void Actor::initActorClass(ScummEngine *scumm) {
    _vm = scumm;
}
 
} // End of namespace Scumm

// Indentation level is not increased after class clause for the keywords "public" and "private".
// There is no whitespace between the keywords "public"/"private" and the colons.
class Foo {
public:
    Foo();
private:
    int m_bar;
    int m_bar2;
};

// Array delete operator has no whitespace before []
delete[] foo;

// No whitespace between template keyword and < 
template<typename foo>

// No whitespace after a cast.
// In a pointer, we write a whitespace after the * character but not before it. The * is stuck in the type.
const char* ptr = (const char*)foobar;

// We use the same rule for references as we do for pointers : use a whitespace after the "&" but not before it. 
int i = 0;
int& ref = i;

// Switch case construct
switch (cmd) {
    case kSomeCmd:
        someCmd();
        // Fall Through intended
    case kSomeVerySimilarCmd:
        someMoreCmd();
        break;
    case kSaveCmd:
        save();
        break;
    case kLoadCmd:
    case kPlayCmd:
        close();
        break;
    default:
        Dialog::handleCommand(sender, cmd, data);
        break;
} // End switch


/* *************************************************************************************** */
/* Documenting the code */
/* *************************************************************************************** */

// We use Doxygen comenting model.
// Comments in the code should look like the following example.

/**
* A test class. A more elaborate class description.
*/
class Test {
public:
    /**
    * An enum.
    * More detailed enum description.
    */
    enum TEnum {
    TVal1, /**< enum value TVal1. */
    TVal2, /**< enum value TVal2. */
    TVal3 /**< enum value TVal3. */
    }
    *enumPtr, /**< enum pointer. Details. */
    enumVar; /**< enum variable. Details. */
    /**
    * A constructor.
    * A more elaborate description of the constructor.
    */
    Test();
    /**
    * A destructor.
    * A more elaborate description of the destructor.
    */
    ~Test();
    /**
    * a normal member taking two arguments and returning an integer value.
    * @param a an integer argument.
    * @param s a constant character pointer.
    * @see Test()
    * @see ~Test()
    * @see testMeToo()
    * @see publicVar()
    * @return The test results
    */
    int testMe(int a, const char* s);
    /**
    * A pure virtual member.
    * @see testMe()
    * @param c1 the first argument.
    * @param c2 the second argument.
    */
    virtual void testMeToo(char c1, char c2) = 0;
    
private:
    /**
    * a private variable.
    * Details.
    */
    int m_publicVar;

};

#endif // _H_SYMPTOGEN_MODULE_FILENAME_H_
