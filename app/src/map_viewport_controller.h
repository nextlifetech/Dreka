#ifndef MAP_VIEWPORT_CONTROLLER_H
#define MAP_VIEWPORT_CONTROLLER_H

#include <QObject>

class Coordinate : public QObject {
  Q_OBJECT

  Q_PROPERTY(double latitude MEMBER latitude NOTIFY changed)
  Q_PROPERTY(double longitude MEMBER longitude NOTIFY changed)
  Q_PROPERTY(double height MEMBER height NOTIFY changed)
  Q_PROPERTY(bool valid READ isValid NOTIFY changed)

 public:
  explicit Coordinate(QObject* parent = nullptr);

  double latitude = qQNaN();
  double longitude = qQNaN();
  float height = 0;

  bool isValid() const;

 public slots:
  void invalidate();

 signals:
  void changed();
};

class MapViewportController : public QObject {
  Q_OBJECT

  Q_PROPERTY(Coordinate* cursorPosition MEMBER cursorPosition CONSTANT)
  Q_PROPERTY(Coordinate* position MEMBER position CONSTANT)

  Q_PROPERTY(float heading MEMBER heading NOTIFY headingChanged)
  Q_PROPERTY(float pitch MEMBER pitch NOTIFY pitchChanged)

  Q_PROPERTY(
      double metersInPixel MEMBER metersInPixel NOTIFY metersInPixelChanged)

 public:
  explicit MapViewportController(QObject* parent = nullptr);

  Coordinate* const cursorPosition;
  Coordinate* const position;

  float heading = qQNaN();
  float pitch = qQNaN();
  double metersInPixel = 0.0;

 public slots:
  void save();
  void restore();

 signals:
  void headingChanged();
  void pitchChanged();
  void metersInPixelChanged();

  void flyTo(double latitude, double longitude, float height, float heading,
             float pitch, float duration = 0.0);
  void lookTo(float heading, float pitch, float duration = 0.0);
};

#endif  // MAP_VIEWPORT_CONTROLLER_H
