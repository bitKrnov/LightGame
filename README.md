# 🕹️ LightGame – 1D LED Arcade na Arduinu

**LightGame** je elektronická 1D hra postavená na mikrokontroléru Arduino Nano, adresovatelném RGB LED pásku WS2812B a 3 barevných tlačítkách. Projekt nabízí 9 různých herních režimů (od postřehových her přes paměťovky až po duely pro dva hráče) s bezpečným napájením přímo z USB-C portu či běžné 5V nabíječky.

---

## ⚡ Hlavní vlastnosti

* **9 integrovaných her:** Klasická střelnice, míchání barev, reflexní ping-pong, paměťovka (Simon Says), vyhýbačka, časovaná bomba, duely rychlosti, rytmická hra a přetahovaná pro 2 hráče.
* **Vizuální Menu & Odpočet:** Snadný výběr hry pomocí tlačítek s vizuální indikací přímo na LED pásku.
* **Bezpečné napájení z USB:** Integrovaný power-management v kódu zaručuje, že proudový odběr nepřesáhne limit 0,5 A ($500\,\text{mA}$).
* **Jednoduché HW zapojení:** Minimální počet součástek, ideální pro začátečníky i pokročilé bastlíře.

---

## 🛠️ Hardwarové požadavky

| Součástka | Specifikace / Popis | Počet |
| :--- | :--- | :---: |
| **Mikrokolébka** | Arduino Nano (ATmega328P) | 1 ks |
| **LED pásek** | WS2812B / 5V / 60 LED/m (délka 55 LED) | 1 ks |
| **Tlačítka** | Barevná tlačítka (Červené, Zelené, Modré) | 3 ks |
| **Rezistor** | $330\,\Omega$ až $470\,\Omega$ (ochrana datového pinu) | 1 ks |
| **Kondenzátor** | Elektrolytický $1000\,\mu\text{F} / 6.3\text{V}+$ (filtrace napájení) | 1 ks |
| **Napájení** | USB-C kabel + 5V USB zdroj / port (min. 0,5 A) | 1 ks |

---

## 📐 Schéma zapojení

```text
               +-------------------+
               |   Arduino Nano    |
               |                   |
 USB 5V ------>| 5V                |
 GND ----------| GND               |
               |                   |
               |  D2 (Pull-Up) <---|---- [Tlačítko ČERVENÉ] ---- GND
               |  D3 (Pull-Up) <---|---- [Tlačítko ZELENÉ]  ---- GND
               |  D4 (Pull-Up) <---|---- [Tlačítko MODRÉ]   ---- GND
               |                   |
               |  D6 (Data) ------[ 330Ω Rezistor ]----> WS2812B DIN
               +-------------------+

 Napájecí větev:
  5V  --------------------------------------------------+---> WS2812B VCC
                                                        |
 GND ---------------------------------------------------+---> WS2812B GND
                                                        |
                                            [ 1000µF Kondenzátor ]
```

---

## 💻 Instalace a zprovoznění

### 1. Požadavky na software
* [Arduino IDE](https://www.arduino.cc/en/software) (verze 1.8.x nebo 2.x)
* Knihovna **FastLED** (dostupná přímo ve správci knihoven)

### 2. Postup nahrání kódu
1. Otevři **Arduino IDE**.
2. Nainstaluj knihovnu FastLED: `Nástroje -> Spravovat knihovny...` (Ctrl+Shift+I) a vyhledej **FastLED**.
3. Otevři soubor `LightGame.ino` z tohoto repozitáře.
4. Připoj desku Arduino Nano přes USB kabel.
5. V nabídce zvol desku `Arduino Nano` (případně procesor `ATmega328P (Old Bootloader)`) a správný port (COM).
6. Klikni na **Nahrát** (Upload).

---

## 🎮 Seznam her a ovládání

### Ovládání v Menu
* **Červené tlačítko:** Posun na předchozí hru
* **Zelené tlačítko:** Posun na následující hru
* **Modré tlačítko:** Potvrzení výběru hry

---

### Přehled jednotlivých her

1. **Střelnice (Target Shooter):** Z konce pásku se k vám blíží barevné terče. Stisknutím tlačítka odpovídající barvy vystřelíte projektil a terč sestřelíte.
2. **Míchání barev (Color Mixer):** Pokročilá varianta střelnice. Vyžaduje míchání barviv stisknutím 2 tlačítek naráz pro sestřelení složených barev (např. Červená + Zelená = Žlutá).
3. **Reflexní Ping-Pong:** Odrážejte kuličku v odrazové zóně (první 3 LED od hráče) stisknutím tlačítka barvy letící kuličky.
4. **Paměťovka (Simon Says):** Zapamatujte si sekvenci barevných bliknutí a přesně ji zopakujte stisknutím tlačítek.
5. **Laserový Běh (Dodger):** Měňte barvu svého štítu stiskem tlačítek tak, aby odpovídala barvě přijíždějící překážky.
6. **Časovaná Bomba (Defuse):** Zastavte plnící se červenou zónu stiskem správného tlačítka přesně ve chvíli, kdy odpočet protnou cílovou diodu.
7. **Rychloprst (Speed Duel):** Test čisté reakční doby. Počkejte na náhodný záblesk celého pásku a co nejrychleji stiskněte odpovídající tlačítko.
8. **Barevný Rytmus (1D Guitar Hero):** Stiskněte barevné tlačítko v okamžiku, kdy padající barevný tón dorazí k cílové bílé LED.
9. **Přetahovaná (Tug of War - 2 Hráči):** Souboj pro 2 hráče na jednom pásku. Hráč 1 (Červené tlačítko) a Hráč 2 (Modré tlačítko) soupeří v rychlosti mačkání o přetažení světelného bodu na svou stranu.

---

## ⚙️ Power Management & Bezpečnost

Adresovatelné LED pásky WS2812B mohou při plném jasu bílé barvy (RGB) odebírat až $60\,\text{mA}$ na jednu diodu. Při 55 LED diodách by teoretický maximální odběr dosáhl až $3{,}3\,\text{A}$.

V tomto projektu je napájení bezpečně dimenzováno pro standardní USB-C zdroje ($5\,\text{V} / 0{,}5\,\text{A}$) pomoci dvou mechanismů:
* **Globální limit jasu:** `FastLED.setBrightness(64);` (omezení jasu na 25 %).
* **Dynamický proudový limiter:** Funkce `FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);` průběžně přepočítává a limituje odběr proudové špičky na maximálně $500\,\text{mA}$.

---

## 📜 Licence

Tento projekt je poskytován pod licencí [MIT License](LICENSE).