#include <FlexCAN_T4.h>

uint8_t emptyBytePad = 0xCC;
uint32_t ecmResponseId = 2024;
uint8_t currentDataService = 0x41;

CAN_message_t supportedPidsOneToThirtyTwoResponse = {
    .id = ecmResponseId,
    .buf = {6, currentDataService, 0, B10011000, B00011100, B10000000,
            B00010011, emptyBytePad}};
CAN_message_t supportedPidsThirtyThreeToSixtyFourResponse = {
    .id = ecmResponseId,
    .buf = {6, currentDataService, 32, B00000000, B00000000, B00000000,
            B00000001, emptyBytePad}};
CAN_message_t supportedPidsSixtyFiveToNinetySixResponse = {
    .id = ecmResponseId,
    .buf = {6, currentDataService, 64, B00000000, B00000000, B10000000,
            B00000001, emptyBytePad}};
CAN_message_t supportedPidsNinetySevenToOneHundredTwentyEightResponse = {
    .id = ecmResponseId,
    .buf = {6, currentDataService, 96, B00000000, B00000001, B00000000,
            B00000001, emptyBytePad}};
CAN_message_t supportedPidsOneHundredTwentyNineToOneHundredOneHundredFiftyResponse = {
    .id = ecmResponseId,
    .buf = {6, currentDataService, 96, B00000000, B00000000, B00000000,
            B00000001, emptyBytePad}};

CAN_message_t monitorStatusResponse = {
    .id = ecmResponseId,
    .buf = {7, currentDataService, 1, B00000000, B00001000, B00000000,
            B00000000, emptyBytePad}};
CAN_message_t engineLoadTempResponse = {
    .id = ecmResponseId,
    .buf = {4, currentDataService, 4, 0, emptyBytePad, emptyBytePad,
            emptyBytePad, emptyBytePad}};
CAN_message_t waterTempResponse = {
    .id = ecmResponseId,
    .buf = {4, currentDataService, 5, 0, emptyBytePad, emptyBytePad,
            emptyBytePad, emptyBytePad}};
CAN_message_t rpmResponse = {.id = ecmResponseId,
                             .buf = {5, currentDataService, 12, 0, 0,
                                     emptyBytePad, emptyBytePad, emptyBytePad}};
CAN_message_t speedResponse = {
    .id = ecmResponseId,
    .buf = {4, currentDataService, 13, 0, emptyBytePad, emptyBytePad,
            emptyBytePad, emptyBytePad}};

CAN_message_t timingResponse = {
    .id = ecmResponseId,
    .buf = {4, currentDataService, 14, 0, emptyBytePad, emptyBytePad,
            emptyBytePad, emptyBytePad}};

CAN_message_t throttleResponse = {
    .id = ecmResponseId,
    .buf = {4, currentDataService, 17, 0, emptyBytePad, emptyBytePad,
            emptyBytePad, emptyBytePad}};

CAN_message_t obdStandardResponse = {
    .id = ecmResponseId,
    .buf = {4, currentDataService, 28, 1, emptyBytePad, emptyBytePad,
            emptyBytePad, emptyBytePad}};

CAN_message_t runtimeResponse = {
    .id = ecmResponseId,
    .buf = {4, currentDataService, 31, 0, emptyBytePad, emptyBytePad,
            emptyBytePad, emptyBytePad}};

CAN_message_t fuelTypeResponse = {
    .id = ecmResponseId,
    .buf = {4, currentDataService, 81, 4, emptyBytePad, emptyBytePad,
            emptyBytePad, emptyBytePad}};

CAN_message_t oilTempResponse = {
    .id = ecmResponseId,
    .buf = {4, currentDataService, 92, 0, emptyBytePad, emptyBytePad,
            emptyBytePad, emptyBytePad}};

CAN_message_t boostResponse = {
    .id = ecmResponseId, .buf = {7, currentDataService, 112, 0, 0, 0, 0, 0}};

CAN_message_t oilResponse = {
    .id = ecmResponseId, .buf = {7, currentDataService, 253, 0, 0, 0, 0, 0}};

CAN_message_t manifoldResponse = {
    .id = ecmResponseId, .buf = {7, currentDataService, 254, 0, 0, 0, 0, 0}};

CAN_message_t transResponse = {
    .id = ecmResponseId, .buf = {7, currentDataService, 255, 0, 0, 0, 0, 0}};

CAN_message_t odometerResponse = {
    .id = ecmResponseId, .buf = {7, currentDataService, 166, 0, 0, 0, 0, 0}};

CAN_message_t tripAResponse = {
    .id = ecmResponseId, .buf = {7, currentDataService, 252, 0, 0, 0, 0, 0}};

CAN_message_t tripBResponse = {
    .id = ecmResponseId, .buf = {7, currentDataService, 251, 0, 0, 0, 0, 0}};