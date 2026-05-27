# 🏥 IoT Smart Patient Monitoring & Alert System

### Arduino-Based Healthcare Monitoring Prototype

This is an IoT patient monitoring system I built using an Arduino Uno for my INFO 4381 (Internet of Things) course. Everything was done in Tinkercad since we didn't have access to physical hardware, but the logic and behavior are exactly what you'd get with real sensors.

The system tracks patient temperature, heart rate, and movement, then decides if everything's fine or if we need to sound an alarm. It handles all the processing on the Arduino itself—no cloud stuff, no Wi-Fi, just edge computing.

---

# 📄 What's in this repo

- `README.md` — this file  
- `code.ino` — the Arduino sketch  
- `iotReport_Smart_Patient_Control_System.pdf` — full technical report if you want the details  

---


<img width="1103" height="759" alt="Screenshot 2026-05-26 at 5 17 44 PM" src="https://github.com/user-attachments/assets/8a4de6c2-2447-4880-882a-49a251976e4a" />


# ⚙️ What it actually does

We built this to simulate a hospital room monitoring setup.

Here's the basic idea:

- Temperature sensor checks if the patient is running too hot or too cold
- Heart rate is simulated with a potentiometer (not a real pulse sensor, but it works for demo purposes)
- PIR sensor detects if the patient is moving around when they shouldn't be

Based on those inputs, the system picks a state:

| State | What triggers it | What happens |
|---|---|---|
| Normal | Everything in range | Green LED, LCD says all good |
| Caution | Temp or BPM slightly off | Yellow LED, warning on LCD |
| Critical | Temp or BPM way off | Red LED + buzzer alarm |
| Double Critical | Both temp and BPM critical | Full alarm mode |
| Motion Lock | Movement detected | Servo locks the door (simulated) |

There's also:
- a nurse call button
- an ACK button to silence the buzzer

---

# 🛠️ Hardware we used (all simulated in Tinkercad)

- Arduino Uno
- I2C LCD (16x2)
- PIR motion sensor
- Analog temperature sensor
- Potentiometer (standing in for a heart rate sensor)
- RGB LED
- Buzzer
- Servo motor (for the door lock)

---

# 🧠 How the logic works

The code checks conditions in this order of priority:

1. Double critical (both temp and BPM critical)
2. Single critical (either temp or BPM critical)
3. Caution (either slightly high/low)
4. Nurse call button pressed
5. Motion detected → lock door
6. Normal state

So if someone hits the nurse call button during a critical alert, the critical alert still takes priority.

Motion lock is pretty low priority too—it won't override a real medical alert.

---

# 🧪 Test cases we ran

We tested 9 scenarios total:

1. Normal vitals — baseline, everything green  
2. Temperature critical — fever or hypothermia simulation  
3. BPM critical — heart rate way too high or low  
4. Double critical — both temp and BPM bad at once  
5. Temperature caution — borderline, not quite critical  
6. BPM caution — same idea for heart rate  
7. Motion lock — PIR triggers, servo moves  
8. ACK button — silences the buzzer during an alarm  
9. Nurse call — button press registers  

All passed in simulation.

---

# 📷 What the simulation looks like

When temperature is in caution range, you get:

- LCD showing the caution message
- Yellow LED on
- No buzzer (since it's not critical)

![IoT Smart Patient Monitoring System](https://github.com/user-attachments/assets/4fffee30-c828-4b8c-acb2-9ebd1827b6b3)

---

# 🏗️ Architecture

We mapped this project to a standard 7-layer IoT architecture:

1. Physical devices  
2. Connectivity  
3. Edge computing  
4. Data accumulation  
5. Data abstraction  
6. Application  
7. Collaboration & processes  

Right now, layers 4–7 don't really exist since there's no cloud or backend.

That was the scope of this project — keep it local and make sure the edge logic is solid.

---

# 🚀 Future stuff we'd add

If we had more time or a bigger budget:

- Swap the Uno for an ESP32 and add Wi-Fi
- Hook it up to AWS IoT Core or Azure IoT Hub
- Build a mobile app so nurses can check patients remotely
- Store historical data somewhere
- Add a real pulse sensor instead of faking it with a potentiometer
- Maybe some basic AI to predict problems before they happen

---

# ⚠️ Known limitations

- All sensors are simulated — no real hardware
- No network connectivity at all
- Nothing gets saved; no logs, no database
- Can't monitor remotely
- We had to drop IR remote support because Tinkercad's library for it was broken or missing

---

# 👥 Who worked on this

- Lovepreet Sidhu
- Jack Xu
- Jackson Maclean

---

# 📚 Course Information

**Course:** INFO 4381 – Internet of Things  
**School:** Kwantlen Polytechnic University  
**Date:** April 2026
