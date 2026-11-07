#include <FastLED.h>

// --- HARDWARE CONFIGURATION ---
#define NUM_LEDS     55
#define DATA_PIN     6       // Data pin for WS2812B LED strip
#define BTN_RED_PIN   2       // RED Button
#define BTN_GREEN_PIN 3       // GREEN Button
#define BTN_BLUE_PIN  4       // BLUE Button

#define MAX_BRIGHTNESS 64    // Brightness limit (64 / 255 = ~25%, safe for USB)
#define TOTAL_GAMES 9        // Total number of games

CRGB leds[NUM_LEDS];

// Game State Machine
enum GameState {
  STATE_MENU,
  STATE_COUNTDOWN,
  STATE_PLAYING,
  STATE_GAME_OVER
};

GameState currentState = STATE_MENU;
uint8_t selectedGame = 1;
uint32_t finalScore = 0;

// --- BUTTON HELPER FUNCTIONS ---
bool isButtonPressed(uint8_t pin) {
  if (digitalRead(pin) == LOW) {
    delay(20); // Debounce delay
    if (digitalRead(pin) == LOW) {
      while (digitalRead(pin) == LOW); // Wait for release
      return true;
    }
  }
  return false;
}

uint8_t getPressedButtonsMask() {
  uint8_t mask = 0;
  if (digitalRead(BTN_RED_PIN) == LOW)   mask |= 0x01;
  if (digitalRead(BTN_GREEN_PIN) == LOW) mask |= 0x02;
  if (digitalRead(BTN_BLUE_PIN) == LOW)  mask |= 0x04;
  return mask;
}

CRGB getColorFromIndex(uint8_t idx) {
  switch (idx) {
    case 0: return CRGB::Red;
    case 1: return CRGB::Green;
    case 2: return CRGB::Blue;
    case 3: return CRGB::Yellow;    // R + G
    case 4: return CRGB::Magenta;   // R + B
    case 5: return CRGB::Cyan;      // G + B
    default: return CRGB::White;
  }
}

// --- SETUP ---
void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  
  // Power management limit: 5V and max 500mA for USB-C
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);

  pinMode(BTN_RED_PIN, INPUT_PULLUP);
  pinMode(BTN_GREEN_PIN, INPUT_PULLUP);
  pinMode(BTN_BLUE_PIN, INPUT_PULLUP);

  randomSeed(analogRead(A0));
  FastLED.clear();
  FastLED.show();
}

// --- MAIN LOOP ---
void loop() {
  switch (currentState) {
    case STATE_MENU:
      runMenu();
      break;
    case STATE_COUNTDOWN:
      runCountdown();
      break;
    case STATE_PLAYING:
      launchSelectedGame(selectedGame);
      break;
    case STATE_GAME_OVER:
      runGameOverEffect();
      currentState = STATE_MENU;
      break;
  }
}

// ==========================================
// MENU, COUNTDOWN & EFFECTS
// ==========================================

void runMenu() {
  static unsigned long lastPulse = 0;
  static uint8_t pulseVal = 50;
  static int8_t pulseDir = 5;

  if (millis() - lastPulse > 15) {
    lastPulse = millis();
    pulseVal += pulseDir;
    if (pulseVal <= 40 || pulseVal >= 250) pulseDir = -pulseDir;
  }

  FastLED.clear();
  for (uint8_t i = 0; i < TOTAL_GAMES; i++) {
    if (i == (selectedGame - 1)) {
      leds[i] = CRGB(pulseVal, pulseVal, 0); // Selected game pulses yellow
    } else {
      leds[i] = CRGB(0, 0, 20);             // Other games glow dim blue
    }
  }
  FastLED.show();

  // Controls: Red = Previous | Green = Next | Blue = Confirm
  if (isButtonPressed(BTN_RED_PIN)) {
    selectedGame = (selectedGame > 1) ? selectedGame - 1 : TOTAL_GAMES;
  }
  if (isButtonPressed(BTN_GREEN_PIN)) {
    selectedGame = (selectedGame < TOTAL_GAMES) ? selectedGame + 1 : 1;
  }
  if (isButtonPressed(BTN_BLUE_PIN)) {
    currentState = STATE_COUNTDOWN;
  }
}

void runCountdown() {
  FastLED.clear();
  FastLED.show();

  for (int step = 3; step >= 1; step--) {
    FastLED.clear();
    for (int i = 0; i < step * 4; i++) {
      leds[i] = CRGB::Red;
    }
    FastLED.show();
    delay(500);
    FastLED.clear();
    FastLED.show();
    delay(200);
  }

  fill_solid(leds, 20, CRGB::Green);
  FastLED.show();
  delay(300);
  FastLED.clear();
  FastLED.show();

  currentState = STATE_PLAYING;
}

void runGameOverEffect() {
  // Flash red to indicate game over
  for (int i = 0; i < 3; i++) {
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    delay(120);
    FastLED.clear();
    FastLED.show();
    delay(120);
  }

  // Display score bar using green LEDs
  FastLED.clear();
  uint16_t scoreLeds = min((uint16_t)finalScore, (uint16_t)NUM_LEDS);
  for (uint16_t i = 0; i < scoreLeds; i++) {
    leds[i] = CRGB::Green;
  }
  FastLED.show();
  delay(1500);
}

void launchSelectedGame(uint8_t gameId) {
  finalScore = 0;
  switch (gameId) {
    case 1: playGame1_TargetShooter(); break;
    case 2: playGame2_ColorMixer(); break;
    case 3: playGame3_ReflexPingPong(); break;
    case 4: playGame4_SimonSays(); break;
    case 5: playGame5_LaserDodger(); break;
    case 6: playGame6_BombDefuse(); break;
    case 7: playGame7_SpeedDuel(); break;
    case 8: playGame8_GuitarHero(); break;
    case 9: playGame9_TugOfWar(); break;
  }
}

// ==========================================
// GAME 1: TARGET SHOOTER
// ==========================================
void playGame1_TargetShooter() {
  int targetPos = NUM_LEDS - 1;
  uint8_t targetColorIndex = random(0, 3);
  CRGB targetColor = getColorFromIndex(targetColorIndex);

  unsigned long lastMoveTime = millis();
  int moveInterval = 120;

  while (true) {
    if (millis() - lastMoveTime > moveInterval) {
      lastMoveTime = millis();
      targetPos--;
      if (targetPos < 0) { currentState = STATE_GAME_OVER; return; }
    }

    FastLED.clear();
    leds[targetPos] = targetColor;
    FastLED.show();

    int pressed = -1;
    if (digitalRead(BTN_RED_PIN) == LOW)   pressed = 0;
    if (digitalRead(BTN_GREEN_PIN) == LOW) pressed = 1;
    if (digitalRead(BTN_BLUE_PIN) == LOW)  pressed = 2;

    if (pressed != -1) {
      CRGB shotColor = getColorFromIndex(pressed);
      for (int i = 0; i <= targetPos; i++) {
        FastLED.clear();
        leds[targetPos] = targetColor;
        leds[i] = shotColor;
        FastLED.show();
        delay(6);
      }

      if (pressed == targetColorIndex) {
        finalScore++;
        if (moveInterval > 25) moveInterval -= 4;
        targetPos = NUM_LEDS - 1;
        targetColorIndex = random(0, 3);
        targetColor = getColorFromIndex(targetColorIndex);
        delay(100);
      } else {
        currentState = STATE_GAME_OVER;
        return;
      }
    }
  }
}

// ==========================================
// GAME 2: COLOR MIXER
// ==========================================
void playGame2_ColorMixer() {
  int targetPos = NUM_LEDS - 1;
  uint8_t targetType = random(0, 6); // 0-2 Primary colors, 3-5 Mixed colors
  CRGB targetColor = getColorFromIndex(targetType);

  unsigned long lastMoveTime = millis();
  int moveInterval = 140;

  while (true) {
    if (millis() - lastMoveTime > moveInterval) {
      lastMoveTime = millis();
      targetPos--;
      if (targetPos < 0) { currentState = STATE_GAME_OVER; return; }
    }

    FastLED.clear();
    leds[targetPos] = targetColor;
    FastLED.show();

    uint8_t mask = getPressedButtonsMask();
    if (mask != 0) {
      delay(40); // Short window to register simultaneous dual-button press
      mask = getPressedButtonsMask();

      uint8_t shotType = 255;
      if (mask == 0x01) shotType = 0; // Red
      else if (mask == 0x02) shotType = 1; // Green
      else if (mask == 0x04) shotType = 2; // Blue
      else if (mask == 0x03) shotType = 3; // Yellow (R+G)
      else if (mask == 0x05) shotType = 4; // Magenta (R+B)
      else if (mask == 0x06) shotType = 5; // Cyan (G+B)

      if (shotType != 255) {
        CRGB shotColor = getColorFromIndex(shotType);
        for (int i = 0; i <= targetPos; i++) {
          FastLED.clear();
          leds[targetPos] = targetColor;
          leds[i] = shotColor;
          FastLED.show();
          delay(5);
        }

        if (shotType == targetType) {
          finalScore++;
          if (moveInterval > 35) moveInterval -= 4;
          targetPos = NUM_LEDS - 1;
          targetType = random(0, 6);
          targetColor = getColorFromIndex(targetType);
          while(getPressedButtonsMask() != 0); // Wait for button release
        } else {
          currentState = STATE_GAME_OVER;
          return;
        }
      }
    }
  }
}

// ==========================================
// GAME 3: REFLEX PING-PONG
// ==========================================
void playGame3_ReflexPingPong() {
  float ballPos = NUM_LEDS - 1;
  float ballSpeed = -0.4;
  uint8_t ballColorIdx = random(0, 3);

  while (true) {
    ballPos += ballSpeed;

    if (ballPos <= 0) { currentState = STATE_GAME_OVER; return; }
    if (ballPos >= NUM_LEDS - 1 && ballSpeed > 0) { ballSpeed = -ballSpeed; }

    FastLED.clear();
    // Bounce zone (first 3 LEDs)
    leds[0] = leds[1] = leds[2] = CRGB(15, 15, 15);
    leds[(int)ballPos] = getColorFromIndex(ballColorIdx);
    FastLED.show();

    int pressed = -1;
    if (digitalRead(BTN_RED_PIN) == LOW)   pressed = 0;
    if (digitalRead(BTN_GREEN_PIN) == LOW) pressed = 1;
    if (digitalRead(BTN_BLUE_PIN) == LOW)  pressed = 2;

    if (pressed != -1) {
      if (ballPos <= 4 && pressed == ballColorIdx && ballSpeed < 0) {
        ballSpeed = -ballSpeed * 1.08; // Accelerate ball after successful return
        ballColorIdx = random(0, 3);
        finalScore++;
        while (digitalRead(BTN_RED_PIN) == LOW || digitalRead(BTN_GREEN_PIN) == LOW || digitalRead(BTN_BLUE_PIN) == LOW);
      } else if (ballPos > 4) {
        currentState = STATE_GAME_OVER;
        return;
      }
    }
    delay(20);
  }
}

// ==========================================
// GAME 4: SIMON SAYS (MEMORY GAME)
// ==========================================
void playGame4_SimonSays() {
  uint8_t sequence[30];
  uint8_t seqLength = 0;

  while (seqLength < 30) {
    sequence[seqLength] = random(0, 3);
    seqLength++;

    // Playback sequence
    for (uint8_t i = 0; i < seqLength; i++) {
      FastLED.clear();
      fill_solid(leds, NUM_LEDS, getColorFromIndex(sequence[i]));
      FastLED.show();
      delay(350);
      FastLED.clear();
      FastLED.show();
      delay(150);
    }

    // Player repeats sequence
    for (uint8_t i = 0; i < seqLength; i++) {
      int pressed = -1;
      while (pressed == -1) {
        if (isButtonPressed(BTN_RED_PIN))   pressed = 0;
        if (isButtonPressed(BTN_GREEN_PIN)) pressed = 1;
        if (isButtonPressed(BTN_BLUE_PIN))  pressed = 2;
      }

      fill_solid(leds, NUM_LEDS, getColorFromIndex(pressed));
      FastLED.show();
      delay(150);
      FastLED.clear();
      FastLED.show();

      if (pressed != sequence[i]) {
        currentState = STATE_GAME_OVER;
        return;
      }
    }
    finalScore = seqLength;
    delay(500);
  }
}

// ==========================================
// GAME 5: LASER DODGER
// ==========================================
void playGame5_LaserDodger() {
  int playerShield = 0; // 0=Red, 1=Green, 2=Blue
  int enemyPos = NUM_LEDS - 1;
  int enemyColorIdx = random(0, 3);
  unsigned long lastMove = millis();
  int speed = 70;

  while (true) {
    if (isButtonPressed(BTN_RED_PIN))   playerShield = 0;
    if (isButtonPressed(BTN_GREEN_PIN)) playerShield = 1;
    if (isButtonPressed(BTN_BLUE_PIN))  playerShield = 2;

    if (millis() - lastMove > speed) {
      lastMove = millis();
      enemyPos--;

      if (enemyPos == 0) {
        if (enemyColorIdx == playerShield) {
          finalScore++;
          enemyPos = NUM_LEDS - 1;
          enemyColorIdx = random(0, 3);
          if (speed > 20) speed -= 2;
        } else {
          currentState = STATE_GAME_OVER;
          return;
        }
      }
    }

    FastLED.clear();
    leds[0] = getColorFromIndex(playerShield); // Player shield
    leds[enemyPos] = getColorFromIndex(enemyColorIdx); // Incoming obstacle
    FastLED.show();
    delay(10);
  }
}

// ==========================================
// GAME 6: BOMB DEFUSE
// ==========================================
void playGame6_BombDefuse() {
  int bombPos = 0;
  int targetLed = random(10, 45);
  int targetColorIdx = random(0, 3);
  unsigned long lastTick = millis();

  while (true) {
    if (millis() - lastTick > 100) {
      lastTick = millis();
      bombPos++;
      if (bombPos >= NUM_LEDS) { currentState = STATE_GAME_OVER; return; }
    }

    FastLED.clear();
    for (int i = 0; i < bombPos; i++) leds[i] = CRGB::Red; // Bomb fuse fills up
    leds[targetLed] = getColorFromIndex(targetColorIdx); // Target disarm LED
    FastLED.show();

    int pressed = -1;
    if (isButtonPressed(BTN_RED_PIN))   pressed = 0;
    if (isButtonPressed(BTN_GREEN_PIN)) pressed = 1;
    if (isButtonPressed(BTN_BLUE_PIN))  pressed = 2;

    if (pressed != -1) {
      if (abs(bombPos - targetLed) <= 2 && pressed == targetColorIdx) {
        finalScore++;
        bombPos = max(0, bombPos - 15); // Disarming pushes fuse back
        targetLed = random(10, 45);
        targetColorIdx = random(0, 3);
      } else {
        currentState = STATE_GAME_OVER;
        return;
      }
    }
  }
}

// ==========================================
// GAME 7: SPEED DUEL
// ==========================================
void playGame7_SpeedDuel() {
  FastLED.clear(); FastLED.show();
  delay(random(2000, 5000)); // Random delay before signal

  uint8_t targetColorIdx = random(0, 3);
  fill_solid(leds, NUM_LEDS, getColorFromIndex(targetColorIdx));
  FastLED.show();

  unsigned long startTime = millis();
  while (true) {
    int pressed = -1;
    if (digitalRead(BTN_RED_PIN) == LOW)   pressed = 0;
    if (digitalRead(BTN_GREEN_PIN) == LOW) pressed = 1;
    if (digitalRead(BTN_BLUE_PIN) == LOW)  pressed = 2;

    if (pressed != -1) {
      unsigned long reactionTime = millis() - startTime;
      if (pressed == targetColorIdx) {
        // Score calculated based on reaction speed
        finalScore = (reactionTime < 1000) ? (1000 - reactionTime) / 20 : 1;
      } else {
        finalScore = 0;
      }
      currentState = STATE_GAME_OVER;
      return;
    }
  }
}

// ==========================================
// GAME 8: 1D GUITAR HERO (RHYTHM GAME)
// ==========================================
struct Note {
  int pos;
  int colorIdx;
  bool active;
};

void playGame8_GuitarHero() {
  Note notes[5];
  for (int i = 0; i < 5; i++) notes[i].active = false;

  unsigned long lastSpawn = millis();
  unsigned long lastMove = millis();

  while (true) {
    // Spawn notes
    if (millis() - lastSpawn > 1200) {
      lastSpawn = millis();
      for (int i = 0; i < 5; i++) {
        if (!notes[i].active) {
          notes[i].pos = NUM_LEDS - 1;
          notes[i].colorIdx = random(0, 3);
          notes[i].active = true;
          break;
        }
      }
    }

    // Move notes
    if (millis() - lastMove > 60) {
      lastMove = millis();
      for (int i = 0; i < 5; i++) {
        if (notes[i].active) {
          notes[i].pos--;
          if (notes[i].pos < 0) { currentState = STATE_GAME_OVER; return; }
        }
      }
    }

    FastLED.clear();
    leds[0] = CRGB::White; // Target hit zone
    for (int i = 0; i < 5; i++) {
      if (notes[i].active) leds[notes[i].pos] = getColorFromIndex(notes[i].colorIdx);
    }
    FastLED.show();

    int pressed = -1;
    if (isButtonPressed(BTN_RED_PIN))   pressed = 0;
    if (isButtonPressed(BTN_GREEN_PIN)) pressed = 1;
    if (isButtonPressed(BTN_BLUE_PIN))  pressed = 2;

    if (pressed != -1) {
      bool hit = false;
      for (int i = 0; i < 5; i++) {
        if (notes[i].active && notes[i].pos <= 3 && notes[i].colorIdx == pressed) {
          notes[i].active = false;
          finalScore++;
          hit = true;
          break;
        }
      }
      if (!hit) { currentState = STATE_GAME_OVER; return; }
    }
  }
}

// ==========================================
// GAME 9: TUG OF WAR (2 PLAYERS)
// ==========================================
void playGame9_TugOfWar() {
  int pos = NUM_LEDS / 2; // Light marker starts in center (LED 27)

  while (true) {
    FastLED.clear();
    leds[pos] = CRGB::Yellow;
    // Goal lines
    leds[0] = CRGB::Red;
    leds[NUM_LEDS - 1] = CRGB::Blue;
    FastLED.show();

    // Player 1 (Red Button) pulls left
    if (isButtonPressed(BTN_RED_PIN)) {
      pos--;
    }
    // Player 2 (Blue Button) pulls right
    if (isButtonPressed(BTN_BLUE_PIN)) {
      pos++;
    }

    // Check win conditions
    if (pos <= 0) {
      finalScore = 1; // Player 1 wins
      currentState = STATE_GAME_OVER;
      return;
    }
    if (pos >= NUM_LEDS - 1) {
      finalScore = NUM_LEDS; // Player 2 wins
      currentState = STATE_GAME_OVER;
      return;
    }
  }
}
