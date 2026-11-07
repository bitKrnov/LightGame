# 🕹️ LightGame – 1D LED Arcade on Arduino

**LightGame** is a 1D electronic arcade game built on an Arduino Nano microcontroller, an addressable WS2812B RGB LED strip, and 3 color buttons. The project features 9 integrated game modes (ranging from reflex games and memory challenges to 2-player duels) with built-in power safety management for standard USB-C power sources.

---

## ⚡ Main Features

* **9 Built-in Games:** Target Shooter, Color Mixer, Reflex Ping-Pong, Simon Says, Laser Dodger, Bomb Defuse, Speed Duel, Rhythm Game, and 2-Player Tug of War.
* **Visual Menu & Countdown:** Easy game selection using the buttons with visual feedback directly on the LED strip.
* **USB Power Safe:** Integrated power management ensures total current draw never exceeds $0.5\,\text{A}$ ($500\,\text{mA}$).
* **Simple Hardware Build:** Minimal component count, perfect for beginners and makers.

---

## 🛠️ Hardware Requirements

| Component | Description / Specification | Qty |
| :--- | :--- | :---: |
| **Microcontroller** | Arduino Nano (ATmega328P) | 1 |
| **LED Strip** | WS2812B / 5V / 60 LEDs/m (55 LEDs total) | 1 |
| **Buttons** | Push buttons (Red, Green, Blue) | 3 |
| **Resistor** | $330\,\Omega$ to $470\,\Omega$ (data line protection) | 1 |
| **Capacitor** | Electrolytic $1000\,\mu\text{F} / 6.3\text{V}+$ (power filter) | 1 |
| **Power Supply** | USB-C cable + 5V USB charger / port (min. 0.5A) | 1 |

---

## 📐 Wiring Diagram

```text
               +-------------------+
               |   Arduino Nano    |
               |                   |
 USB 5V ------>| 5V                |
 GND ----------| GND               |
               |                   |
               |  D2 (Pull-Up) <---|---- [RED Button]   ---- GND
               |  D3 (Pull-Up) <---|---- [GREEN Button] ---- GND
               |  D4 (Pull-Up) <---|---- [BLUE Button]  ---- GND
               |                   |
               |  D6 (Data) ------[ 330Ω Resistor ]----> WS2812B DIN
               +-------------------+

 Power Rails:
  5V  --------------------------------------------------+---> WS2812B VCC
                                                        |
 GND ---------------------------------------------------+---> WS2812B GND
                                                        |
                                            [ 1000µF Capacitor ]
```

---

## 💻 Setup and Installation

### 1. Software Requirements
* [Arduino IDE](https://www.arduino.cc/en/software) (version 1.8.x or 2.x)
* **FastLED** library (available via Library Manager)

### 2. Uploading the Code
1. Open **Arduino IDE**.
2. Install the FastLED library: Navigate to `Tools -> Manage Libraries...` (Ctrl+Shift+I) and search for **FastLED**.
3. Open the `LightGame.ino` sketch from this repository.
4. Connect the Arduino Nano via USB cable.
5. Select `Arduino Nano` as the Board (and `ATmega328P (Old Bootloader)` if needed) and pick the correct COM Port.
6. Click **Upload**.

---

## 🎮 Games & Controls

### Menu Navigation
* **Red Button:** Previous game
* **Green Button:** Next game
* **Blue Button:** Confirm game selection

---

### Game Modes Overview

1. **Target Shooter:** Color targets approach from the far end. Press the corresponding color button to fire a projectile and destroy the target.
2. **Color Mixer:** Advanced shooter variant. Requires pressing 2 buttons simultaneously to shoot combined color projectiles (e.g., Red + Green = Yellow).
3. **Reflex Ping-Pong:** Bounce the light ball back when it enters the bounce zone (first 3 LEDs) by pressing the matching color button.
4. **Simon Says:** Memorize the sequence of colored flashes and repeat it using the buttons.
5. **Laser Dodger:** Change your shield color to match the color of the incoming laser obstacle.
6. **Bomb Defuse:** Stop the expanding red zone by pressing the correct color button right when it passes the target LED.
7. **Speed Duel:** Pure reaction time test. Wait for the random full-strip flash and press the matching color button as quickly as possible.
8. **Rhythm Game (1D Guitar Hero):** Press the color button right as the falling note hits the target white LED.
9. **Tug of War (2 Players):** Head-to-head battle. Player 1 (Red button) and Player 2 (Blue button) mash their buttons to pull the central light point to their side.

---

## ⚙️ Power Management & Safety

WS2812B LEDs can draw up to $60\,\text{mA}$ per LED at full white brightness (RGB). For a 55-LED strip, the theoretical maximum draw could reach $3.3\,\text{A}$.

This project uses two safety mechanisms to safely run from standard USB ports ($5\,\text{V} / 0.5\,\text{A}$):
* **Global Brightness Limit:** `FastLED.setBrightness(64);` (caps brightness at ~25%).
* **Dynamic Current Limiter:** `FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);` dynamically calculates LED draw and limits peak consumption to $500\,\text{mA}$.

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).