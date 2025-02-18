# LilyGo T Dongle ESP-NOW RX

This unit serves as an in-hand debugging reference for monitoring ESP-NOW sensor node traffic using a LilyGo T-Dongle with a TFT display. The program listens for ESP-NOW packets, logs received messages over serial, and displays relevant information.

## System Components
1. **T-Dongle S3** - Acts as the receiver for ESP-NOW messages.
2. **ESP-NOW Protocol** - Used for low-power, peer-to-peer wireless communication between nodes.
3. **TFT Display (ST7735S)** - Provides a visual output of received messages.
4. **Serial Output** - Logs messages and received packet details for additional debugging.

## Functionality
- **Receives ESP-NOW messages** from transmitting nodes.
- **Displays received data on the TFT screen** in a structured format.
- **Maintains a message counter** to track network traffic.
- **Outputs detailed logs over serial** for advanced debugging.

## Display Format
Each received message is displayed in the following format:
```
RX: <Message Counter>
DATA: <Received Data>
Length: <Bytes Received>
```
### Example Output on TFT:
```
RX: 45
DATA: Hello ESP
Length: 10
```

## Code Explanation
### 1. **ESP-NOW Initialization**
The receiver initializes ESP-NOW with:
- `esp_now_init()` to enable ESP-NOW.
- `esp_now_register_recv_cb(espnow_recv_cb)` to register the callback function for received messages.
- `esp_now_set_pmk(PMK_KEY)` to set the pre-shared key for encrypted communication.

### 2. **Message Reception & Processing**
The callback function `espnow_recv_cb()` executes when a message is received:
- Increments the `msg_counter`.
- Logs the message over serial.
- Updates the TFT display with the received data.

### 3. **TFT Display Updates**
The TFT screen is updated using:
```cpp
tft.clear();
tft.setCursor(0, 0);
tft.printf("RX: %d\n", msg_counter);
tft.printf("DATA: %.*s\n", len, data);
tft.printf("Length: %d", len);
tft.display();
```
This ensures the most recent message is always visible.

### 4. **Wi-Fi Setup for ESP-NOW**
Wi-Fi is configured to station mode and set to use **long-range (802.11 LR)** for improved communication over distances:
```cpp
ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
ESP_ERROR_CHECK(esp_wifi_set_channel(9, WIFI_SECOND_CHAN_NONE));
```

## Debugging Steps
### 1. **Verify Serial Output**
Monitor messages via the serial console:
```sh
idf.py monitor
```
Expected log output:
```
I (12345) RX_MAIN: Packet received from AA:BB:CC:DD:EE:FF, len: 10, Data: Hello ESP
```

### 2. **Check TFT Display**
Ensure that:
- The screen initializes correctly and displays `Waiting for ESPNOW...` at startup.
- Incoming messages update the display in real-time.

### 3. **Confirm Node Communication**
If messages aren’t appearing:
- Verify TX nodes are transmitting on **channel 9**.
- Ensure ESP-NOW encryption keys match.
- Check Wi-Fi mode (must be `WIFI_MODE_STA`).

