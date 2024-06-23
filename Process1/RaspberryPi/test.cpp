#include <wiringPi.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#define TRIG 4  // GPIO23
#define ECHO 5  // GPIO24

using namespace cv;
using namespace std;

void setup() {
    wiringPiSetup();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    digitalWrite(TRIG, LOW);
    delay(30);
}

double getDistance() {
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    while (digitalRead(ECHO) == LOW);

    long startTime = micros();
    while (digitalRead(ECHO) == HIGH);
    long travelTime = micros() - startTime;

    // Calculate distance in cm
    double distance = travelTime / 58.0;
    return distance;
}

int main() {
    setup();
    VideoCapture cap(0);  // Open the default camera

    if (!cap.isOpened()) {
        cerr << "Error opening camera!" << endl;
        return -1;
    }

    Mat frame;
    while (true) {
        double distance = getDistance();
        cout << "Distance: " << distance << " cm" << endl;

        if (distance < 50) {  // Adjust the threshold as needed
            cout << "Motion detected! Capturing image..." << endl;
            cap >> frame;  // Capture a frame
            imwrite("motion_detected.jpg", frame);
            delay(2000);  // Wait for 2 seconds before next measurement
        }
        delay(1000);  // Measure every second
    }

    return 0;
}