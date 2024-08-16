#include <pigpio.h>
#include <MFRC522.h>
#include "timer.h"
#include "rfidreader.h"  
#include <cstdio> 

#define RST_PIN 25    // Configurable, depending on your wiring
#define SS_PIN 8      // CE0 (Chip Enable)

MFRC522 mfrc522(SS_PIN, RST_PIN);

// List of valid UIDs as integers
int valid_uids[] = {
    041224,  // Example UID aka birthday 1
    040618   // Example UID aka birthday 2
};

int num_valid_uids = sizeof(valid_uids) / sizeof(valid_uids[0]);

void rfid_thread(bool &is_verified, bool &kill_flag) {
    // Initialize pigpio
    if (gpioInitialise() < 0) {
        printf("pigpio initialization failed\n");
        return;
    }

    mfrc522.PCD_Init();

    while (!kill_flag) {
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
            int scanned_uid = mfrc522.uid.uidByte[0]; // Simplified for now, will need to be more complex when real UID's are being used

            for (int i = 0; i < num_valid_uids; i++) {
                if (scanned_uid == valid_uids[i]) {
                    is_verified = true;
                    setFalseIn(is_verified, 30000); // Reset after 30 seconds
                    printf("Valid card detected!\n");
                    break;
                }
            }

            mfrc522.PICC_HaltA();
            mfrc522.PCD_StopCrypto1();
        }
    }

    gpioTerminate();
}
