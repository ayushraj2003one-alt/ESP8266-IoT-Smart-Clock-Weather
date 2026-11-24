<div align="center">

# ⏰ ESP8266 Wi-Fi Smart Clock & Weather Station
### Embedded IoT Weather Telemetry, DS1307 Real-Time Clock, and Local Web Dashboard

[![Microcontroller](https://img.shields.io/badge/MCU-ESP8266%20NodeMCU-E7352C?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B%20%2F%20Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Build Tool](https://img.shields.io/badge/Framework-PlatformIO%20%7C%20Arduino%20IDE-F16436?style=for-the-badge&logo=platformio&logoColor=white)](https://platformio.org/)
[![Display](https://img.shields.io/badge/Display-0.96%22%20OLED%20(SSD1306)-00599C?style=for-the-badge)](https://www.adafruit.com/product/326)
[![Sensors](https://img.shields.io/badge/Sensors-DHT11%20%2B%20DS1307%20RTC-4B6A9B?style=for-the-badge)](#pinout--wiring-guide)
[![Protocol](https://img.shields.io/badge/Protocol-I2C%20%2B%201--Wire%20%2B%20HTTP-8A2BE2?style=for-the-badge)](#hardware-architecture)
[![Network](https://img.shields.io/badge/WiFi%20Mode-SoftAP%20(Access%20Point)-success?style=for-the-badge)](#web-dashboard--time-calibration)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)

<br/>

<p align="center">
  <b>A complete embedded IoT hardware & firmware project built on the ESP8266 NodeMCU.</b><br/>
  Features a standalone Wi-Fi Access Point (SoftAP), dynamic HTTP web dashboard, 0.96" I2C OLED display, battery-backed DS1307 RTC, DHT11 environmental sensing, and a non-blocking asynchronous timing loop.
</p>

</div>

---

## 🛠️ Technology Stack & Hardware Components

<table>
  <tr>
    <td align="center" width="25%"><b>Component</b></td>
    <td align="center"><b>Technologies & Colorful Cards</b></td>
  </tr>
  <tr>
    <td align="center"><b>Microcontroller</b></td>
    <td>
      <img src="https://img.shields.io/badge/ESP8266-NodeMCU%20v2%20%2F%20ESP--12E-E7352C?style=for-the-badge&logo=espressif&logoColor=white" alt="ESP8266" />
      <img src="https://img.shields.io/badge/Clock-80%20%2F%20160%20MHz-blue?style=for-the-badge" alt="Clock" />
      <img src="https://img.shields.io/badge/Flash-4MB%20SPI-purple?style=for-the-badge" alt="Flash" />
    </td>
  </tr>
  <tr>
    <td align="center"><b>Firmware & Toolchain</b></td>
    <td>
      <img src="https://img.shields.io/badge/C%2B%2B-Embedded%20C%2B%2B11-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++" />
      <img src="https://img.shields.io/badge/Arduino-Core%20for%20ESP8266-00979D?style=for-the-badge&logo=arduino&logoColor=white" alt="Arduino" />
      <img src="https://img.shields.io/badge/PlatformIO-Compatible-F16436?style=for-the-badge&logo=platformio&logoColor=white" alt="PlatformIO" />
    </td>
  </tr>
  <tr>
    <td align="center"><b>Peripherals & Displays</b></td>
    <td>
      <img src="https://img.shields.io/badge/OLED-SSD1306%20128x64%20(I2C%200x3C)-00599C?style=for-the-badge" alt="OLED" />
      <img src="https://img.shields.io/badge/RTC-DS1307%20Real--Time%20Clock-FF8C00?style=for-the-badge" alt="DS1307" />
      <img src="https://img.shields.io/badge/Climate-DHT11%20Temp%20%26%20Humidity-239120?style=for-the-badge" alt="DHT11" />
    </td>
  </tr>
  <tr>
    <td align="center"><b>Protocols & Bus Interfaces</b></td>
    <td>
      <img src="https://img.shields.io/badge/Bus-I2C%20(Two--Wire)-4B6A9B?style=for-the-badge" alt="I2C" />
      <img src="https://img.shields.io/badge/GPIO-1--Wire%20Protocol-107C41?style=for-the-badge" alt="1-Wire" />
      <img src="https://img.shields.io/badge/Network-802.11%20b%2Fg%2Fn%20Wi--Fi-success?style=for-the-badge" alt="WiFi" />
      <img src="https://img.shields.io/badge/Web-HTTP%20REST%20Server-blueviolet?style=for-the-badge" alt="HTTP" />
    </td>
  </tr>
  <tr>
    <td align="center"><b>Web Stack</b></td>
    <td>
      <img src="https://img.shields.io/badge/Frontend-HTML5%20%2B%20CSS3%20Mobile%20Responsive-E34F26?style=for-the-badge&logo=html5&logoColor=white" alt="HTML5" />
      <img src="https://img.shields.io/badge/Server-ESP8266WebServer-orange?style=for-the-badge" alt="WebServer" />
    </td>
  </tr>
</table>

---

## 🔌 Pinout & Wiring Guide

The ESP8266 NodeMCU communicates with the OLED display and the DS1307 RTC over a **shared I2C bus** (`SCL` on `D1`, `SDA` on `D2`), while the DHT11 sensor connects over a dedicated single-wire digital interface (`D5`).

```mermaid
graph LR
    subgraph ESP8266 [ESP8266 NodeMCU]
        3V3[3.3V Power]
        GND[Ground GND]
        D1[Pin D1 / GPIO5: I2C SCL]
        D2[Pin D2 / GPIO4: I2C SDA]
        D5[Pin D5 / GPIO14: Digital]
    end

    subgraph OLED [0.96" OLED Display SSD1306]
        O_VCC[VCC]
        O_GND[GND]
        O_SCL[SCL]
        O_SDA[SDA]
    end

    subgraph RTC [DS1307 RTC Module]
        R_VCC[VCC]
        R_GND[GND]
        R_SCL[SCL]
        R_SDA[SDA]
    end

    subgraph DHT [DHT11 Sensor]
        D_VCC[VCC]
        D_GND[GND]
        D_DATA[DATA]
    end

    3V3 --- O_VCC
    3V3 --- R_VCC
    3V3 --- D_VCC
    GND --- O_GND
    GND --- R_GND
    GND --- D_GND

    D1 --- O_SCL
    D1 --- R_SCL

    D2 --- O_SDA
    D2 --- R_SDA

    D5 --- D_DATA

    style ESP8266 fill:#1565C0,stroke:#fff,stroke-width:2px,color:#fff
    style OLED fill:#00838F,stroke:#fff,stroke-width:2px,color:#fff
    style RTC fill:#E65100,stroke:#fff,stroke-width:2px,color:#fff
    style DHT fill:#2E7D32,stroke:#fff,stroke-width:2px,color:#fff
```

### Complete Hardware Connection Table

| Module | Module Pin | ESP8266 Pin | GPIO Number | Function / Interface |
| :--- | :--- | :--- | :--- | :--- |
| **0.96" OLED** | `VCC` | `3V3` | - | 3.3V Power Supply |
| | `GND` | `GND` | - | Common Ground |
| | `SCL` | `D1` | `GPIO5` | I2C Clock Line (Shared) |
| | `SDA` | `D2` | `GPIO4` | I2C Data Line (Shared) |
| **DS1307 RTC** | `VCC` | `3V3` or `Vin` | - | 3.3V or 5V Power Supply |
| | `GND` | `GND` | - | Common Ground |
| | `SCL` | `D1` | `GPIO5` | I2C Clock Line (Shared) |
| | `SDA` | `D2` | `GPIO4` | I2C Data Line (Shared) |
| **DHT11 Sensor** | `VCC` | `3V3` | - | 3.3V Power Supply |
| | `GND` | `GND` | - | Common Ground |
| | `DATA / OUT` | `D5` | `GPIO14` | Digital Single-Wire Data Line |

---

## 🌟 Key Highlights & Enhancements

- **📡 Standalone Wi-Fi Access Point (SoftAP)**: Broadcasts its own private network (`SSID: ESP-Clock`). No router or external internet connection is required.
- **⚡ Non-Blocking `millis()` Timing**: Eliminates conventional blocking `delay(2000)` calls, allowing the web server to handle client requests with near-zero latency.
- **🖥️ 0.96" OLED Graphical UI**: Real-time multi-line display rendering temperature, humidity, time, and calendar date.
- **🌐 Responsive Embedded Web Dashboard**: Dark-mode web interface served directly from the ESP8266 flash, allowing users to view real-time telemetry from any phone or computer browser.
- **⏱️ Web-Based RTC Calibration**: Adjust time and date directly through HTML5 `<input type="time">` and `<input type="date">` forms without re-flashing firmware.
- **🔋 Battery-Backed Clock Persistence**: The DS1307 RTC retains accurate calendar time via a CR2032 coin battery even during complete power loss.

---

## 🌐 Web Dashboard & Endpoints

When your smartphone or PC connects to the **`ESP-Clock`** Wi-Fi network, navigating to **`http://192.168.4.1/`** opens the dashboard:

| Endpoint | HTTP Method | Function |
| :--- | :--- | :--- |
| `/` | `GET` | Displays live temperature, humidity, RTC time, date, and calibration forms |
| `/setTime` | `GET` | Updates DS1307 hours and minutes via URL parameter `?time=HH:MM` |
| `/setDate` | `GET` | Updates DS1307 year, month, and day via URL parameter `?date=YYYY-MM-DD` |

---

## 📁 Repository Organization

```text
ESP8266-IoT-Smart-Clock-Weather/
├── 📁 src/
│   └── main.cpp                      # Core firmware with non-blocking timing loop
├── 📁 arduino/
│   └── 📁 ESP8266_Clock_Weather/
│       └── ESP8266_Clock_Weather.ino # Arduino IDE ready sketch
├── 📁 docs/                          # Circuit schematics & hardware documentation
├── 📄 platformio.ini                 # PlatformIO project configuration & dependencies
├── 📄 .gitignore                     # Build cache & artifact exclusions
└── 📄 README.md                      # Project documentation with cards & pinouts
```

---

## 🚀 How to Build & Flash

### Method 1: Using PlatformIO (VS Code) — Recommended
1. Install the **PlatformIO IDE** extension in VS Code.
2. Open the project root directory.
3. Connect your NodeMCU to your computer via USB.
4. Click the **PlatformIO: Upload** button (or run `pio run -t upload` in the terminal).

### Method 2: Using the Arduino IDE
1. Open the Arduino IDE.
2. Add ESP8266 Board Manager URL in **File > Preferences**:
   ```text
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Install the **ESP8266** board package from **Tools > Board > Boards Manager**.
4. Install required libraries via **Sketch > Include Library > Manage Libraries**:
   - `Adafruit SSD1306`
   - `Adafruit GFX Library`
   - `DHT sensor library`
   - `RTClib` (by Adafruit)
5. Select **NodeMCU 1.0 (ESP-12E Module)** under **Tools > Board**.
6. Open `arduino/ESP8266_Clock_Weather/ESP8266_Clock_Weather.ino` and click **Upload**.

---

## 🎓 Viva & Technical Interview Notes

<details>
<summary><b>Click to expand Viva Q&A Cheat Sheet</b></summary>
<br/>

1. **How can the OLED display and the DS1307 RTC share the exact same pins (D1 & D2)?**
   - The I2C protocol uses **unique 7-bit bus addresses**. The SSD1306 OLED responds to address `0x3C`, while the DS1307 RTC responds to address `0x68`. The master (ESP8266) addresses each peripheral independently over the shared SDA/SCL lines.

2. **Why is using `delay(2000)` bad practice in an IoT web server loop?**
   - `delay()` is a blocking call that freezes the CPU. If an HTTP request arrives while the microcontroller is blocked in `delay()`, the request will either time out or experience severe lag. Replacing `delay()` with an asynchronous `millis()` timer allows `server.handleClient()` to process requests continuously.

3. **What is the difference between Wi-Fi SoftAP and Station (STA) mode?**
   - In **Station (STA) mode**, the ESP8266 connects as a client to an existing home or office Wi-Fi router. In **SoftAP (Access Point) mode**, the ESP8266 acts as its own independent router, broadcasting an SSID and assigning IP addresses to connected devices without needing an internet connection.

4. **Why does the DS1307 require an external crystal oscillator?**
   - The DS1307 uses an external $32.768\text{ kHz}$ quartz crystal. $32768 = 2^{15}$, which allows binary frequency divider flip-flops to divide the frequency exactly down to $1\text{ Hz}$ ($1\text{ second}$) with low power consumption.

</details>

---

<div align="center">
  <sub>Developed by Ayush Raj • Built with ❤️ for Embedded Systems & IoT</sub>
</div>
