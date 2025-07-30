/*!
  \file
  \brief Class View implementation file
*/
#include "view.h"

namespace scn {

View::View(QWidget *parent) : QMainWindow(parent), ui_view(new Ui::View) {
  ui_view->setupUi(this);

  // array of QPushButton pointers
  QPushButton *simpleButtons[] = {ui_view->pushButton_0,
                                  ui_view->pushButton_1,
                                  ui_view->pushButton_2,
                                  ui_view->pushButton_3,
                                  ui_view->pushButton_4,
                                  ui_view->pushButton_5,
                                  ui_view->pushButton_6,
                                  ui_view->pushButton_7,
                                  ui_view->pushButton_8,
                                  ui_view->pushButton_9,
                                  ui_view->pushButton_dot,
                                  ui_view->pushButton_plus,
                                  ui_view->pushButton_minus,
                                  ui_view->pushButton_mult,
                                  ui_view->pushButton_div,
                                  ui_view->pushButton_open_bracket,
                                  ui_view->pushButton_close_bracket,
                                  ui_view->pushButton_mod,
                                  ui_view->pushButton_pow,
                                  ui_view->pushButton_u_minus,
                                  ui_view->pushButton_u_plus,
                                  ui_view->pushButton_sqrt,
                                  ui_view->pushButton_symbol_x,
                                  ui_view->pushButton_e_plus,
                                  ui_view->pushButton_e_minus,
                                  ui_view->pushButton_ln,
                                  ui_view->pushButton_log,
                                  ui_view->pushButton_sin,
                                  ui_view->pushButton_cos,
                                  ui_view->pushButton_tan,
                                  ui_view->pushButton_asin,
                                  ui_view->pushButton_acos,
                                  ui_view->pushButton_atan,
                                  ui_view->pushButton_backspace,
                                  ui_view->pushButton_ac,
                                  ui_view->pushButton_equal};

  // connection of private slot with signals (button push)
  for (QPushButton *button : simpleButtons) {
    connect(button, &QPushButton::clicked, this, &View::expression_slot);
  }
  // connection of private graph slot with Plot graph button
  connect(ui_view->pushButton_graph, &QPushButton::clicked, this,
          &View::graph_slot);
  // connection of private graph slot with AC button for clean up the plot
  connect(ui_view->pushButton_ac, &QPushButton::clicked, this,
          &View::graph_slot);
}

View::~View() { delete ui_view; }

void View::expression_slot() {
  QPushButton *button = (QPushButton *)sender();
  controller->edit_variable(ui_view->x_value->text());
  controller->handle_button_pressed(button->text());
  expression = controller->expression_content();
  result = controller->result_content();
  setView();
}

void View::graph_slot() {
  double x_lo = ui_view->x_min->value();
  double x_hi = ui_view->x_max->value();
  double y_lo = ui_view->y_min->value();
  double y_hi = ui_view->y_max->value();
  ui_view->graph->xAxis->setRange(x_lo, x_hi);
  ui_view->graph->yAxis->setRange(y_lo, y_hi);
  int x_pix = ui_view->graph->xAxis->coordToPixel(1) -
              ui_view->graph->xAxis->coordToPixel(0);
  int y_pix = ui_view->graph->yAxis->coordToPixel(0) -
              ui_view->graph->yAxis->coordToPixel(1);
  graphs = controller->graph_content(x_lo, x_hi, x_pix, y_lo, y_hi, y_pix);
  result = controller->result_content();
  setView();
}

void View::setView() {
  ui_view->expres_label->setText(expression);
  ui_view->result_label->setText(result);

  ui_view->graph->clearGraphs();
  ui_view->graph->replot();
  if (!graphs.empty()) {
    for (int i = 0; i < graphs.size(); i++) {
      ui_view->graph->addGraph();
      QVector<double> x = QVector<double>::fromList(graphs[i].keys());
      QVector<double> y = QVector<double>::fromList(graphs[i].values());
      ui_view->graph->graph(i)->setData(x, y);
    }
    ui_view->graph->replot();
  }
}

}  // namespace scn
