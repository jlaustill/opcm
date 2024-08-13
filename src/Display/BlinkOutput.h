#ifndef SRC_DISPLAY_BLINKOUTPUT_H_
#define SRC_DISPLAY_BLINKOUTPUT_H_

//
// Create by jlaustill on 7/6/24
// Copyright 2024 Joshua Lawrence Austill

class BlinkOutput {
 public:
  static void initialize();
  static void blink();

 private:
  static uint32_t lastBlink;
};

#endif  // SRC_DISPLAY_BLINKOUTPUT_H_
