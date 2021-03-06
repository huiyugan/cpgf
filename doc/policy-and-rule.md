<!--notoc-->

# Policies and Rules -- customize the underlying behavior at compile time

## Get start

Policy is a mechanism to change the default behavior of the library.

Policy is composed by rules.  
One rule is just a data type (a struct).  
Rules are pre-defined and recognized by the library.  
One rule will change one aspect of the default behavior.

For example, by default a meta class requires the class has a default constructor.  
If the class doesn't have it, compiling will fail if meta class is being built for it.  
However, if feeding the meta class with a policy with the rule of GMetaRuleDefaultConstructorAbsent, the library will know there is no default constructor and will succeed the compiling.

Rules are pre-defined, you can't add any new rules or change any existing rules behavior.  
However, with the combination of rules, a new policy can be created.

To create a policy, include the header gpolicy.h, use MakePolicy to make a policy.
```c++
typedef MakePolicy<Rule1, Rule2, ..., RuleN> NewPolicy;
```

For the reflection library, there are several predefined policies in file gmetapolicy.h.

## Meta rules

**GMetaRuleForbidRead**  
Apply to: field and property.  
Effect: the value of field and property can't be read directly.  
Invoking "Get" will cause exception.

**GMetaRuleForbidWrite**  
Apply to: field and property.  
Effect: the value of field and property can't be written directly.  
Invoking "set" will cause exception.

**GMetaRuleDefaultConstructorAbsent**  
Apply to: class  
Effect: the class doesn't have default constructor.

**GMetaRuleCopyConstructorAbsent**  
Apply to: class  
Effect: the class doesn't have copy constructor.

**GMetaRuleTransferOwnership <N>**  
Apply to: invokable -- method, constructor, and operator  
Effect: the parameter or result will discard the object ownership.  
This rule is a template with an integer parameter N.  
If N is metaPolicyResultIndex (which value is -1), the rule is for result. The object returned by the invokable should be freed by the caller.  
If N is 0 or greater integer, the rule is for parameter N. The object passed to the invokable at parameter N will be freed by the callee (the invokable).  
Note: parameter 0 is the first explicit parameter, not "this" pointer.

**GMetaRuleCopyConstReference <N>**  
Apply to: invokable -- method, constructor, and operator  
Effect: the parameter or result will copy the object to reference-to-const instead of reference.  
If implicit type conversion is required, such like C string to std::string, use this rule.  
See Rule 5 in GVariant document for details.

**GMetaRuleParamNoncopyable <N>**  
Apply to: invokable -- method, constructor, and operator  
Effect: the parameter or result is not copyable.

**GMetaRuleExplicitThis**  
Apply to: method, operator and property which getter or setter is method  
Effect: the first parameter of the method is treated as "this" pointer and it's invisible from the outside.  
This policy is used to use a global method to pretend being a member function.  
For example, if we have global function,  
```c++
void myMethod(MyClass * instance, int param);
```
Applying GMetaRuleExplicitThis will have the same effect as if myMethod is a member function of MyClass and the prototype is void myMethod(int param).  
With this policy, global function can be used as property getter or setter.

**GMetaRuleGetterExplicitThis** and **GMetaRuleSetterExplicitThis**  
Apply to: property getter and setter.  
Effect: same as GMetaRuleExplicitThis, but GMetaRuleGetterExplicitThis only has effect on property getter, and GMetaRuleSetterExplicitThis only has effect on property setter.  
With these two rules, we can control the explict this on getter and setter separately.

## Meta policies

Though you can combine the rules to create the policy you need, there are some pre-defined meta policies in gmetapolicy.h  
Below only lists the source code of all of the pre-defined policies.  
The meaning can be found by the policy name and the rules.
```c++
typedef MakePolicy<
    	GMetaRuleForbidRead,
    	GMetaRuleForbidWrite
    >
    GMetaPolicyNoncopyable;


typedef MakePolicy<
    	GMetaRuleForbidWrite
    >
    GMetaPolicyReadOnly;


typedef MakePolicy<
    	GMetaRuleForbidRead
    >
    GMetaPolicyWriteOnly;


typedef MakePolicy<
    	GMetaRuleDefaultConstructorAbsent
    >
    GMetaPolicyNoDefaultConstructor;


typedef MakePolicy<
    	GMetaRuleCopyConstructorAbsent
    >
    GMetaPolicyNoCopyConstructor;


typedef MakePolicy<
    	GMetaRuleDefaultConstructorAbsent,
    	GMetaRuleCopyConstructorAbsent
    >
    GMetaPolicyNoDefaultAndCopyConstructor;


typedef MakePolicy<
    	GMetaRuleTransferOwnership <metaPolicyResultIndex>
    >
    GMetaPolicyTransferResultOwnership;


typedef MakePolicy<
    	GMetaRuleCopyConstReference <metaPolicyResultIndex>,
    	GPP_REPEAT(REF_MAX_ARITY, POLICY_ALL_INDEX_HELPER, GMetaRuleCopyConstReference)
    >
    GMetaPolicyCopyAllConstReference;


typedef MakePolicy<
    	GMetaRuleParamNoncopyable <metaPolicyResultIndex>,
    	GPP_REPEAT(REF_MAX_ARITY, POLICY_ALL_INDEX_HELPER, GMetaRuleParamNoncopyable)
    >
    GMetaPolicyAllParamNoncopyable;

typedef MakePolicy<
    	GMetaRuleExplicitThis
    >
    GMetaPolicyExplicitThis;
```
