/*!
  \file
  \brief main() function file

  The Scientific_calculator_V1.0 program is implemented in the C++ language
  of the C++20 standard in the object-oriented programming paradigm
  using the MVC (Model-View-Controller) pattern.
  The model is presented as a static class library that implements
  calculations and graph plotting.
  The graphical user interface is implemented with Qt library.
*/
#include <QApplication>

#include "controller/controller.h"

/*!
  The main function serves as Composition Root of the application:
  it creates instances of the all main objects (QApplication, View,
  Controller, CalculatorModel) as well as concrete instances of
  objects, that are used in CalculatorModel, and calls the
  methods for demonstrating the widget and the main event loop.
  \return exit code of the program
*/
int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  // Calculating Stack
  scn::CalculatingDblStack stack_simple;
  // Variable as string
  std::string variable;
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
  // Model
  scn::CalculatorModel model(&comp_expression_x, &graph_plot_expression);
  scn::View view;
  scn::Controller ctrl(&view, &model);
  view.show();
  return app.exec();
}
