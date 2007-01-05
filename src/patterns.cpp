/*TODO
 - some bugs left:
   - - - a not working correctly...
 - make a RetractRule with a predicate as argument.
 */
#include "yacasprivate.h"
#include "yacasbase.h"
#include "patterns.h"
#include "standard.h"
#include "mathuserfunc.h"
#include "lispobject.h"
#include "lispeval.h"
#include "standard.h"

#define MATCH_NUMBERS


//#define YACAS_LOGGING
#include "log.h"
/*
*/

#define InternalEval aEnvironment.iEvaluator->Eval


YacasParamMatcherBase::~YacasParamMatcherBase()
{
}


MatchAtom::MatchAtom(LispString * aString) : iString(aString)
{
}

LispBoolean MatchAtom::ArgumentMatches(LispEnvironment& aEnvironment,
                                       LispPtr& aExpression,
                                       LispPtr* arguments)
{
    /*
     LogPrintf("Enter match atom\n");
    if (aExpression->String())
        LogPrintf("Atom match %s to %s\n",
                  iString->String(),aExpression->String()->c_str());
    else
    {
        LogPrintf("trying to match atom to list\n");
    }
    */

    // If it is a floating point, don't even bother comparing
    if (!!aExpression)
      if (aExpression->Number(0))
        if (!aExpression->Number(0)->IsInt())
          return LispFalse;
      
    return (iString == aExpression->String());
}







MatchNumber::MatchNumber(BigNumber* aNumber) : iNumber(aNumber)
{
}

LispBoolean MatchNumber::ArgumentMatches(LispEnvironment& aEnvironment,
                                       LispPtr& aExpression,
                                       LispPtr* arguments)
{
  if (aExpression->Number(aEnvironment.Precision()))
    return iNumber->Equals(*aExpression->Number(aEnvironment.Precision()));
  return LispFalse;
}




MatchVariable::MatchVariable(LispInt aVarIndex) : iVarIndex(aVarIndex)
{
}
LispBoolean MatchVariable::ArgumentMatches(LispEnvironment& aEnvironment,
                                       LispPtr& aExpression,
                                       LispPtr* arguments)
{
    if (!arguments[iVarIndex])
    {
        arguments[iVarIndex] = (aExpression);
//        LogPrintf("Set var %d\n",iVarIndex);
        return LispTrue;
    }
    else
    {
        if (InternalEquals(aEnvironment, aExpression, arguments[iVarIndex]))
        {
//            LogPrintf("Matched var %d\n",iVarIndex);
            return LispTrue;
        }
        return LispFalse;
    }
    return LispFalse;
}

MatchSubList::MatchSubList(YacasParamMatcherBase** aMatchers,
                           LispInt aNrMatchers)
: iMatchers(aMatchers),iNrMatchers(aNrMatchers)

{
}

MatchSubList::~MatchSubList()
{
    if (iMatchers)
    {
        LispInt i;
        for (i=0;i<iNrMatchers;i++)
            delete iMatchers[i];
        PlatFree(iMatchers);
    }
}

LispBoolean MatchSubList::ArgumentMatches(LispEnvironment& aEnvironment,
                                          LispPtr& aExpression,
                                          LispPtr* arguments)
{
    if (!aExpression->SubList())
        return LispFalse;

    LispIterator iter(aExpression);
#if 0
	iter.GoSub();
#else
	LispObject * pObj = iter.getObj();
    Check(pObj,KLispErrInvalidArg);
    LispPtr * pPtr = pObj->SubList();
    Check(pPtr,KLispErrNotList);
	iter = *pPtr;
#endif

    for (LispInt i=0;i<iNrMatchers;i++,++iter)
	{
		// TODO: woof -- fix this ugliness
        if (!iter.getObj())
            return LispFalse;
        if (!iMatchers[i]->ArgumentMatches(aEnvironment,*iter,arguments))
            return LispFalse;
	}
    if (iter.getObj())
        return LispFalse;
    return LispTrue;
}

LispInt YacasPatternPredicateBase::LookUp(LispString * aVariable)
{
    LispInt i;
    for (i=0;i<iVariables.Size();i++)
    {
        if (iVariables[i] == aVariable)
        {
            return i;
        }
    }
    iVariables.Append(aVariable);
    return iVariables.Size()-1;
}


YacasParamMatcherBase* YacasPatternPredicateBase::MakeParamMatcher(LispEnvironment& aEnvironment, LispObject* aPattern)
{
    if (!aPattern)
        return NULL;
#ifdef MATCH_NUMBERS
    if (aPattern->Number(aEnvironment.Precision()))
    {
        return NEW MatchNumber(aPattern->Number(aEnvironment.Precision()));
    }
#endif
    // Deal with atoms
    if (aPattern->String())
    {
        return NEW MatchAtom(aPattern->String());
    }

    // Else it must be a sublist
    if (aPattern->SubList())
    {
        // See if it is a variable template:
        LispPtr* sublist = aPattern->SubList();
        LISPASSERT(sublist);

        LispInt num = InternalListLength(*sublist);

        // variable matcher here...
        if (num>1)
        {
            LispObject* head = (*sublist);
            if (head->String() == aEnvironment.HashTable().LookUp("_"))
            {
                LispObject* second = head->Nixed();
                if (second->String())
                {
                    LispInt index = LookUp(second->String());

                    // Make a predicate for the type, if needed
                    if (num>2)
                    {
                        LispPtr third;

                        LispObject* predicate = second->Nixed();
                        if (predicate->SubList())
                        {
                            InternalFlatCopy(third, *predicate->SubList());
                        }
                        else
                        {
                            third = (second->Nixed()->Copy());
                        }

						// TODO: woof
                        //LispChar * str = second->String()->c_str();
                        LispObject* last = third;
                        while (!!last->Nixed())
                            last = last->Nixed();

                        last->Nixed() = (LispAtom::New(aEnvironment,second->String()->c_str()));

//                        third->Nixed() = (LispAtom::New(aEnvironment,aEnvironment.HashTable().LookUp(str)));
                        LispPtr pred(LispSubList::New(third));
#define LP pred

                        DBG_( third->Nixed()->SetFileAndLine(second->iFileName,second->iLine); )
                        DBG_( LP->SetFileAndLine(head->iFileName,head->iLine); )
//LispPtr hold(LP);
//aEnvironment.CurrentPrinter().Print(LP,
//                                    *aEnvironment.CurrentOutput());
						#undef LP
#if 0	// for testing
						LispPtr src;
						LispPtr dst;
						ArrOpsCustom<LispPtr> opers;
						opers.construct(&dst, &src);
						const LispPtr aVal(src);
						//opers.construct(&dst, &aVal);
#endif
						iPredicates.Append(pred);
                    }
                    return NEW MatchVariable(index);
                }
            }
        }

        YacasParamMatcherBase** matchers = PlatAllocN<YacasParamMatcherBase*>(num);
        LispIterator iter(*sublist);
        for (LispInt i=0;i<num;i++,++iter)
        {
            matchers[i] = MakeParamMatcher(aEnvironment,iter.getObj());
            LISPASSERT(matchers[i]);
        }
		return NEW MatchSubList(matchers, num);
    }
    
    return NULL;
}

YacasPatternPredicateBase::YacasPatternPredicateBase(LispEnvironment& aEnvironment, LispPtr& aPattern,
                                                     LispPtr& aPostPredicate)
{
    LispIterator iter(aPattern);
    for ( ; iter.getObj(); ++iter)
	{
        YacasParamMatcherBase* matcher = MakeParamMatcher(aEnvironment,iter.getObj());
        LISPASSERT(matcher!=NULL);
        iParamMatchers.Append(matcher);
	}
	LispPtr post(aPostPredicate);
  iPredicates.Append(post);
}

LispBoolean YacasPatternPredicateBase::Matches(LispEnvironment& aEnvironment,
                                              LispPtr& aArguments)
{
    LispPtr* arguments = NULL;
    if (iVariables.Size() > 0)
		arguments = NEW LispPtr[iVariables.Size()];
    LocalArgs args(arguments); //Deal with destruction
    LispIterator iter(aArguments);
    for (LispInt i=0;i<iParamMatchers.Size();i++,++iter)
	{
        if (!iter.getObj())
            return LispFalse;
        if (!iParamMatchers[i]->ArgumentMatches(aEnvironment,*iter,arguments))
        {
            return LispFalse;
        }
	}
    if (iter.getObj())
        return LispFalse;

    {
        // set the local variables.
        LispLocalFrame frame(aEnvironment,LispFalse);

        SetPatternVariables(aEnvironment,arguments);

        // do the predicates
        if (!CheckPredicates(aEnvironment))
            return LispFalse;
    }

    // set the local variables for sure now
    SetPatternVariables(aEnvironment,arguments);
    
    return LispTrue;
}




LispBoolean YacasPatternPredicateBase::Matches(LispEnvironment& aEnvironment,
                                              LispPtr* aArguments)
{
    LispInt i;

    LispPtr* arguments = NULL;
    if (iVariables.Size() > 0)
        arguments = NEW LispPtr[iVariables.Size()];
    LocalArgs args(arguments); //Deal with destruction

    for (i=0;i<iParamMatchers.Size();i++)
    {
        if (!iParamMatchers[i]->ArgumentMatches(aEnvironment,aArguments[i],arguments))
        {
            return LispFalse;
        }
    }

    {
        // set the local variables.
        LispLocalFrame frame(aEnvironment,LispFalse);
        SetPatternVariables(aEnvironment,arguments);

        // do the predicates
        if (!CheckPredicates(aEnvironment))
            return LispFalse;
    }

    // set the local variables for sure now
    SetPatternVariables(aEnvironment,arguments);
    return LispTrue;
}



LispBoolean YacasPatternPredicateBase::CheckPredicates(LispEnvironment& aEnvironment)
{
  for (LispInt i=0;i<iPredicates.Size();i++)
  {
    LispPtr pred;
    InternalEval(aEnvironment, pred, iPredicates[i]);
    if (IsFalse(aEnvironment, pred))
    {
      return LispFalse;
    }
    Check(IsTrue(aEnvironment, pred), KLispErrNonBooleanPredicateInPattern);
  }
  //hier
  return LispTrue;
}
                                                

void YacasPatternPredicateBase::SetPatternVariables(LispEnvironment& aEnvironment,
                                                    LispPtr* arguments)
{
    LispInt i;
    for (i=0;i<iVariables.Size();i++)
    {
        // set the variable to the new value
        aEnvironment.NewLocal(iVariables[i],arguments[i]);
    }
}


YacasPatternPredicateBase::~YacasPatternPredicateBase()
{
}

