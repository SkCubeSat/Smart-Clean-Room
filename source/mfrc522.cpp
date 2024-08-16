#include <MFRC522.h>
#include <cstdio>
#include "timer.h"  

#define RST_PIN 25    // Configurable, depending on your wiring
#define SS_PIN 8      // CE0 (Chip Enable)

MFRC522 mfrc522(SS_PIN, RST_PIN);

// List of valid UIDs as integers
int valid_uids[] = {
    041224,  // Example UID aka birthday 1
    040618   // Example UID aka birthday 2
};

int num_valid_uids = sizeof(valid_uids) / sizeof(valid_uids[0]);

bool valid_id_detected = false;

// Function to check if a scanned UID is valid
bool check_valid_id() {
    if (!mfrc522.PICC_IsNewCardPresent()) return false;
    if (!mfrc522.PICC_ReadCardSerial()) return false;

    int scanned_uid = mfrc522.uid.uidByte[0]; // Simplified for now, will have to change logic later when real uids are being used

    for (int i = 0; i < num_valid_uids; i++) {
        if (scanned_uid == valid_uids[i]) {
            return true;
        }
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return false;
}

void setup() {
    if (gpioInitialise() < 0) {
        printf("pigpio initialization failed\n");
        exit(1);
    }

    mfrc522.PCD_Init();
    printf("Waiting for an RFID card...\n");
}

void loop() {
    if (check_valid_id()) {
        valid_id = true;
        setFalseIn(valid_id, 30000); 
        printf("Valid card detected!\n");
    }
}

int main() {
    setup();
    while (true) {
        loop();
    }
    gpioTerminate();
    return 0;
}
