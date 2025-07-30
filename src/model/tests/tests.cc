/*!
  \file
  \brief Model library unit test file
*/
#include <gtest/gtest.h>

#include "../model.h"

#define TOL 1e-7

namespace scn {

TEST(ShuntingYardStringStack, test_0) {
  ShuntingYardStringStack stack;
  EXPECT_TRUE(stack.empty());
  stack.push("sin");
  EXPECT_FALSE(stack.empty());
  stack.clear();
  EXPECT_TRUE(stack.empty());
}

TEST(ShuntingYardStringStack, test_1) {
  ShuntingYardStringStack stack;
  stack.push("-");
  stack.push("+");
  stack.push("mod");
  stack.push("*");
  stack.push("cos");
  stack.push("sin");
  stack.push("(");
  stack.push("(");
  std::vector<std::string> test0({"("});
  EXPECT_EQ(stack.left_bracket(), test0);
  stack.pop_multiple(stack.left_bracket());
  std::vector<std::string> test1({"("});
  EXPECT_EQ(stack.left_bracket(), test1);
  stack.pop_multiple(stack.left_bracket());
  EXPECT_FALSE(stack.empty());
}

TEST(ShuntingYardStringStack, test_1_1) {
  ShuntingYardStringStack stack;
  stack.push("-");
  stack.push("+");
  stack.push("mod");
  stack.push("*");
  stack.push("cos");
  stack.push("sin");
  EXPECT_FALSE(stack.empty());
  std::vector<std::string> test0({});
  EXPECT_EQ(stack.left_bracket(), test0);
}

TEST(ShuntingYardStringStack, test_2) {
  ShuntingYardStringStack stack;
  stack.push("-");
  stack.push("+");
  stack.push("mod");
  stack.push("*");
  stack.push("cos");
  stack.push("sin");
  std::vector<std::string> test0({"sin", "cos"});
  EXPECT_EQ(stack.unary_operators(), test0);
  stack.pop_multiple(stack.unary_operators());
  EXPECT_FALSE(stack.empty());
}

TEST(ShuntingYardStringStack, test_2_1) {
  ShuntingYardStringStack stack;
  stack.push("-");
  stack.push("+");
  stack.push("mod");
  stack.push("*");
  std::vector<std::string> test0({});
  EXPECT_EQ(stack.unary_operators(), test0);
}

TEST(ShuntingYardStringStack, test_3) {
  ShuntingYardStringStack stack;
  stack.push("*");
  stack.push("+");
  stack.push("mod");
  stack.push("-");
  std::vector<std::string> test0({"-", "mod", "+", "*"});
  EXPECT_EQ(stack.not_unary_operators(), test0);
  stack.pop_multiple(stack.not_unary_operators());
  EXPECT_TRUE(stack.empty());
}

TEST(ShuntingYardStringStack, test_3_1) {
  ShuntingYardStringStack stack;
  stack.push("sin");
  stack.push("cos");
  stack.push("tan");
  std::vector<std::string> test0({});
  EXPECT_EQ(stack.not_unary_operators(), test0);
}

TEST(ShuntingYardStringStack, test_4) {
  ShuntingYardStringStack stack;
  stack.push("*");
  stack.push("*");
  std::vector<std::string> test0({"*", "*"});
  EXPECT_EQ(stack.hi_preced_operators("+"), test0);
  stack.pop_multiple(stack.hi_preced_operators("+"));
  EXPECT_TRUE(stack.empty());
}

TEST(ShuntingYardStringStack, test_5) {
  ShuntingYardStringStack stack;
  stack.push("*");
  stack.push("/");
  std::vector<std::string> test0({"/", "*"});
  EXPECT_EQ(stack.hi_preced_operators("mod"), test0);
  stack.pop_multiple(stack.hi_preced_operators("mod"));
  EXPECT_TRUE(stack.empty());
}

TEST(ShuntingYardStringStack, test_6) {
  ShuntingYardStringStack stack;
  stack.push("^");
  stack.push("/");
  std::vector<std::string> test0({});
  EXPECT_EQ(stack.hi_preced_operators("^"), test0);
  stack.pop_multiple(stack.hi_preced_operators("^"));
  EXPECT_FALSE(stack.empty());
}

TEST(PostfixStringExpression, test_0_0) {
  ShuntingYardStringStack stack;
  std::vector<std::string> input = {"acos", "unary +", "^", "1",
                                    "2",    "E-",      "3", "E+"};
  std::vector<std::string> expected = {"acos", "unary +", "^", "12E-3E+"};
  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  EXPECT_EQ(expr.tokenized(), expected);
}

TEST(PostfixStringExpression, test_0) {
  ShuntingYardStringStack stack;
  std::vector<std::string> input = {"("};
  std::vector<std::string> expected = {};
  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);

  try {
    expr.postfixed();
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing right parenthesis");
  }
}

TEST(PostfixStringExpression, test_1) {
  // 1 -> 1
  std::vector<std::string> input = {"1"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {"1"};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_2) {
  // ) -> exception "missing left parenthesis"
  std::vector<std::string> input = {")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
    FAIL() << "Expected exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing left parenthesis");
  }
}

TEST(PostfixStringExpression, test_3) {
  // sin -> exception "missing right parenthesis"
  std::vector<std::string> input = {"sin"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing right parenthesis");
  }
}

TEST(PostfixStringExpression, test_4) {
  // ( 1 ) -> 1
  std::vector<std::string> input = {"(", "1", ")"};

  ShuntingYardStringStack stack;
  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {"1"};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_5) {
  // ( sin ) -> exception "missing right parenthesis"
  std::vector<std::string> input = {"(", "sin", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
    FAIL() << "Expected exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing right parenthesis");
  }
}

TEST(PostfixStringExpression, test_6) {
  // () ->
  std::vector<std::string> input = {"(", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_7) {
  // sin 1 -> 1 sin
  std::vector<std::string> input = {"sin", "1"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {"1", "sin"};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_8) {
  // 1 sin -> 1 sin
  std::vector<std::string> input = {"1", "sin"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
    FAIL() << "Expected exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing right parenthesis");
  }
}

TEST(PostfixStringExpression, test_9) {
  // 1 2 -> 12
  std::vector<std::string> input = {"1", "2"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {"12"};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_10) {
  // sin 1 2 -> 12 sin
  std::vector<std::string> input = {"sin", "1", "2"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {"12", "sin"};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_11) {
  // # sin 2 -> Error
  std::vector<std::string> input = {"#", "sin", "2"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {"#", "2", "sin"};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_12) {
  // sin # 2 -> Error
  std::vector<std::string> input = {"sin", "#", "2"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {"#2", "sin"};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_13) {
  // sin 2 # -> Error
  std::vector<std::string> input = {"2#", "sin"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  EXPECT_THROW(expr.postfixed(), std::string);
}

TEST(PostfixStringExpression, test_14) {
  // 3 + 4 * 2 / ( 1 − 5 ) ^ 2 ^ 3 -> 3 4 2 * 1 5 − 2 3 ^ ^ / +
  std::vector<std::string> input = {"3", "+", "4", "*", "2", "/", "(", "1",
                                    "-", "5", ")", "^", "2", "^", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {"3", "4", "2", "*", "1", "5", "-",
                                       "2", "3", "^", "^", "/", "+"};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_15) {
  // 3 + 4 × 2 ÷ ( 1 − 5 ) ^ 2 ^ 77 -> 3 4 2 * 1 5 − 2 77 ^ ^ / +
  std::vector<std::string> input = {"3", "+", "4", "*", "2", "/", "(", "1",
                                    "-", "5", ")", "^", "2", "^", "77"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected = {"3", "4",  "2", "*", "1", "5", "-",
                                       "2", "77", "^", "^", "/", "+"};
  std::vector<std::string> result = expr.postfixed();
  EXPECT_EQ(result, expected);
}

TEST(PostfixStringExpression, test_16) {
  // sin ( cos ( 2 + 5 ) / 3 * 7 ) -> 2 5 + cos 3 / 7 * sin
  std::vector<std::string> input = {"sin", "(", "cos", "(", "2", "+", "5",
                                    ")",   "/", "3",   "*", "7", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"2", "5", "+", "cos", "3", "/", "7", "*", "sin"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_17) {
  // sin ( cos 2 + 5 ) -> 2 cos 5 + sin
  std::vector<std::string> input = {"sin", "(", "cos", "2", "+", "5", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "cos", "5", "+", "sin"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_18) {
  // sin 2 cos 3 -> 2 sin 3 cos
  std::vector<std::string> input = {"sin", "2", "cos", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "sin", "3", "cos"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_19) {
  // u+ 1 -> 1 u+
  std::vector<std::string> input = {"unary +", "1"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"1", "unary +"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_20) {
  // u- 1 -> 1 u-
  std::vector<std::string> input = {"unary -", "1"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"1", "unary -"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_21) {
  // u+ -> +
  std::vector<std::string> input = {"unary +"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
    FAIL() << "Expected exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing right parenthesis");
  }
}

TEST(PostfixStringExpression, test_22) {
  // u- -> -
  std::vector<std::string> input = {"unary -"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
    FAIL() << "Expected exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing right parenthesis");
  }
}

TEST(PostfixStringExpression, test_23) {
  // u+ 1 -> 1 +
  std::vector<std::string> input = {"unary +", "1"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"1", "unary +"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_24) {
  // u- 1 -> 1 -
  std::vector<std::string> input = {"unary -", "1"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"1", "unary -"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_25) {
  // u- ( 2 / 3 ) -> 2 3 / u-
  std::vector<std::string> input = {"unary -", "(", "2", "/", "3", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "3", "/", "unary -"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_26) {
  // u- ( 3 + 4 * 2 / ( 1 − 5 ) ^ 2 ^ 3 ) -> 3 4 2 * 1 5 − 2 3 ^ ^ / + u-
  std::vector<std::string> input = {"unary -", "(", "3", "+", "4", "*",
                                    "2",       "/", "(", "1", "-", "5",
                                    ")",       "^", "2", "^", "3", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"3", "4", "2", "*", "1", "5", "-", "2",
                                     "3", "^", "^", "/", "+", "unary -"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_27) {
  // u+ u- 2 -> 2 u- u+
  std::vector<std::string> input = {"unary +", "unary -", "2"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "unary -", "unary +"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_28) {
  // u- 2 u- 3 -> 2 u- 3 u-
  std::vector<std::string> input = {"unary -", "2", "unary -", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "unary -", "3", "unary -"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_29) {
  // u- 2 u+ 3 -> 2 u- 3 u+
  std::vector<std::string> input = {"unary -", "2", "unary +", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "unary -", "3", "unary +"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_30) {
  // u+ u- 2 3 -> 2 u- u+ 3
  std::vector<std::string> input = {"unary +", "unary -", "2", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"23", "unary -", "unary +"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_31) {
  // u+ u- ( 2 ) 3 -> 2 u- u+ 3
  std::vector<std::string> input = {"unary +", "unary -", "(", "2", ")", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "unary -", "unary +", "3"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_32) {
  // ( u+ u- 2 ) 3 -> 2 u- u+ 3
  std::vector<std::string> input = {"(", "unary +", "unary -", "2", ")", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "unary -", "unary +", "3"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_33) {
  // u+ u- ( u+ u- 2 ) 3 -> 2 u- u+ u- u+ 3
  std::vector<std::string> input = {"unary +", "unary -", "(", "unary +",
                                    "unary -", "2",       ")", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"2", "unary -", "unary +", "unary -", "unary +", "3"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_34) {
  // u- 2 cos 3 -> 2 u- 3 cos
  std::vector<std::string> input = {"unary -", "2", "cos", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "unary -", "3", "cos"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_35) {
  // sin u+ u- 2 3 -> 2 u- u+ sin 3
  std::vector<std::string> input = {"sin", "unary +", "unary -", "2", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"23", "unary -", "unary +", "sin"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_36) {
  // u- 2 + u- 3 -> 2 u- 3 u- +
  std::vector<std::string> input = {"unary -", "2", "+", "unary -", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "unary -", "3", "unary -", "+"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_37) {
  // u- 2 + u+ 3 -> 2 u- 3 u+ +
  std::vector<std::string> input = {"unary -", "2", "+", "unary +", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "unary -", "3", "unary +", "+"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_38) {
  // u+ 2 + u- 3 -> 2 u+ 3 u- +
  std::vector<std::string> input = {"unary +", "2", "+", "unary -", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "unary +", "3", "unary -", "+"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_39) {
  // 2 ^ 3 ^ 4 -> 2 3 4 ^ ^
  std::vector<std::string> input = {"2", "^", "3", "^", "4"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "3", "4", "^", "^"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_40) {
  // 2 ^ u- 1 ^ 4 -> 2 1 u- 4 ^ ^
  std::vector<std::string> input = {"2", "^", "unary -", "1", "^", "4"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "1", "unary -", "4", "^", "^"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_41) {
  // sin u- 2 cos u- 3 -> 2 u- sin 3 u- cos
  std::vector<std::string> input = {"sin", "unary -", "2",
                                    "cos", "unary -", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"2", "unary -", "sin", "3", "unary -", "cos"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_42) {
  // sin u- 2 cos u+ 3 -> 2 u- sin 3 u+ cos
  std::vector<std::string> input = {"sin", "unary -", "2",
                                    "cos", "unary +", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"2", "unary -", "sin", "3", "unary +", "cos"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_43) {
  // sin ( u- 2 ) cos ( u+ 3 ) -> 2 u- sin 3 u+ cos
  std::vector<std::string> input = {"sin", "(", "unary -", "2", ")",
                                    "cos", "(", "unary +", "3", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"2", "unary -", "sin", "3", "unary +", "cos"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_44) {
  // sin u- 2 * cos u+ 3 -> 2 u- sin 3 u+ cos *
  std::vector<std::string> input = {"sin", "unary -", "2", "*",
                                    "cos", "unary +", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"2", "unary -", "sin", "3", "unary +", "cos", "*"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_45) {
  // sin ( u- 2 ) * cos ( u+ 3 ) -> 2 u- sin 3 u+ cos *
  std::vector<std::string> input = {"sin", "(", "unary -", "2", ")", "*",
                                    "cos", "(", "unary +", "3", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"2", "unary -", "sin", "3", "unary +", "cos", "*"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_46) {
  // u- sin 2 u+ cos 3 -> 2 sin u- 3 cos u+
  std::vector<std::string> input = {"unary -", "sin", "2",
                                    "unary +", "cos", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"2", "sin", "unary -", "3", "cos", "unary +"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_47) {
  // sin cos 2 -> 2 cos sin
  std::vector<std::string> input = {"sin", "cos", "2"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected({"2", "cos", "sin"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_48) {
  // 3 * + - / u+ u- 2 -> 3 * + 2 u- u+ / -
  std::vector<std::string> input = {"3", "*",       "+",       "-",
                                    "/", "unary +", "unary -", "2"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"3", "*", "+", "2", "unary -", "unary +", "/", "-"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_49) {
  // 4 * 5 / ( 7 mod u+ u- 2 ) -> 4 5 * 7 2 u- u+ mod /
  std::vector<std::string> input = {"4",   "*",       "5",       "/", "(", "7",
                                    "mod", "unary +", "unary -", "2", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"4", "5", "*", "7", "2", "unary -", "unary +", "mod", "/"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_50) {
  // 6 / u- 1 * u- 2 -> 6 1 u- / 2 u- *
  std::vector<std::string> input = {"6", "/",       "unary -", "1",
                                    "*", "unary -", "2"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  std::vector<std::string> expected(
      {"6", "1", "unary -", "/", "2", "unary -", "*"});
  EXPECT_EQ(expr.postfixed(), expected);
}

TEST(PostfixStringExpression, test_51) {
  // 2 + 3 ) -> Error
  std::vector<std::string> input = {"2", "+", "3", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing left parenthesis");
  }
}

TEST(PostfixStringExpression, test_52) {
  // 2 3 ) -> Error
  std::vector<std::string> input = {"2", "3", ")"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing left parenthesis");
  }
}

TEST(PostfixStringExpression, test_53) {
  // 2 u- -> Error
  std::vector<std::string> input = {"2", "unary -"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing right parenthesis");
  }
}

TEST(PostfixStringExpression, test_54) {
  // 2 u- + 3 -> Error
  std::vector<std::string> input = {"2", "unary -", "+", "3"};
  ShuntingYardStringStack stack;

  PostfixStringExpression expr(&stack);
  for (const auto& lexema : input) expr.edit(lexema);
  try {
    expr.postfixed();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "missing right parenthesis");
  }
}

TEST(CalculatingDblStack, test_0) {
  std::vector<double> input;
  CalculatingDblStack stack_calc;
  try {
    stack_calc.push("");
    stack_calc.top();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "std::stod error: string <> is unconvertable to number");
  }
}

TEST(CalculatingDblStack, test_1) {
  std::vector<double> input;
  CalculatingDblStack stack_calc;
  try {
    stack_calc.push("a");
    stack_calc.top();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message,
              "std::stod error: string <a> is unconvertable to number");
  }
}

TEST(CalculatingDblStack, test_2) {
  std::vector<double> input;
  CalculatingDblStack stack_calc;
  try {
    stack_calc.push("1.79769e+309");
    stack_calc.top();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message,
              "std::stod error: string <1.79769e+309> is to big for current "
              "number type (double)");
  }
}

TEST(CalculatingDblStack, test_3) {
  std::vector<double> input;
  CalculatingDblStack stack_calc;
  try {
    stack_calc.push("1 ");
    stack_calc.top();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "string <1 > is unconvertable to number");
  }
}

TEST(CalculatingDblStack, test_4) {
  std::vector<double> input;
  CalculatingDblStack stack_calc;
  try {
    stack_calc.push("1.2.3");
    stack_calc.top();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "string <1.2.3> is unconvertable to number");
  }
}

TEST(CalculatingDblStack, test_5) {
  std::vector<double> input;
  CalculatingDblStack stack_calc;
  try {
    stack_calc.push("1,2");
    stack_calc.top();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "string <1,2> is unconvertable to number");
  }
}

TEST(CalculatingDblStack, test_6) {
  std::vector<double> input;
  CalculatingDblStack stack_calc;
  stack_calc.push("1.23E+4");
  EXPECT_EQ(stack_calc.top(), 12300);
}

TEST(CalculatingDblStack, test_7) {
  std::vector<double> input;
  CalculatingDblStack stack_calc;
  stack_calc.push("12.3E-4");
  EXPECT_EQ(stack_calc.top(), 0.00123);
}

TEST(CalculatingDblStack, test_8) {
  std::vector<double> input;
  CalculatingDblStack stack_calc;
  try {
    stack_calc.push("12.3E+4.5");
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "string <12.3E+4.5> is unconvertable to number");
  }
}

TEST(CalculatingDblStack, test_9) {
  CalculatingDblStack stack_calc;
  stack_calc.push("2");
  stack_calc.push("0.5");
  stack_calc.push("^");
  EXPECT_NEAR(stack_calc.top(), 1.4142136, TOL);
}

TEST(CalculatingDblStack, test_10) {
  std::vector<double> input = {};
  CalculatingDblStack stack_calc;
  try {
    stack_calc.push("unary +");
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "not enough arguments");
  }
}

TEST(CalculatingDblStack, test_11) {
  std::vector<double> input = {2};
  CalculatingDblStack stack_calc;
  try {
    stack_calc.push("^");
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "not enough arguments");
  }
}

TEST(ComputableStringExpression, test_0) {
  ShuntingYardStringStack oper_stack;
  PostfixStringExpression infix_expr(&oper_stack);
  CalculatingDblStack stack_calc;
  ComputableStringExpression comp_expression(&infix_expr, &stack_calc);
  comp_expression.edit("unary +");
  EXPECT_EQ(comp_expression.string(), "+");
  comp_expression.edit("unary -");
  EXPECT_EQ(comp_expression.string(), "+-");
  comp_expression.edit(".");
  EXPECT_EQ(comp_expression.string(), "+-.");
  comp_expression.edit("<-");
  EXPECT_EQ(comp_expression.string(), "+-");
  comp_expression.clear();
  EXPECT_EQ(comp_expression.string(), "");
  comp_expression.edit("sin");
  EXPECT_EQ(comp_expression.string(), "sin");
  comp_expression.edit("E+");
  comp_expression.edit(".");
  EXPECT_EQ(comp_expression.string(), "sinE+.");
  comp_expression.edit("<-");
  EXPECT_EQ(comp_expression.string(), "sinE+");
  comp_expression.edit("<-");
  EXPECT_EQ(comp_expression.string(), "sin");
  comp_expression.edit("<-");
  EXPECT_EQ(comp_expression.string(), "");
  comp_expression.edit("<-");
  EXPECT_EQ(comp_expression.string(), "");
}

TEST(ComputableStringExpression, test_1) {
  ShuntingYardStringStack oper_stack;
  PostfixStringExpression infix_expr(&oper_stack);
  CalculatingDblStack stack_calc;
  ComputableStringExpression comp_expression(&infix_expr, &stack_calc);
  comp_expression.edit("2");
  comp_expression.edit("^");
  comp_expression.edit("unary -");
  comp_expression.edit("1");
  comp_expression.edit("^");
  comp_expression.edit("2");
  EXPECT_EQ(comp_expression.string(), "2^-1^2");
  EXPECT_EQ(comp_expression.solution(), 2);
}

TEST(ComputableStringExpression, test_2) {
  ShuntingYardStringStack oper_stack;
  PostfixStringExpression infix_expr(&oper_stack);
  CalculatingDblStack stack_calc;
  ComputableStringExpression comp_expression(&infix_expr, &stack_calc);
  comp_expression.edit("2");
  comp_expression.edit("^");
  comp_expression.edit("unary -");
  comp_expression.edit("1");
  comp_expression.edit("^");
  EXPECT_EQ(comp_expression.string(), "2^-1^");
  try {
    comp_expression.solution();
    FAIL() << "Expected std::string exception";
  } catch (const std::string& message) {
    EXPECT_EQ(message, "not enough arguments");
  }
}

TEST(VariableCalculator, test_0) {
  ShuntingYardStringStack oper_stack;
  PostfixStringExpression infix_expr(&oper_stack);
  CalculatingDblStack stack_calc;
  std::string variable;
  CalculatingStack_with_variable stack_w_X(&stack_calc, &variable);
  ComputableStringExpression comp_expression(&infix_expr, &stack_w_X);
  ComputStrExpressionWithVariable var_calc(&comp_expression, &variable);
  var_calc.edit("2");
  var_calc.edit("^");
  var_calc.edit("unary -");
  var_calc.edit("X");
  var_calc.edit("^");
  var_calc.edit("2");
  var_calc.edit_variable("1");
  EXPECT_EQ(var_calc.string(), "2^-X^2");
  EXPECT_EQ(var_calc.solution(), 2);
}

TEST(GraphVarCalculator, test_4) {
  ShuntingYardStringStack oper_stack;
  PostfixStringExpression infix_expr(&oper_stack);
  CalculatingDblStack stack_calc;
  std::string variable;
  CalculatingStack_with_variable stack_w_X(&stack_calc, &variable);
  ComputableStringExpression comp_expression(&infix_expr, &stack_w_X);
  ComputStrExpressionWithVariable var_calc(&comp_expression, &variable);
  PlotableExpression graph_calc(&var_calc);
  var_calc.edit("X");
  var_calc.edit("^");
  var_calc.edit("2");
  EXPECT_EQ(var_calc.string(), "X^2");
  std::vector<std::map<double, double>> graph =
      graph_calc.graphs(-2, 2, 2, -2, 4, 1);
  std::map<double, double> test_map = {
      {-2, 4}, {-1, 1}, {0, 0}, {1, 1}, {2, 4}};
  for (const auto& [key, value] : test_map)
    EXPECT_EQ(graph.back().at(key), value);
}

TEST(CalculatorModel, test_0) {
  std::string variable;
  // Calculating Stack
  scn::CalculatingDblStack stack_simple;
  // Calculating Stack
  scn::CalculatingStack_with_variable stack_w_X(&stack_simple, &variable);
  // Shunting Yard Algorithm Stack
  scn::ShuntingYardStringStack oper_stack;
  // Postfixable Expression
  scn::PostfixStringExpression infix_expr(&oper_stack);
  // Computable Expression
  scn::ComputableStringExpression comp_expression(&infix_expr, &stack_w_X);
  // Computable Expression With Variable
  scn::ComputStrExpressionWithVariable comp_expression_x(&comp_expression,
                                                         &variable);
  // ExpressionGraphPlot
  scn::PlotableExpression graph_plot_expression(&comp_expression_x);
  scn::CalculatorModel model(&comp_expression_x, &graph_plot_expression);

  model.modify("unary +");

  EXPECT_EQ(model.expression(), "+");
  model.modify("unary -");

  EXPECT_EQ(model.expression(), "+-");
  model.modify(".");

  EXPECT_EQ(model.expression(), "+-.");
  model.modify("<-");

  EXPECT_EQ(model.expression(), "+-");
  model.modify("AC");

  EXPECT_EQ(model.expression(), "");
  model.modify("sin");

  EXPECT_EQ(model.expression(), "sin");
  model.modify("E+");

  model.modify(".");

  EXPECT_EQ(model.expression(), "sinE+.");
  model.modify("<-");

  EXPECT_EQ(model.expression(), "sinE+");
  model.modify("<-");

  EXPECT_EQ(model.expression(), "sin");
  model.modify("<-");

  EXPECT_EQ(model.expression(), "");
  model.modify("<-");

  EXPECT_EQ(model.expression(), "");
}

TEST(CalculatorModel, test_1) {
  std::string variable;
  // Calculating Stack
  scn::CalculatingDblStack stack_simple;
  // Calculating Stack
  scn::CalculatingStack_with_variable stack_w_X(&stack_simple, &variable);
  // Shunting Yard Algorithm Stack
  scn::ShuntingYardStringStack oper_stack;
  // Postfixable Expression
  scn::PostfixStringExpression infix_expr(&oper_stack);
  // Computable Expression
  scn::ComputableStringExpression comp_expression(&infix_expr, &stack_w_X);
  // Computable Expression With Variable
  scn::ComputStrExpressionWithVariable comp_expression_x(&comp_expression,
                                                         &variable);
  // ExpressionGraphPlot
  scn::PlotableExpression graph_plot_expression(&comp_expression_x);
  scn::CalculatorModel model(&comp_expression_x, &graph_plot_expression);

  model.modify("2");

  model.modify("^");

  model.modify("unary -");

  model.modify("1");

  model.modify("^");

  model.modify("2");

  EXPECT_EQ(model.expression(), "2^-1^2");
  model.modify("=");

  EXPECT_EQ(model.expression(), "2^-1^2");
  EXPECT_EQ(model.some_result(), "2");
}

TEST(CalculatorModel, test_2) {
  std::string variable;
  // Calculating Stack
  scn::CalculatingDblStack stack_simple;
  // Calculating Stack
  scn::CalculatingStack_with_variable stack_w_X(&stack_simple, &variable);
  // Shunting Yard Algorithm Stack
  scn::ShuntingYardStringStack oper_stack;
  // Postfixable Expression
  scn::PostfixStringExpression infix_expr(&oper_stack);
  // Computable Expression
  scn::ComputableStringExpression comp_expression(&infix_expr, &stack_w_X);
  // Computable Expression With Variable
  scn::ComputStrExpressionWithVariable comp_expression_x(&comp_expression,
                                                         &variable);
  // ExpressionGraphPlot
  scn::PlotableExpression graph_plot_expression(&comp_expression_x);
  scn::CalculatorModel model(&comp_expression_x, &graph_plot_expression);

  model.modify("2");

  model.modify("^");

  model.modify("unary -");

  model.modify("X");

  model.modify("^");

  model.modify("2");
  model.edit_variable("1");

  EXPECT_EQ(model.expression(), "2^-X^2");
  model.modify("=");

  EXPECT_EQ(model.expression(), "2^-X^2");
  EXPECT_EQ(model.some_result(), "2");
}

TEST(CalculatorModel, test_3) {
  std::string variable;
  // Calculating Stack
  scn::CalculatingDblStack stack_simple;
  // Calculating Stack
  scn::CalculatingStack_with_variable stack_w_X(&stack_simple, &variable);
  // Shunting Yard Algorithm Stack
  scn::ShuntingYardStringStack oper_stack;
  // Postfixable Expression
  scn::PostfixStringExpression infix_expr(&oper_stack);
  // Computable Expression
  scn::ComputableStringExpression comp_expression(&infix_expr, &stack_w_X);
  // Computable Expression With Variable
  scn::ComputStrExpressionWithVariable comp_expression_x(&comp_expression,
                                                         &variable);
  // ExpressionGraphPlot
  scn::PlotableExpression graph_plot_expression(&comp_expression_x);
  scn::CalculatorModel model(&comp_expression_x, &graph_plot_expression);

  model.modify("2");

  model.modify("^");

  model.modify("unary -");

  model.modify("X");

  model.modify("^");
  model.edit_variable("1");

  EXPECT_EQ(model.expression(), "2^-X^");
  model.modify("=");

  EXPECT_EQ(model.expression(), "2^-X^");
  EXPECT_EQ(model.some_result(), "not enough arguments");
}

TEST(CalculatorModel, test_4) {
  std::string variable;
  // Calculating Stack
  scn::CalculatingDblStack stack_simple;
  // Calculating Stack
  scn::CalculatingStack_with_variable stack_w_X(&stack_simple, &variable);
  // Shunting Yard Algorithm Stack
  scn::ShuntingYardStringStack oper_stack;
  // Postfixable Expression
  scn::PostfixStringExpression infix_expr(&oper_stack);
  // Computable Expression
  scn::ComputableStringExpression comp_expression(&infix_expr, &stack_w_X);
  // Computable Expression With Variable
  scn::ComputStrExpressionWithVariable comp_expression_x(&comp_expression,
                                                         &variable);
  // ExpressionGraphPlot
  scn::PlotableExpression graph_plot_expression(&comp_expression_x);
  scn::CalculatorModel model(&comp_expression_x, &graph_plot_expression);

  model.modify("X");

  model.modify("^");

  model.modify("2");

  std::map<double, double> test_map = {
      {-2, 4}, {-1, 1}, {0, 0}, {1, 1}, {2, 4}};
  for (const auto& [key, value] : test_map)
    EXPECT_EQ(model.graphs(-2, 2, 2, -2, 4, 1).back().at(key), value);
}

TEST(CalculatorModel, test_5) {
  std::string variable;
  // Calculating Stack
  scn::CalculatingDblStack stack_simple;
  // Calculating Stack
  scn::CalculatingStack_with_variable stack_w_X(&stack_simple, &variable);
  // Shunting Yard Algorithm Stack
  scn::ShuntingYardStringStack oper_stack;
  // Postfixable Expression
  scn::PostfixStringExpression infix_expr(&oper_stack);
  // Computable Expression
  scn::ComputableStringExpression comp_expression(&infix_expr, &stack_w_X);
  // Computable Expression With Variable
  scn::ComputStrExpressionWithVariable comp_expression_x(&comp_expression,
                                                         &variable);
  // ExpressionGraphPlot
  scn::PlotableExpression graph_plot_expression(&comp_expression_x);
  scn::CalculatorModel model(&comp_expression_x, &graph_plot_expression);

  model.modify("X");

  model.modify("^");

  model.modify("2");

  model.modify("-");

  model.edit_variable("1");

  model.modify("=");

  EXPECT_EQ(model.expression(), "X^2-");
  EXPECT_EQ(model.some_result(), "not enough arguments");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace scn
