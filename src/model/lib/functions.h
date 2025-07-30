/*!
  \file
  \brief Header file for math function declaration and implementation
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>

namespace scn {
/*!
  \brief Interface - abstraction for math function class
*/
class Function {
 public:
  virtual ~Function() {}
  /*!
    Provides arity of the function
    \return function arity
  */
  virtual int arity() const = 0;
  /*!
    Provides information either function is left associative
    (for infix to postfix notation expression sorting)
    \return function true if the function is left associative
    and false if right associative
  */
  virtual bool left_associative() const = 0;
  /*!
    Overloaded operator() provide mean to perform the operation
    \param[in] operands in form of vector of operand values
    \return result of operation
  */
  virtual double operator()(const std::vector<double>& operands) const = 0;
};

/*!
  \brief Interface - abstraction for math function classes facade
*/
class Functions {
 public:
  virtual ~Functions() {}
  /*!
    Provides information if token is available math function
    \param[in] token expression token
    \return true if the function is available,
    false if not available
  */
  virtual bool contains(const std::string& token) const = 0;
  /*!
    Provides arity of the token function
    \param[in] token expression token
    \return function arity
  */
  virtual int arity(const std::string& token) const = 0;
  /*!
    Provides information either token is left associative
    function (for infix to postfix notation expression sorting)
    \param[in] token expression token
    \return function true if the function is left associative
    and false if right associative
  */
  virtual bool left_associative(const std::string& token) const = 0;
  /*!
    Overloaded operator() provide mean to perform the operation
    \param[in] token expression token
    \param[in] operands in form of vector of operand values
    \return result of operation
  */
  virtual double operation(const std::string& token,
                           const std::vector<double>& operands) const = 0;
};

/*!
  \brief Class - Math function classes facade

  Act as wraper around std::map<std::string, Function*>,
  where key is function name and value is pointer to function object.
  Main responsibility is to represent single point for manage
  memory allocation for function classes and memory clean up
  as std::map<T1, T2> can not to clean up memory if T2 is raw pointer.
*/
class FunctionMap : public Functions {
 public:
  FunctionMap(const std::map<std::string, Function*> func_map)
      : func_map(func_map) {}
  ~FunctionMap() {
    for (auto const& [key, val] : func_map) {
      delete val;
    }
  }
  /*!
    Provides information if token is available math function
    \param[in] token expression token
    \return true if the function is available,
    false if not available
  */
  bool contains(const std::string& token) const override {
    return func_map.contains(token);
  }
  /*!
    Provides arity of the token function
    \param[in] token expression token
    \return function arity
  */
  int arity(const std::string& token) const override {
    return func_map.at(token)->arity();
  }
  /*!
    Provides information either token is left associative
    function (for infix to postfix notation expression sorting)
    \param[in] token expression token
    \return function true if the function is left associative
    and false if right associative
  */
  bool left_associative(const std::string& token) const override {
    return func_map.at(token)->left_associative();
  }
  /*!
    Overloaded operator() provide mean to perform the operation
    \param[in] token expression token
    \param[in] operands in form of vector of operand values
    \return result of operation
  */
  double operation(const std::string& token,
                   const std::vector<double>& operands) const override {
    return func_map.at(token)->operator()(operands);
  }

 private:
  const std::map<std::string, Function*> func_map;
};

class unary_plus : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return operands[0];
  }
};

class unary_minus : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return -(operands[0]);
  }
};

class sin : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::sin(operands[0]);
  }
};

class cos : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::cos(operands[0]);
  }
};

class tan : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::tan(operands[0]);
  }
};

class asin : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::asin(operands[0]);
  }
};

class acos : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::acos(operands[0]);
  }
};

class atan : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::atan(operands[0]);
  }
};

class ln : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::log(operands[0]);
  }
};

class log : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::log10(operands[0]);
  }
};

class sqrt : public Function {
 public:
  int arity() const override { return 1; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::sqrt(operands[0]);
  }
};

class pow : public Function {
 public:
  int arity() const override { return 2; }
  bool left_associative() const override { return false; }
  double operator()(const std::vector<double>& operands) const override {
    return std::pow(operands[1], operands[0]);
  }
};

class mult : public Function {
 public:
  int arity() const override { return 2; }
  bool left_associative() const override { return true; }
  double operator()(const std::vector<double>& operands) const override {
    return operands[1] * operands[0];
  }
};

class div : public Function {
 public:
  int arity() const override { return 2; }
  bool left_associative() const override { return true; }
  double operator()(const std::vector<double>& operands) const override {
    return operands[1] / operands[0];
  }
};

class mod : public Function {
 public:
  int arity() const override { return 2; }
  bool left_associative() const override { return true; }
  double operator()(const std::vector<double>& operands) const override {
    return fmod(operands[1], operands[0]);
  }
};

class plus : public Function {
 public:
  int arity() const override { return 2; }
  bool left_associative() const override { return true; }
  double operator()(const std::vector<double>& operands) const override {
    return operands[1] + operands[0];
  }
};

class minus : public Function {
 public:
  int arity() const override { return 2; }
  bool left_associative() const override { return true; }
  double operator()(const std::vector<double>& operands) const override {
    return operands[1] - operands[0];
  }
};
}  // namespace scn

#endif  // FUNCTIONS_H
