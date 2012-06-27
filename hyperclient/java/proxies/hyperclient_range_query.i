// Write our own (safe) setter for attr
// in order to avoid warnings
//
%ignore hyperclient_range_query::attr;

// Java API users need not access this struct
//
%typemap(javaclassmodifiers) hyperclient_range_query "class"

%typemap(javacode) hyperclient_range_query
%{
  String getRangeQueryAttrName()
  {
    return HyperClient.read_range_query_attr_name(this);
  }
%}
