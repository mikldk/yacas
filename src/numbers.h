
#ifndef __numbers_h__
#define __numbers_h__

#include "lispenvironment.h"
#include "yacasbase.h"


/// Create a internal number object from an ascii string.
void* AsciiToNumber(LispCharPtr aString,LispInt aPrecision);

/// Convert from internal number format to ascii format
LispStringPtr NumberToAscii(void* aNumber,LispHashTable& aHashTable,
                           LispInt aBase);

/// Whether the numeric library supports 1.0E-10 and such.
LispInt NumericSupportForMantissa();

/// Numeric library name
const LispCharPtr NumericLibraryName();

/// Copy a number class
void* NumberCopy(void* aOriginal);

/// Delete a number object.
void NumberDestroy(void* aNumber);


LispStringPtr GcdInteger(LispCharPtr int1, LispCharPtr int2,
                         LispHashTable& aHashTable);

LispStringPtr MultiplyFloat(LispCharPtr int1, LispCharPtr int2,
                            LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr AddFloat(LispCharPtr int1, LispCharPtr int2,
                       LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr PlusFloat(LispCharPtr int1,LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr SubtractFloat(LispCharPtr int1, LispCharPtr int2,
                            LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr NegateFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr DivideFloat(LispCharPtr int1, LispCharPtr int2,
                          LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr PowerFloat(LispCharPtr int1, LispCharPtr int2,
                         LispHashTable& aHashTable,LispInt aPrecision);

LispStringPtr SinFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr CosFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr TanFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr ArcSinFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr ArcCosFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr ArcTanFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr ExpFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr LnFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);

LispStringPtr SqrtFloat(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr AbsFloat( LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);

LispBoolean GreaterThan(LispCharPtr int1, LispCharPtr int2,
                       LispHashTable& aHashTable,LispInt aPrecision);
LispBoolean LessThan(LispCharPtr int1, LispCharPtr int2,
                       LispHashTable& aHashTable,LispInt aPrecision);

LispStringPtr ShiftLeft( LispCharPtr int1, LispCharPtr int2, LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr ShiftRight( LispCharPtr int1, LispCharPtr int2, LispHashTable& aHashTable,LispInt aPrecision);

LispStringPtr FromBase( LispCharPtr int1, LispCharPtr int2, LispHashTable& aHashTable,
                        LispInt aPrecision);
LispStringPtr ToBase( LispCharPtr int1, LispCharPtr int2, LispHashTable& aHashTable,
                      LispInt aPrecision);

LispStringPtr FloorFloat( LispCharPtr int1, LispHashTable& aHashTable,
                        LispInt aPrecision);
LispStringPtr CeilFloat( LispCharPtr int1, LispHashTable& aHashTable,
                        LispInt aPrecision);
LispStringPtr ModFloat( LispCharPtr int1, LispCharPtr int2, LispHashTable& aHashTable,
                        LispInt aPrecision);
LispStringPtr DivFloat( LispCharPtr int1, LispCharPtr int2, LispHashTable& aHashTable,
                        LispInt aPrecision);
LispStringPtr PiFloat( LispHashTable& aHashTable, LispInt aPrecision);

LispStringPtr BitAnd(LispCharPtr int1, LispCharPtr int2,
                     LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr BitOr(LispCharPtr int1, LispCharPtr int2,
                     LispHashTable& aHashTable,LispInt aPrecision);
LispStringPtr BitXor(LispCharPtr int1, LispCharPtr int2,
                     LispHashTable& aHashTable,LispInt aPrecision);


LispStringPtr LispFactorial(LispCharPtr int1, LispHashTable& aHashTable,LispInt aPrecision);



/** Base number class. 
 */
 
// You can enable this define to have a BigNumber implementation based on 'double' and 'long'
#define noUSE_NATIVE

#ifndef USE_NATIVE
  #ifdef USE_GMP
    //...
  #else
    class ANumber;
  #endif
#endif


/// Virtual base class for low-level multiple-precision arithmetic.
/// All calculations are done at given precision. Integers grow as needed, floats don't grow beyond given precision.
class BigNumber : public YacasBase
{
public: //constructors
  BigNumber(LispCharPtr aString,LispInt aPrecision,LispInt aBase=10);
  BigNumber(const BigNumber& aOther);
  BigNumber();
  ~BigNumber();
  // assign to another number
  void SetTo(const BigNumber& aOther);
public: // Convert back to other types
  /// ToString : return string representation of number in aResult 
  void ToString(LispString& aResult, LispInt aBase) const;
  /// Give approximate representation as a double number
  double Double() const;
public: //information retrieval on library used  
  /// Numeric library name
  const LispCharPtr NumericLibraryName() const;
public://arithmetic
  /// Multiply two numbers at given precision and put result in *this
  void Multiply(const BigNumber& aX, const BigNumber& aY, LispInt aPrecision);
  /** Multiply two numbers, and add to aResult (this is useful and generally efficient to implement).
   * This is most likely going to be used by internal functions only, using aResult as an accumulator.
   */
  void MultiplyAdd(BigNumber& aResult, const BigNumber& aX, const BigNumber& aY, LispInt aPrecision);
  /// Add two numbers at given precision and return result in *this
  void Add(const BigNumber& aX, const BigNumber& aY, LispInt aPrecision);
  /// Negate the given number, return result in *this
  void Negate(const BigNumber& aX);
  /// Divide two numbers and return result in *this. Note: if the two arguments are integer, it should return an integer result!
  void Divide(const BigNumber& aX, const BigNumber& aY, LispInt aPrecision);

public:/// Bitwise operations, return result in *this.
  void ShiftLeft( const BigNumber& aX, LispInt aNrToShift);
  void ShiftRight( const BigNumber& aX, LispInt aNrToShift);
  void BitAnd(const BigNumber& aX, const BigNumber& aY);
  void BitOr(const BigNumber& aX, const BigNumber& aY);
  void BitXor(const BigNumber& aX, const BigNumber& aY);
  /// Bit count operation: return the number of significant bits if integer, return the binary exponent if float (shortcut for binary logarithm)
  LispInt BitCount() const;
  /// Give sign (-1, 0, 1)
  LispInt Sign() const;

#ifdef USE_NATIVE

private:
  enum EType
  {
    KInteger = 0,
    KDouble=1,
  };
  union UValue
  {
    long i;
    double  d;
  };
  EType type;
  UValue value;

#else 
  #ifdef USE_GMP
  //...
  #else
    ANumber* iNumber;
  #endif
#endif
};



#endif













