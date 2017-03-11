#include "test_variant_common.h"

#include "gvariant.h"
#include "cpgf/gexception.h"
#include "cpgf/gmetaapi.h"

#include <type_traits>
#include <iostream>

using namespace std;
using namespace xcpgf;

namespace Test_Variant_Misc { namespace {

class CLASS
{
public:
	CLASS() {}
	CLASS(const CLASS &) {}
	CLASS(const string &) {}

	CLASS(int) {}
};

class ClassB
{
public:
	static ClassB * lastB;

public:
	ClassB() { lastB = this; }

	ClassB(const ClassB &) {
	}

	ClassB & operator = (const ClassB &) {
		return *this;
	}
};
ClassB * ClassB::lastB = NULL;

struct XXX {
//private:
	XXX(const XXX &);
	XXX & operator = (const XXX &);
};


GTEST(TestVariant_ConstReference)
{
const char * s = "abc";
GVariant v = s;
cout << fromVariant<const string &, VarantCastCopyConstRef>(v) << endl;

	const ClassB & a = fromVariant<const ClassB &, VarantCastCopyConstRef>(createVariant<false>(ClassB()));
	GDIFF(&a, ClassB::lastB);

	const ClassB & b = fromVariant<const ClassB &, VarantCastKeepConstRef>(createVariant<false>(ClassB()));
	GEQUAL(&b, ClassB::lastB);
}

/*
GTEST(TestVariant_ObjectPointer)
{
	GEQUAL(vtGetBaseType(GVariant((CLASS *)0).refData().typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS *)0).refData().typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((CLASS const *)0).refData().typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS const *)0).refData().typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((CLASS volatile *)0).refData().typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS volatile *)0).refData().typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((CLASS const volatile *)0).refData().typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS const volatile *)0).refData().typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((CLASS **)0).refData().typeData), vtObject);
	GEQUAL(vtGetPointers(GVariant((CLASS **)0).refData().typeData), 2);

	GEQUAL(vtGetBaseType(GVariant((IMetaClass *)0).refData().typeData), vtInterface);
	GEQUAL(vtGetPointers(GVariant((IMetaClass *)0).refData().typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((const IMetaClass *)0).refData().typeData), vtInterface);
	GEQUAL(vtGetPointers(GVariant((const IMetaClass *)0).refData().typeData), 1);

	GEQUAL(vtGetBaseType(GVariant((const volatile IMetaClass *)0).refData().typeData), vtInterface);
	GEQUAL(vtGetPointers(GVariant((const volatile IMetaClass *)0).refData().typeData), 1);
}
*/


} }
