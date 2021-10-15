## Linux

1. Grab the tarball for [libfreefare-0.4.0](https://github.com/nfc-tools/libfreefare/releases/tag/libfreefare-0.4.0)
2. Grab [libnfc 1.8.0](https://github.com/nfc-tools/libnfc/releases/download/libnfc-1.8.0/libnfc-1.8.0.tar.bz2)
3. Build them, weenie
4. Download and install `libgatekeeper`
5. Clone `gatekeeper-door`
8. Build door: `cargo build`
6. Generate secrets: `python generate-creds.py >> .env`
7. Load 'em: `source .env`
9. Clone `gatekeeper-mqtt`
10. Install deps: `pnpm i`
15. Install mongodb
11. Run the server: `node server.js`
16. Format your key (`./examples/format-tag` in `libgatekeeper`)
12. Enroll a new key: `cargo run --bin enroll pn532_uart:/dev/ttyUSB0`
13. Run the door: `cargo run --bin gatekeeper pn532_uart:/dev/ttyUSB0`
14. Try harold! You should just be able to `cargo run`

## macOS

```sh
xcode-select --install
brew install libnfc libfreefare libgcrypt openssl@1.1 pkgconfig cmake
mkdir build
cd build
cmake -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl@1.1 ..

```

### USB-to-UART
Using a UART PN532 breakout with a USB-to-UART converter?
Install the appropriate driver (check System Profiler for the device manufacturer):

- [Silicon Labs CP210x](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers)
- [Prolific PL2303](http://www.prolific.com.tw/US/ShowProduct.aspx?p_id=229&pcid=41)

### Testing Setup
Running `nfc-list` should show:

```sh
nfc-list uses libnfc 1.7.1
NFC device: pn532_uart:/dev/tty.SLAB_USBtoUART opened
```

Or something similar, depending on your NFC reader.

Running `nfc-poll` and presenting a tag to the reader should print some metadata for the tag.
