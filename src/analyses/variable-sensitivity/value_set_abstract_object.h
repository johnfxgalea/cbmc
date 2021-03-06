/*******************************************************************\

 Module: analyses variable-sensitivity

 Author: diffblue

\*******************************************************************/

/// \file
/// Value Set Abstract Object

#ifndef CPROVER_ANALYSES_VARIABLE_SENSITIVITY_VALUE_SET_ABSTRACT_OBJECT_H
#define CPROVER_ANALYSES_VARIABLE_SENSITIVITY_VALUE_SET_ABSTRACT_OBJECT_H

#include <analyses/variable-sensitivity/abstract_object_set.h>
#include <analyses/variable-sensitivity/abstract_value_object.h>

class value_set_abstract_objectt : public abstract_value_objectt,
                                   public value_set_tag
{
public:
  /// \copydoc abstract_objectt::abstract_objectt(const typet&)
  explicit value_set_abstract_objectt(const typet &type);

  /// \copydoc abstract_objectt::abstract_objectt(const typet &, bool, bool)
  value_set_abstract_objectt(const typet &type, bool top, bool bottom);

  value_set_abstract_objectt(
    const exprt &expr,
    const abstract_environmentt &environment,
    const namespacet &ns);

  index_range_implementation_ptrt
  index_range_implementation(const namespacet &ns) const override;

  value_range_implementation_ptrt value_range_implementation() const override;

  /// \copydoc abstract_objectt::to_constant
  exprt to_constant() const override;
  constant_interval_exprt to_interval() const override;

  /// Getter for the set of stored abstract objects.
  /// \return the values represented by this abstract object
  const abstract_object_sett &get_values() const override
  {
    return values;
  }

  /// Setter for updating the stored values
  /// \param other_values: the new (non-empty) set of values
  void set_values(const abstract_object_sett &other_values);

  /// The threshold size for value-sets: past this threshold the object is
  /// either converted to interval or marked as `top`.
  static const size_t max_value_set_size = 10;

  /// \copydoc abstract_objectt::write
  ///
  /// Delegate writing to stored values.
  abstract_object_pointert write(
    abstract_environmentt &environment,
    const namespacet &ns,
    const std::stack<exprt> &stack,
    const exprt &specifier,
    const abstract_object_pointert &value,
    bool merging_write) const override;

  void output(std::ostream &out, const ai_baset &ai, const namespacet &ns)
    const override;

protected:
  CLONE

  /// \copydoc abstract_object::merge
  abstract_object_pointert merge(abstract_object_pointert other) const override;

private:
  /// Update the set of stored values to \p new_values. Build a new abstract
  ///   object of the right type if necessary.
  /// \param new_values: potentially new set of values
  /// \param environment: the abstract environment
  /// \return the abstract object representing \p new_values (either 'this' or
  ///   something new)
  abstract_object_pointert resolve_new_values(
    const abstract_object_sett &new_values,
    const abstract_environmentt &environment) const;

  /// Update the set of stored values to \p new_values. Build a new abstract
  ///   object of the right type if necessary.
  /// \param new_values: potentially new set of values
  /// \return the abstract object representing \p new_values (either 'this' or
  ///   something new)
  abstract_object_pointert
  resolve_values(const abstract_object_sett &new_values) const;

  // data
  abstract_object_sett values;

  void set_top_internal() override;
};

#endif // CPROVER_ANALYSES_VARIABLE_SENSITIVITY_VALUE_SET_ABSTRACT_OBJECT_H
