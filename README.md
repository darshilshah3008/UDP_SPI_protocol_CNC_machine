# Ethernet-to-SPI CNC Machine Control (ENC28J60 + UDP)

This project implements bidirectional communication between a **PC (UDP over Ethernet)** and a **CNC motion-control system over SPI**, using the **ENC28J60 10Base-T Ethernet controller**.  
The system receives Ethernet packets, parses them, forwards commands to motion/file-system modules over SPI, and returns results back to the PC.  
A custom **ping-pong buffering mechanism** ensures non-blocking operation and prevents data overwrite while both motion control and file-system tasks are active.

---

## ✅ Features

- UDP-based communication from PC to embedded controller
- ENC28J60 Ethernet controller operating via SPI
- SPI-based communication with motion control + file-system modules
- Ping-pong buffering to prevent data corruption
- ARP + ICMP handling
- Command routing based on UDP ports
- File upload & directory commands supported
- CNC jog, calibration, position query, start/stop control

---

## 🛠 System Overview

The PC sends UDP packets → ENC28J60 receives data → data is parsed → commands are routed to the appropriate subsystem over SPI → responses are forwarded back to the PC.

The ENC28J60 handles:
- ARP negotiation
- ICMP ping
- Incoming/outgoing UDP payloads  


Since UDP packets arrive in network byte order (big-endian), appropriate conversion (`htons`/`ntohs`) is required before command execution.  


---

## 🔌 Communication Ports

| Port | Function |
|------|----------|
| **1024** | File-system command |
| **1060** | CNC / motion-control command |
| **1080** | File-system data channel |
| **1025** | General PC listen port |


A helper routine **`FindDestinationPort()`** extracts the destination port from UDP packets to route commands properly.  


---

## 🔁 Ping-Pong Buffering

A dual-buffer (2-D array) system prevents collisions between motion-control and file-system responses.  
If one subsystem is busy, the other can still process incoming commands safely.  


This design eliminates packet-overwrites that occurred with single-buffer handling.

---

## ✨ Supported CNC Commands

| Command | Function |
|---------|----------|
| `01` | Start |
| `02` | Pause |
| `05` | Calibration |
| `06` | Jog |
| `07` | Report X,Y,Z position |
| `0B` | Stop |


---

## 📁 File-System Commands

| Command | Hex | Purpose |
|---------|-----|---------|
| Status % | `08` | File completion percentage |
| Upload   | `09` | Upload file to storage |
| Dir      | `0A` | Directory list |
| Data     | `09` (1080) | Write data block |


A parse-field routine handles file-metadata extraction and supports acknowledgment messages back to the PC.  


---

## 📡 ENC28J60 Hardware Interface

| ENC28J60 Pin | MCU Pin |
|--------------|--------|
| SI (SPI-TX)  | PA5 |
| SO (SPI-RX)  | PA4 |
| SCLK         | PA2 |
| CS           | PA3 |
| VCC/GND      | Power |


Memory allocation:
- `0x0000 – 0x1A0A` → RX buffer
- `0x1A0A – 0x1FFF` → TX buffer  


Supports storing up to ~3 PC packets at once.

---

## 🧠 Development Notes

### Key Learnings
- Big-endian network conversion required (`ntohs`)  

- Wireshark used to debug and inspect packet flow  

- Linux command-line tools used for packet testing  

- Buffer cleanup required (`memset`) to avoid stale data  

- Max buffer increased from 512 → 513 bytes to avoid hang  

- UDP ⇄ SPI routing fundamentals mastered  


---

## 🧩 Task Summary

| Task | Duration |
|------|----------|
| Port decoding + `ntohs/htons` | 1 week |
| Wireshark + data validation | 1 week |
| Destination port identification | 1 week |
| PC interface + testing | 1 week |
| Motion-control SPI integration | 1 week |
| Ping-pong buffer + troubleshooting | 1 week |
| File-system SPI integration | 1 week |


---

## Continuous Integration (Jenkins)

This project includes a CI pipeline built with Jenkins.

- Jenkins runs inside Docker
- ARM Cortex-M4 firmware is built using `arm-none-eabi-gcc`
- A dedicated CI Makefile (`ci/Makefile`) is used to decouple
  IDE-specific build artifacts from CI builds
- Builds are automatically triggered via SCM polling
- Build artifacts (`.elf`, `.bin`, `.map`) are archived

---
## ✅ Conclusion

This project successfully demonstrates:
- UDP-to-SPI bridging
- ENC28J60 Ethernet interface
- CNC + filesystem command routing
- Full request/response cycle with data validation
- Robust buffering to prevent blocking
- Real-time motion control + file handling
