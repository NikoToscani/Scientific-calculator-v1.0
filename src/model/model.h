/*!
  \file
  \brief Header file for Model library declaration
*/
#ifndef MODEL_H
#define MODEL_H

#include <cfloat>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "lib/functions.h"

/*!
  \def Safe char* buffer size for arbitrary double conversion
  to char* in hexadecimal format (with "a" specifier) to save
  exact double representation
*/
#define BUF_SIZE 64

/*!
  \def Initialization list of function tokens for
  class LexemeExpression parsing
*/
#define FUNCTION_MAP                   \
  {                                    \
      {"unary +", new unary_plus()},   \
      {"unary -", new unary_minus()},  \
      {"sin", new sin()},              \
      {"cos", new cos()},              \
      {"tan", new tan()},              \
      {"asin", new asin()},            \
      {"acos", new acos()},            \
      {"atan", new atan()},            \
      {"ln", new ln()},                \
      {"log", new log()},              \
      {"sqrt", new sqrt()},            \
      {"^", new pow()},                \
      {"*", new mult()},               \
      {"/", new div()},                \
      {"mod", new mod()},              \
      {"+", new plus()},               \
      {"-", new minus()},              \
  }

/*!
  \def Initialization list of operator precedence for
  class ShuntingYardStringStack sorting to RPN
*/
#define PRECEDENCE_MAP     \
  {                        \
      {"unary +", 3},      \
      {"unary -", 3},      \
      {"sin", 3},          \
      {"cos", 3},          \
      {"tan", 3},          \
      {"asin", 3},         \
      {"acos", 3},         \
      {"atan", 3},         \
      {"ln", 3},           \
      {"log", 3},          \
      {"sqrt", 3},         \
      {"^", 2},            \
      {"*", 2},            \
      {"/", 2},            \
      {"mod", 2},          \
      {"+", 1},            \
      {"-", 1},            \
  }

namespace scn {
/*!
  \brief Interface - abstraction of operator stack used in Shunting Yard
  alghorithm
*/
class ShuntingYardAlgorithmStack {
 public:
  virtual ~ShuntingYardAlgorithmStack() {}  // LCOV_EXCL_LINE

  /*!
    Adds operator token to the stack
    \param[in] token input token
  */
  virtual void push(const std::string& token) const = 0;

  /*!
    Pop_back number of tokens equal to input vector of tokens
    \param[in] tokens input vector of tokens
  */
  virtual void pop_multiple(std::vector<std::string> tokens) const = 0;

  /*!
    If at the back of stack has left parenthesis, then
    vector of tokens with one token - left parenthesis -
    will be returned, else empty vector is returned
    \return vector of one token - left parenthesis
  */
  virtual std::vector<std::string> left_bracket() const = 0;

  /*!
    If at the back of stack has unary operators, then
    vector of tokens consisted of this tokens will be
    returned taken from the back (LIFO) and pushed to
    returning vector one by one, else empty vector is returned
    \return vector of unary operator tokens
  */
  virtual std::vector<std::string> unary_operators() const = 0;

  /*!
    If at the back of stack has tokens that are not unary or
    left parenthesis, then such tokens will be taken
    from the back (LIFO) and pushed to returning vector
    one by one, else empty vector is returned.
    \return vector of non-unary and non-left-parenthesis operator tokens
  */
  virtual std::vector<std::string> not_unary_operators() const = 0;

  /*!
    Compares input token operator with tokens at the back of the stack
    one by one and, if stack token is operator, has higher precedence
    or has same precedence and input token is left associative,
    then such tokens will be taken from the back (LIFO) and pushed
    to returning vector one by one, else empty vector is returned.
    \param[in] token input token
    \return vector of unary operator tokens
  */
  virtual std::vector<std::string> hi_preced_operators(
      const std::string& token) const = 0;

  /*!
    Check if the stack is empty
    \return true if stack is empty, false if not empty
  */
  virtual bool empty() const = 0;

  /*!
    Clears the stack
  */
  virtual void clear() const = 0;
};

/*!
  \brief Class - Shunting Yard alghorithm operator stack

  Allows DI of stack memory, ptr to map of supported functions for
  properties (arity, associativity) and ptr to map of function
  precedence for converting infix notation into reverse polish notation.
*/
class ShuntingYardStringStack : public ShuntingYardAlgorithmStack {
 public:
  ShuntingYardStringStack()
      : stack(new std::vector<std::string>),
        functions(FUNCTION_MAP),
        func_precedence(PRECEDENCE_MAP) {}
  ~ShuntingYardStringStack();
  void push(const std::string& token) const override;
  void pop_multiple(std::vector<std::string> tokens) const override;
  std::vector<std::string> left_bracket() const override;
  std::vector<std::string> unary_operators() const override;
  std::vector<std::string> not_unary_operators() const override;
  std::vector<std::string> hi_preced_operators(
      const std::string& token) const override;
  bool empty() const override;
  void clear() const override;

 private:
  std::vector<std::string>* const stack;
  const std::map<std::string, Function*> functions;
  const std::map<std::string, int> func_precedence;
};
/*!
  \brief Interface - abstraction for postfixable expressions

  Provides methods for editing, clearing, stringifying, tokenizing,
  and converting expressions to postfix notation.
*/
class PostfixableExpression {
 public:
  virtual ~PostfixableExpression() {}  // LCOV_EXCL_LINE

  /*!
    Edits the expression using the input button.
    \param[in] button input button as a string
  */
  virtual void edit(const std::string& button) const = 0;

  /*!
    Clears the expression.
  */
  virtual void clear() const = 0;

  /*!
    Returns the expression as a string.
    \return string representation of the expression
  */
  virtual std::string string() const = 0;

  /*!
    Tokenizes the expression.
    \return vector of tokens
  */
  virtual std::vector<std::string> tokenized() const = 0;

  /*!
    Converts the tokenized expression to postfix notation.
    \return vector of tokens in postfix notation
  */
  virtual std::vector<std::string> postfixed() const = 0;
};

/*!
  \brief Class - Concrete implementation of PostfixableExpression

  Stores and transforms string-based expressions using a
  Shunting Yard algorithm stack.
*/
class PostfixStringExpression : public PostfixableExpression {
 public:
  /*!
    Constructor
    \param[in] stack pointer to a ShuntingYardAlgorithmStack object
  */
  PostfixStringExpression(const ShuntingYardAlgorithmStack* const stack)
      : expression(new std::vector<std::string>),
        functions(FUNCTION_MAP),
        stack(stack) {}
  ~PostfixStringExpression();
  void edit(const std::string& button) const override;
  void clear() const override;
  std::string string() const override;
  std::vector<std::string> tokenized() const override;
  std::vector<std::string> postfixed() const override;

 private:
  void append_tokens(std::vector<std::string> src,
                     std::vector<std::string>* dest) const;
  std::vector<std::string>* const expression;
  const std::map<std::string, Function*> functions;
  const ShuntingYardAlgorithmStack* const stack;
};

/*!
  \brief Interface - abstraction of a stack that calculates numeric results
*/
class CalculatingStack {
 public:
  virtual ~CalculatingStack() {}  // LCOV_EXCL_LINE

  /*!
    Pushes a token (e.g., number or operator) onto the stack.
    \param[in] token input token
  */
  virtual void push(const std::string& token) const = 0;

  /*!
    Returns the top value of the stack.
    \return top value as double
  */
  virtual double top() const = 0;

  /*!
    Clears the stack.
  */
  virtual void clear() const = 0;
};

/*!
  \brief Class - Implementation of CalculatingStack for double values

  Evaluates postfix expressions using a vector-based stack.
*/
class CalculatingDblStack : public CalculatingStack {
 public:
  CalculatingDblStack()
      : stack(new std::vector<double>), functions(FUNCTION_MAP) {}
  ~CalculatingDblStack();
  void push(const std::string& token) const override;
  double top() const override;
  void clear() const override;

 private:
  double strToDbl(const std::string& str) const;
  std::vector<double>* const stack;
  const std::map<std::string, Function*> functions;
};

/*!
  \brief Interface - abstraction for computable expressions

  Allows editing, clearing, viewing, and solving expressions.
*/
class ComputableExpression {
 public:
  virtual ~ComputableExpression() {}  // LCOV_EXCL_LINE

  /*!
    Edits the expression using the input button.
    \param[in] button input button as a string
  */
  virtual void edit(const std::string& button) const = 0;

  /*!
    Clears the expression.
  */
  virtual void clear() const = 0;

  /*!
    Returns the expression as a string.
    \return string representation of the expression
  */
  virtual std::string string() const = 0;

  /*!
    Computes and returns the result of the expression.
    \return numeric solution
  */
  virtual double solution() const = 0;
};

/*!
  \brief Class - String-based computable expression

  Computes result using a postfixable expression and calculating stack.
*/
class ComputableStringExpression : public ComputableExpression {
 public:
  /*!
    Constructor
    \param[in] expression pointer to a PostfixableExpression
    \param[in] calc_stack pointer to a CalculatingStack
  */
  ComputableStringExpression(const PostfixableExpression* const expression,
                             const CalculatingStack* const calc_stack)
      : expression(expression), calc_stack(calc_stack) {}
  void edit(const std::string& button) const override;
  void clear() const override;
  std::string string() const override;
  double solution() const override;

 private:
  const PostfixableExpression* const expression;
  const CalculatingStack* const calc_stack;
};

/*!
  \brief Class - Decorator for CalculatingStack supporting variable substitution

  Substitutes a variable value (e.g., "X") during evaluation.
*/
class CalculatingStack_with_variable : public CalculatingStack {
 public:
  CalculatingStack_with_variable(const CalculatingStack* const stack,
                                 std::string* const str_var)
      : stack(stack), X_str_var(str_var) {}
  void push(const std::string& token) const override;
  double top() const override;
  void clear() const override;

 private:
  const CalculatingStack* const stack;
  std::string* const X_str_var;
};

/*!
  \brief Interface - computable expression with a variable

  Extends ComputableExpression to allow editing a variable's value.
*/
class ComputExpressionWithVariable : public ComputableExpression {
 public:
  virtual ~ComputExpressionWithVariable() {}  // LCOV_EXCL_LINE

  /*!
    Updates the internal variable value.
    \param[in] var_value value as a string
  */
  virtual void edit_variable(const std::string& var_value) const = 0;
};

/*!
  \brief Class - Computes result of expression with a variable

  Wraps another computable expression and allows updating variable value.
*/
class ComputStrExpressionWithVariable : public ComputExpressionWithVariable {
 public:
  /*!
    Constructor
    \param[in] comp_expression base expression
    \param[in] str_var pointer to variable string
  */
  ComputStrExpressionWithVariable(
      const ComputableExpression* const comp_expression,
      std::string* const str_var)
      : comp_expression(comp_expression), X_str_var(str_var) {}
  void edit(const std::string& button) const override;
  void clear() const override;
  std::string string() const override;
  double solution() const override;
  void edit_variable(const std::string& var_value) const override;

 private:
  const ComputableExpression* const comp_expression;
  std::string* const X_str_var;
};

/*!
  \brief Interface - abstraction for plotting graphs of expressions
*/
class Plotable {
 public:
  virtual ~Plotable() {}  // LCOV_EXCL_LINE

  /*!
    Generates graphs over a defined x/y region and pixel space.
    \return vector of graph maps (x->y points)
  */
  virtual std::vector<std::map<double, double>> graphs(double x_lo, double x_hi,
                                                       int x_pix, double y_lo,
                                                       double y_hi,
                                                       int y_pix) const = 0;
};

/*!
  \brief Class - Implementation of Plotable for expressions with variables

  Generates 2D graphs of expressions over defined regions.
*/
class PlotableExpression : public Plotable {
 public:
  /*!
    Constructor
    \param[in] expression_with_var pointer to expression with variable
  */
  PlotableExpression(
      const ComputExpressionWithVariable* const expression_with_var)
      : expression_with_var(expression_with_var) {}
  std::vector<std::map<double, double>> graphs(double x_lo, double x_hi,
                                               int x_pix, double y_lo,
                                               double y_hi,
                                               int y_pix) const override;

 private:
  std::map<double, double> recursive_plot(double x_min, double x_max,
                                          double delta_y, double y_min,
                                          double y_max, double y_lo,
                                          double y_hi) const;
  std::vector<std::map<double, double>> cut_subgraphs(
      std::map<double, double>& source_graph, double y_lo, double y_hi) const;
  std::string precise_dblToStr(double num) const;
  const ComputExpressionWithVariable* const expression_with_var;
};

/*!
  \brief Interface - abstraction of user interaction model

  Handles expression changes, computation, and graph plotting.
*/
class Model {
 public:
  virtual ~Model() {}  // LCOV_EXCL_LINE

  /*!
    Handles most button presses (except Plot), updates expression and result.
    Catches and stores error messages on failure.
    \param[in] button button input string
  */
  virtual void modify(const std::string& button) const = 0;

  /*!
    Returns the current expression string.
    \return string representation of expression
  */
  virtual std::string expression() const = 0;

  /*!
    Returns the current computed result or error message.
    \return string value of solution
  */
  virtual std::string some_result() const = 0;

  /*!
    cause assignig string value to variable X in expression
  */
  virtual void edit_variable(const std::string& var_value) const = 0;

  /*!
    Handles Plot and AC button presses.
    \return vector of graph data (can be empty on error)
  */
  virtual std::vector<std::map<double, double>> graphs(double x_lo, double x_hi,
                                                       int x_pix, double y_lo,
                                                       double y_hi,
                                                       int y_pix) const = 0;
};

/*!
  \brief Class - Concrete implementation of Model

  Handles user input, expression manipulation, solving,
  and plotting of graphs.
*/
class CalculatorModel : public Model {
 public:
  /*!
    Constructor
    \param[in] comp_expression_x pointer to expression with variable
    \param[in] graph_plot_expression pointer to plotable expression
  */
  CalculatorModel(const ComputExpressionWithVariable* const comp_expression_x,
                  const Plotable* const graph_plot_expression)
      : result(new std::string),
        comp_expression_x(comp_expression_x),
        graph_plot_expression(graph_plot_expression) {}
  ~CalculatorModel();
  void modify(const std::string& button) const override;
  std::string expression() const override;
  std::string some_result() const override;
  void edit_variable(const std::string& var_value) const override;
  std::vector<std::map<double, double>> graphs(double x_lo, double x_hi,
                                               int x_pix, double y_lo,
                                               double y_hi,
                                               int y_pix) const override;

 private:
  std::string readble_dblToStr(double num) const;
  std::string* const result;
  const ComputExpressionWithVariable* const comp_expression_x;
  const Plotable* const graph_plot_expression;
};

}  // namespace scn

#endif  // MODEL_H
