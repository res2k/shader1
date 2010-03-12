#ifndef __TESTERRORHANDLER_H__
#define __TESTERRORHANDLER_H__

struct TestErrorHandler : public s1::LexerErrorHandler
{
  bool invalidCharFound;
  bool strayCharFound;
  UChar32 strayCharOffender;
  
  TestErrorHandler() { Reset(); }
    
  void Reset ()
  {
    invalidCharFound = false;
    strayCharFound = false;
    strayCharOffender = 0;
  }
  
  void InputInvalidCharacter () { invalidCharFound = true; }
  void StrayCharacter (UChar32 offender)
  {
    strayCharFound = true;
    strayCharOffender = offender;
  }
};

#endif // __TESTERRORHANDLER_H__
