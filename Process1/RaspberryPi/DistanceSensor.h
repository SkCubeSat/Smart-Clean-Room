#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

class DistanceSensor {
    private:
    int trig, echo;

    public:
    DistanceSensor(int trigPin, int echoPin);
    float getDistance();
};

#endif