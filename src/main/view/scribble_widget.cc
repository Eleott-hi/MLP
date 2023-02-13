#include "scribble_widget.h"

namespace s21 {

ScribbleWidget::ScribbleWidget(QWidget* parent) : QWidget(parent) {
  image_ = QImage(kImageWidth_, kImageHeight_, QImage::Format_RGB32);
  image_.fill(Qt::white);
}

bool ScribbleWidget::OpenImage(const QString& filename) {
  QImage loaded_image;
  if (!loaded_image.load(filename)) return false;

  image_ = loaded_image.scaled(kImageWidth_, kImageHeight_);
  update();
  emit updated(image_);
  return true;
}

void ScribbleWidget::mousePressEvent(QMouseEvent* event) {
  // start scribbling
  if (event->button() == Qt::LeftButton) {
    start_point_ = event->position().toPoint();
    scribbling_ = true;
  }

  // clear area
  if (event->button() == Qt::RightButton) {
    image_.fill(Qt::white);
    update();
    emit cleared();
  }
}

void ScribbleWidget::mouseMoveEvent(QMouseEvent* event) {
  if (scribbling_) DrawLineTo(event->position().toPoint());
}

void ScribbleWidget::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) scribbling_ = false;
}

void ScribbleWidget::paintEvent(QPaintEvent* event) {
  image_painter_.begin(this);
  QRect rect = event->rect();
  image_painter_.drawImage(rect, image_, rect);
  image_painter_.end();
}

void ScribbleWidget::DrawLineTo(const QPoint& end_point) {
  line_painter_.begin(&image_);
  line_painter_.setRenderHint(QPainter::SmoothPixmapTransform, true);
  line_painter_.setRenderHint(QPainter::Antialiasing, true);
  line_painter_.setPen(
      QPen(Qt::black, kPenWidth_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  line_painter_.drawLine(start_point_, end_point);
  line_painter_.end();

  start_point_ = end_point;
  update();
  emit updated(image_);
}

}  // namespace s21
