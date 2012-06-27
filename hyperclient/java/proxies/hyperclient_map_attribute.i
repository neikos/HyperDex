// Write our own (safe) getters for attr and setters for attr, map_key and value
// in order to avoid warnings
//
%ignore hyperclient_map_attribute::attr;
%ignore hyperclient_map_attribute::map_key;
%ignore hyperclient_map_attribute::value;

// Java API users need not access this struct
//
%typemap(javaclassmodifiers) hyperclient_map_attribute "class"

%typemap(javacode) hyperclient_map_attribute
%{
  private java.math.BigInteger attrs_sz_bi = java.math.BigInteger.ZERO;

  // Returns the size of the hyperclient_map_attribute array that this instance
  // is the head of. This signed 64 bit integer will be interpreted as an
  // unsigned 64 bit integer at the c/c++ level.
  //
  long getAttrsSz()
  {
    return attrs_sz_bi.longValue();
  }

  java.math.BigInteger getAttrsSz_bi()
  {
    return attrs_sz_bi;
  }

  void setAttrsSz_bi(java.math.BigInteger value)
  {
    attrs_sz_bi = value;
  }

  String getMapAttrName()
  {
    return HyperClient.read_map_attr_name(this);
  }
%}
