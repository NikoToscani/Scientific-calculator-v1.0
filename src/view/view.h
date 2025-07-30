/*!
  \file
  \brief Header file with View class declaration
*/
#ifndef VIEW_H
#define VIEW_H

#include <QLabel>
#include <QMainWindow>

#include "ui_view.h"

namespace scn {
/*!
  \brief Class - represents visual part of View

  Forward declared Ui::View class representing visual part of View.
  Header file of Ui::View generated automaticaly from *.ui (XML) file
  by uic (User Interface Compiler) during compilation
*/
QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

/*!
  \brief Class - View of MVC pattern

  View inherits and can use public members of QMainWindow class,
  contains Q_OBJECT macro, allowing use of signal-slot mechanism.
  By means of callback can call Controller method for data
  to be sent from View to Model for this data to be processed.
  DI ptr to Ui::View class implementation
*/
class View : public QMainWindow {
  Q_OBJECT

 public:
  /*!
    \brief Interface - abstraction for handling input from View
  */
  class CallbackController {
   public:
    virtual ~CallbackController() {}

    /*!
      cause sending corresponding message to model (edit/clear expression)
    */
    virtual void handle_button_pressed(const QString &button) = 0;

    /*!
      \return expression input by user as QString
    */
    virtual QString expression_content() = 0;

    /*!
      \return output of calculation or error as QString
    */
    virtual QString result_content() = 0;

    /*!
      cause assignig string value to variable X in expression
    */
    virtual void edit_variable(const QString &var_value) = 0;

    /*!
      \return collection of graphs to represent graph expression
    */
    virtual QVector<QMap<double, double>> graph_content(double x_lo,
                                                        double x_hi, int x_pix,
                                                        double y_lo,
                                                        double y_hi,
                                                        int y_pix) = 0;
  };

  View(QWidget *parent = nullptr);
  ~View();
  /*!
    pointer to an interface CallbackController
    implemented by Controller of MVC
  */
  CallbackController *controller;

 private slots:
  void expression_slot();
  void graph_slot();

 private:
  void setView();
  Ui::View *ui_view;
  QString expression;
  QString result;
  QVector<QMap<double, double>> graphs;
};

}  // namespace scn

#endif  // VIEW_H
