#ifndef _H_SYMPTOGEN_MODULE_FILENAME_H_
#define _H_SYMPTOGEN_MODULE_FILENAME_H_

// Tab indents, with tabstop at four spaces.

// Braces in your code should look like the following example: 
for (int i = 0; i < t; i++) {
    // Do something
}
 
if (j < k) {
    // Do something
} else if (j > k) {
    // Do something
} else {
    // Do something
}
 
class Dummy {
    // ...
};

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

// Array delete operator has no whitespace before []
delete[] foo;

// No whitespace between template keyword and < 
template<typename foo>

// No whitespace after a cast; and in a pointer, we write a whitespace after the start but not before it.
const char* ptr = (const char *)foobar;

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
} // End switch

// Naming Constants
const int SOME_KLUDGY_CONSTANT_NAME;

// Naming Classes and Structs : Camel case starting with upper case. 
class MyClass { /* ... */ };
struct MyStruct { /* ... */ };

// Naming class member variable : prefix with "m_" then CamelCase starting with lowercase.
int m_foo;
int m_aBigFoo;

// Naming class methods : CamelCase starting with lowercase
void aRealGreatMethod();

// Local variable : CamelCase starting with lowercase. No underscore separators
char* someVariableName;





#endif // _H_SYMPTOGEN_MODULE_FILENAME_H_
