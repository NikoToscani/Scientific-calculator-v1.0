/*!
  \file
  \brief Header file with Controller class declaration and implementation
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"
#include "../view/view.h"

namespace scn {

/*!
  \brief Class - Controller of MVC pattern

  Implements View::CallbackController introduced in View class and
  allowing data input from View to Controller meanwhile decoupling
  View class with Model, thus Controller performs data type conversion
  and input/output between View and Model and knows about external
  libraries used by them.
  Allows DI of ptr to View and ptr to Model.
*/
class Controller : private View::CallbackController {
 public:
  Controller(View* view, Model* model) : view(view), model(model) {
    this->view->controller = this;
  }

  /*!
    cause sending corresponding message to model (edit/clear expression)
  */
  void handle_button_pressed(const QString& button) override {
    model->modify(button.toStdString());
  }

  /*!
    \return expression input by user as QString
  */
  QString expression_content() override {
    return QString::fromStdString(model->expression());
  }

  /*!
    \return output of calculation or error as QString
  */
  QString result_content() override {
    return QString::fromStdString(model->some_result());
  }

  /*!
    cause assignig string value to variable X in expression
  */
  void edit_variable(const QString& var_value) override {
    model->edit_variable(var_value.toStdString());
  }

  /*!
    \return collection of graphs to represent graph expression
  */
  QVector<QMap<double, double>> graph_content(double x_lo, double x_hi,
                                              int x_pix, double y_lo,
                                              double y_hi, int y_pix) override {
    QVector<QMap<double, double>> result;
    for (const auto& std_map :
         model->graphs(x_lo, x_hi, x_pix, y_lo, y_hi, y_pix)) {
      QMap<double, double> qmap;
      for (const auto& [key, value] : std_map) {
        qmap.insert(key, value);
      }
      result.append(qmap);
    }
    return result;
  }

 private:
  View* view;
  Model* model;
};

}  // namespace scn

#endif  // CONTROLLER_H
