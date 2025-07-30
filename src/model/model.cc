/*!
  \file
  \brief Model library implementation file
*/
#include "model.h"

namespace scn {
// class ShuntingYardStringStack
ShuntingYardStringStack::~ShuntingYardStringStack() {
  delete stack;
  for (const auto& [key, value] : functions) {
    delete value;
  }
}
/*!
  Adds operator token to the stack
  \param[in] token input token
*/
void ShuntingYardStringStack::push(const std::string& token) const {
  stack->push_back(token);
}

/*!
  Pop_back exact number of tokens equal to input size of vector.
  No emptines checked, no equality of input/existing elements checked.
  \param[in] tokens input vector of tokens
*/
void ShuntingYardStringStack::pop_multiple(
    const std::vector<std::string> tokens) const {
  for (size_t i = 0; i != tokens.size(); ++i) {
    stack->pop_back();
  }
}

/*!
  If at the back of stack has left parenthesis, then
  vector of tokens with one token - left parenthesis -
  will be returned, else empty vector is returned
  \return vector of one token - left parenthesis
*/
std::vector<std::string> ShuntingYardStringStack::left_bracket() const {
  std::vector<std::string> result;
  auto it = stack->rbegin();
  if (it != stack->rend() && *it == "(") {
    result.push_back(*it);
  }
  return result;
}

/*!
  If at the back of stack has unary operators, then
  vector of tokens consisted of this tokens will be
  returned taken from the back (LIFO) and pushed to
  returning vector one by one, else empty vector is returned
  \return vector of unary operator tokens
*/
std::vector<std::string> ShuntingYardStringStack::unary_operators() const {
  std::vector<std::string> result;
  auto it = stack->rbegin();
  while (it != stack->rend() && *it != "(" && functions.at(*it)->arity() == 1) {
    result.push_back(*it);
    ++it;
  }
  return result;
}

/*!
  If at the back of stack has tokens that are not unary or
  left parenthesis, then such tokens will be taken
  from the back (LIFO) and pushed to returning vector
  one by one, else empty vector is returned.
  \return vector of non-unary and non-left-parenthesis operator tokens
*/
std::vector<std::string> ShuntingYardStringStack::not_unary_operators() const {
  std::vector<std::string> result;
  auto it = stack->rbegin();
  while (it != stack->rend() && *it != "(" && functions.at(*it)->arity() > 1) {
    result.push_back(*it);
    ++it;
  }
  return result;
}

/*!
  Compares input token operator with tokens at the back of the stack
  one by one and, if stack token is operator, has higher precedence
  or has same precedence and input token is left associative,
  then such tokens will be taken from the back (LIFO) and pushed
  to returning vector one by one, else empty vector is returned.
  \param[in] token input token
  \return vector of unary operator tokens
*/
std::vector<std::string> ShuntingYardStringStack::hi_preced_operators(
    const std::string& token) const {
  std::vector<std::string> result;
  int token_preced = func_precedence.at(token);
  auto it = stack->rbegin();
  while (it != stack->rend() && *it != "(" && functions.at(*it)->arity() > 1 &&
         (func_precedence.at(*it) > token_preced ||
          (func_precedence.at(*it) == token_preced &&
           functions.at(token)->left_associative()))) {
    result.push_back(*it);
    ++it;
  }
  return result;
}

/*!
  Check if the stack is empty
  \return true if stack is empty, false if not empty
*/
bool ShuntingYardStringStack::empty() const { return stack->empty(); }

/*!
  Clears the stack
*/
void ShuntingYardStringStack::clear() const { stack->clear(); }
// end of class ShuntingYardStringStack

PostfixStringExpression::~PostfixStringExpression() {
  delete expression;
  for (const auto& [key, value] : functions) {
    delete value;
  }
}

/*!
  Edits the expression using the input button.
  \param[in] button input button as a string
*/
void PostfixStringExpression::edit(const std::string& button) const {
  if (button == "<-") {
    if (!expression->empty()) expression->pop_back();
  } else {
    expression->push_back(button);
  }
}

/*!
  Clears the expression.
*/
void PostfixStringExpression::clear() const { expression->clear(); }

/*!
  Returns the expression as a string.
  \return string representation of the expression
*/
std::string PostfixStringExpression::string() const {
  std::string result;
  for (const auto& lexeme : *expression) {
    if (lexeme == "unary +")
      result += "+";
    else if (lexeme == "unary -")
      result += "-";
    else
      result += lexeme;
  }
  return result;
}

/*!
  Tokenizes the expression.
  \return vector of tokens
*/
std::vector<std::string> PostfixStringExpression::tokenized() const {
  std::vector<std::string> result;
  for (size_t i = 0; i != expression->size();) {
    if (functions.contains(expression->at(i)) || expression->at(i) == "(" ||
        expression->at(i) == ")") {
      result.push_back(expression->at(i));
      i++;
    } else {
      std::string string_number;
      while (i != expression->size() &&
             !functions.contains(expression->at(i)) &&
             expression->at(i) != "(" && expression->at(i) != ")") {
        string_number += expression->at(i);
        i++;
      }
      result.push_back(string_number);
    }
  }
  return result;
}

/*!
  Converts the tokenized expression to postfix notation.
  \return vector of tokens in postfix notation
*/
std::vector<std::string> PostfixStringExpression::postfixed() const {
  std::vector<std::string> result;
  stack->clear();
  for (const auto& token : tokenized()) {
    if (functions.contains(token)) {
      append_tokens(stack->hi_preced_operators(token), &result);
      stack->pop_multiple(stack->hi_preced_operators(token));
      stack->push(token);
    } else if (token == "(") {
      stack->push(token);
    } else if (token == ")") {
      append_tokens(stack->not_unary_operators(), &result);
      stack->pop_multiple(stack->not_unary_operators());
      if (stack->empty()) throw std::string("missing left parenthesis");
      stack->pop_multiple(stack->left_bracket());
      append_tokens(stack->unary_operators(), &result);
      stack->pop_multiple(stack->unary_operators());
    } else {
      append_tokens({token}, &result);
      append_tokens(stack->unary_operators(), &result);
      stack->pop_multiple(stack->unary_operators());
    }
  }
  append_tokens(stack->not_unary_operators(), &result);
  stack->pop_multiple(stack->not_unary_operators());
  if (!stack->empty()) throw std::string("missing right parenthesis");
  return result;
}

void PostfixStringExpression::append_tokens(
    std::vector<std::string> src, std::vector<std::string>* dest) const {
  dest->insert(dest->end(), src.begin(), src.end());
}

CalculatingDblStack::~CalculatingDblStack() {
  delete stack;
  for (const auto& [key, value] : functions) {
    delete value;
  }
}

/*!
  Pushes a token (e.g., number or operator) onto the stack.
  \param[in] token input token
*/
void CalculatingDblStack::push(const std::string& token) const {
  std::vector<double> operands;
  if (functions.contains(token)) {
    for (int i = 0; i != functions.at(token)->arity(); ++i) {
      if (stack->empty()) throw std::string("not enough arguments");
      operands.push_back(stack->back());
      stack->pop_back();
    }
    stack->push_back(functions.at(token)->operator()(operands));
    operands.clear();
  } else {
    stack->push_back(strToDbl(token));
  }
}

/*!
  Returns the top value of the stack.
  \return top value as double
*/
double CalculatingDblStack::top() const { return stack->back(); }

/*!
  Clears the stack.
*/
void CalculatingDblStack::clear() const { stack->clear(); }

double CalculatingDblStack::strToDbl(const std::string& str) const {
  double result;
  size_t read = 0;
  try {
    result = std::stod(str, &read);
  } catch (std::invalid_argument&) {
    throw std::string("std::stod error: string <" + str +
                      "> is unconvertable to number");
  } catch (std::out_of_range&) {
    throw std::string("std::stod error: string <" + str +
                      "> is to big for current number type (double)");
  }
  if (str.size() != read) {
    throw std::string("string <" + str + "> is unconvertable to number");
  }
  return result;
}

/*!
  Edits the expression using the input button.
  \param[in] button input button as a string
*/
void ComputableStringExpression::edit(const std::string& button) const {
  expression->edit(button);
}

/*!
  Clears the expression.
*/
void ComputableStringExpression::clear() const { expression->clear(); }

/*!
  Returns the expression as a string.
  \return string representation of the expression
*/
std::string ComputableStringExpression::string() const {
  return expression->string();
}

/*!
  Computes and returns the result of the expression.
  \return numeric solution
*/
double ComputableStringExpression::solution() const {
  double result = 0;
  calc_stack->clear();
  for (const auto& token : expression->postfixed()) {
    calc_stack->push(token);
    result = calc_stack->top();
  }
  return result;
}

/*!
  Pushes a token (e.g., number or operator) onto the stack.
  \param[in] token input token
*/
void CalculatingStack_with_variable::push(const std::string& token) const {
  if (token == "X") {
    stack->push(*X_str_var);
  } else {
    stack->push(token);
  }
}

/*!
  Returns the top value of the stack.
  \return top value as double
*/
double CalculatingStack_with_variable::top() const { return stack->top(); }

/*!
  Clears the stack.
*/
void CalculatingStack_with_variable::clear() const { stack->clear(); }

/*!
  Edits the expression using the input button.
  \param[in] button input button as a string
*/
void ComputStrExpressionWithVariable::edit(const std::string& button) const {
  comp_expression->edit(button);
}

/*!
  Clears the expression.
*/
void ComputStrExpressionWithVariable::clear() const {
  comp_expression->clear();
}

/*!
  Returns the expression as a string.
  \return string representation of the expression
*/
std::string ComputStrExpressionWithVariable::string() const {
  return comp_expression->string();
}

/*!
  Computes and returns the result of the expression.
  \return numeric solution
*/
double ComputStrExpressionWithVariable::solution() const {
  return comp_expression->solution();
}

/*!
  Updates the internal variable value.
  \param[in] var_value value as a string
*/
void ComputStrExpressionWithVariable::edit_variable(
    const std::string& var_value) const {
  *X_str_var = var_value;
}

/*!
  Generates graphs over a defined x/y region and pixel space.
  \return vector of graph maps (x->y points)
*/
std::vector<std::map<double, double>> PlotableExpression::graphs(
    double x_lo, double x_hi, int x_pix, double y_lo, double y_hi,
    int y_pix) const {
  std::map<double, double> graph;
  double prev_x, y, prev_y;
  double delta_x = 1.0 / x_pix;
  double delta_y = 1.0 / y_pix;
  for (double x = x_lo; x <= x_hi; x += delta_x) {
    expression_with_var->edit_variable(precise_dblToStr(x));
    y = expression_with_var->solution();
    graph[x] = y;
    if (x != x_lo && std::abs(y - prev_y) > delta_y) {
      graph.merge(recursive_plot(prev_x, x, delta_y, prev_y, y, y_lo, y_hi));
    }
    prev_x = x;
    prev_y = y;
  }
  return cut_subgraphs(graph, y_lo, y_hi);
}

std::map<double, double> PlotableExpression::recursive_plot(
    double x_min, double x_max, double delta_y, double y_min, double y_max,
    double y_lo, double y_hi) const {
  std::map<double, double> result;
  double x_mid = (x_min + x_max) / 2;
  expression_with_var->edit_variable(precise_dblToStr(x_mid));
  double y_mid = expression_with_var->solution();
  result[x_mid] = y_mid;
  if (std::abs(y_mid - y_min) < delta_y || (y_min < y_mid && y_min > y_hi) ||
      (y_max < y_mid && y_max > y_hi) || (y_max > y_mid && y_max < y_lo) ||
      (y_min > y_mid && y_min < y_lo)) {
    return result;
  } else {
    result.merge(
        recursive_plot(x_min, x_mid, delta_y, y_min, y_mid, y_lo, y_hi));
    result.merge(
        recursive_plot(x_mid, x_max, delta_y, y_mid, y_max, y_lo, y_hi));
    return result;
  }
}

std::vector<std::map<double, double>> PlotableExpression::cut_subgraphs(
    std::map<double, double>& source_graph, double y_lo, double y_hi) const {
  std::vector<std::map<double, double>> graphs;
  std::map<double, double> temp_graph;
  for (const auto& [key, value] : source_graph) {
    if (value >= y_lo && value <= y_hi) {
      temp_graph[key] = value;
    } else {
      if (!temp_graph.empty()) {
        graphs.push_back(temp_graph);
        temp_graph.clear();
      }
    }
  }
  if (!temp_graph.empty()) graphs.push_back(temp_graph);
  return graphs;
}

std::string PlotableExpression::precise_dblToStr(double num) const {
  char buffer[BUF_SIZE];
  int written = std::snprintf(buffer, sizeof(buffer), "%a", num);
  if (written < 0) {
    throw std::string("Error during convertion double to char buffer");
  } else if (written >= (int)sizeof(buffer)) {
    throw std::string("Char buffer is too small for converted double");
  }
  return buffer;
}

CalculatorModel::~CalculatorModel() { delete result; }

/*!
  Handles most button presses (except Plot), updates expression and result.
  Catches and stores error messages on failure.
  \param[in] button button input string
*/
void CalculatorModel::modify(const std::string& button) const {
  if (button == "AC") {
    comp_expression_x->clear();
    result->clear();
  } else if (button == "=") {
    try {
      *result = readble_dblToStr(comp_expression_x->solution());
    } catch (const std::string& message) {
      *result = message;
    }
  } else {
    comp_expression_x->edit(button);
  }
}

/*!
  Returns the current expression string.
  \return string representation of expression
*/
std::string CalculatorModel::expression() const {
  return comp_expression_x->string();
}

/*!
  Returns the current computed result or error message.
  \return string value of solution
*/
std::string CalculatorModel::some_result() const { return *result; }

std::string CalculatorModel::readble_dblToStr(double num) const {
  char buffer[DBL_DIG * 2];
  int written = std::snprintf(buffer, sizeof(buffer), "%.*G", DBL_DIG, num);
  if (written < 0) {
    throw std::string("Error during convertion double to char buffer");
  } else if (written >= (int)sizeof(buffer)) {
    throw std::string("Char buffer is too small for converted double");
  }
  return buffer;
}

/*!
  cause assignig string value to variable X in expression
*/
void CalculatorModel::edit_variable(const std::string& var_value) const {
  comp_expression_x->edit_variable(var_value);
}

/*!
  Handles Plot and AC button presses.
  \return vector of graph data (can be empty on error)
*/
std::vector<std::map<double, double>> CalculatorModel::graphs(
    double x_lo, double x_hi, int x_pix, double y_lo, double y_hi,
    int y_pix) const {
  std::vector<std::map<double, double>> graph_vector;
  if (!expression().empty()) {
    try {
      graph_vector =
          graph_plot_expression->graphs(x_lo, x_hi, x_pix, y_lo, y_hi, y_pix);
    } catch (const std::string& message) {
      *result = message;
    }
  }
  return graph_vector;
}

}  // namespace scn
