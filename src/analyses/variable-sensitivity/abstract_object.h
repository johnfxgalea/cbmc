/*******************************************************************\

 Module: analyses variable-sensitivity

 Author: Thomas Kiley, thomas.kiley@diffblue.com

\*******************************************************************/
#ifndef CPROVER_ANALYSES_VARIABLE_SENSITIVITY_ABSTRACT_OBJECT_H
#define CPROVER_ANALYSES_VARIABLE_SENSITIVITY_ABSTRACT_OBJECT_H

class typet;
class constant_exprt;

#include <util/expr.h>
#include <memory>


#define CLONE \
  virtual abstract_objectt* clone() const \
  { \
    typedef std::remove_const<std::remove_reference<decltype(*this)>::type \
      >::type current_typet; \
    return new current_typet(*this); \
  } \

#define MERGE(parent_typet) \
  virtual abstract_object_pointert merge( \
    const abstract_object_pointert op, \
    bool &out_any_modifications) \
  {\
    assert(this->type==op->type); \
    typedef std::remove_const<std::remove_reference<decltype(*this)>::type \
      >::type current_typet; \
    \
    /*Check the supplied parent type is indeed a parent*/ \
    static_assert(std::is_base_of<parent_typet, current_typet>::value, \
      "parent_typet in MERGE must be parent class of the current type"); \
    \
    typedef sharing_ptrt<current_typet> current_type_ptrt; \
    /*Cast the supplied type to the current type to facilitate double dispatch*/ \
    current_type_ptrt n=std::dynamic_pointer_cast<current_typet>(op); \
    current_type_ptrt m=current_type_ptrt(new current_typet(*this)); \
    if (n!= NULL) \
    { \
      m->merge_state(current_type_ptrt(new current_typet(*this)), n); \
      out_any_modifications=!(*this==*m); \
      return m; \
    } \
    else \
    { \
      return parent_typet::merge( \
        abstract_object_pointert(op), out_any_modifications); \
    } \
  } \

template<class T>
using sharing_ptrt=std::shared_ptr<T>;

typedef sharing_ptrt<class abstract_objectt> abstract_object_pointert;

class abstract_objectt
{
public:
  abstract_objectt(const typet &type);
  abstract_objectt(const typet &type, bool top, bool bottom);
  abstract_objectt(const abstract_objectt &old);
  abstract_objectt(const constant_exprt &expr);

  const typet &get_type() const;
  virtual bool is_top() const;
  virtual bool is_bottom() const;

  // Sets the state of this object
  void merge_state(
    const abstract_object_pointert op1, const abstract_object_pointert op2);

  // This is both the interface and the base case of the recursion
  // It uses merge state to
  virtual abstract_object_pointert merge(
    const abstract_object_pointert op, bool &out_any_modifications);

  virtual exprt to_constant() const;

  virtual void output(
    std::ostream &out, const class ai_baset &ai, const class namespacet &ns);

  CLONE

  virtual bool operator==(const abstract_objectt &other) const;

  //protected
  typet type;
  bool top;
  bool bottom;
};

#endif // CPROVER_ANALYSES_VARIABLE_SENSITIVITY_ABSTRACT_OBJECT_H