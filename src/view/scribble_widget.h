#ifndef MLP_SRC_VIEW_SCRIBBLE_WIDGET_H_
#define MLP_SRC_VIEW_SCRIBBLE_WIDGET_H_

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

class ScribbleWidget : public QWidget {
  Q_OBJECT

 public:
  ScribbleWidget(QWidget* parent = nullptr);
  ~ScribbleWidget() = default;
  ScribbleWidget(const ScribbleWidget&) = delete;
  ScribbleWidget(ScribbleWidget&&) = delete;
  ScribbleWidget& operator=(const ScribbleWidget&) = delete;
  ScribbleWidget& operator=(ScribbleWidget&&) = delete;

  bool OpenImage(const QString& filename);

 signals:
  void updated(QImage);
  void cleared();

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

 private:
  bool scribbling_ = false;
  QPoint start_point_;
  QImage image_;
  QPainter line_painter_, image_painter_;
  const int kPenWidth_ = 30;
  const int kImageWidth_ = 512, kImageHeight_ = 512;

  void DrawLineTo(const QPoint& end_point);
};

#endif  // MLP_SRC_VIEW_SCRIBBLE_WIDGET_H_
