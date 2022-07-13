# MWSDK Library Revision History

Major modifications for each library are listed.

See also the individual libraries below.

* MWX library  ([Manual](https://mwx.twelite.info/v/latest_en/)|[repository](https://github.com/monowireless/mwx))
* twesettings library ([repository](https://github.com/monowireless/twesettings))

The version name is MWSDK{YYYYY}\_{MMM} including the year and month of release, with UNOFFICIAL indicating that the release does not include comprehensive validation work.

# MWSDK2022_07

Addition of sample Act, new functions for TWELITE ARIA/TWELITE CUE/TWELITE PAL, etc.

## TWELITE APPS

* Added English translation to 000desc.txt
* Changed Samp_PingPong and Samp_I2C to newly adopted deprecated

### Cue App (App_CUE/App_CUE_OTA) v1.1.0
* Added ability to send one sample of acceleration at each specified interval
* Addition of function to send average, minimum and maximum values of acceleration measured for a certain period of time

### Aria App(App_ARIA/App_ARIA_OTA) v1.1.0
* Added the ability to multiply measured temperature and humidity values by an offset and a factor

### Pal App (App_PAL) v1.5.1
* Added ability to send acceleration one sample at a time with motion sensor PAL
* Added ability to send average, minimum and maximum values of acceleration measured for a certain period of time in motion sensor PAL
* Add function to multiply measured values of temperature and humidity by offset and coefficient in environment sensor pal 

### Parent Node and Repeater Application (App_PAL) v1.2.0
* Added function to output average, maximum and minimum values of acceleration

## Act_samples
* Added English translation to 000desc.txt
* Add BRD_I2C_TEMPHUMID
  * Sample using I2C sensor device to measure and transmit from periodic wake-up

## Act_extras
* Add English translation to 000desc.txt
* Add ActEx_AQM0802
  * Sample using LCD module AQM0802 implementing Sitronix I2C connected LCD controller ST7032

## mwx - 0.2.0
* Changed Wire object to allocate memory to heap area
* Changed function name from G_OCTET() to G_BYTE() to avoid name conflict in utils.h
* Changed the order of vAHI_DioInterruptEnable() in attachIntDio()
* Added the_twelite.network2 to support universal receivers (NWK_LAYERED, NWK_SIMPLE or networkless packets in the same executable code)
* Added NWK_LAYERED (only Parent Node reception is supported at this time)
* Introduced MWX_Set_Usder_App_Ver() function to set application version at MWX initialization
* mwx::pnew() was added to simplify description of deployment new
* added support for EASTL
* added new[] operator for EASTL
* Precompiled most of the MWX source code to speed up compilation
* Fixed DIO events being passed to unrelated ports

## twesettings - 0.2.6

No changes since MWSDK2021_09. Compiled libraries remain 2.5. Source code only.



## TWENET C - 1.3.05 (L1305)

No changes since MWSDK2021_09.

# MWSDK2021_12

Fixes for TWELITE ARIA support, etc.



## TweApps

### App_PAL (1.3.4)

* Added support for TWELITE ARIA



### App_UART (1.4.6)

* Fixed to not output NUL characters at the end.
* `>` (end of checksum range) now works in output customization.
  * For example, setting `MSG;<*>;%X\\n` will output "`MSG;HELLO;nn<newline>`" (nn is the XOR checksum of `HELLO`) for the transmission string "`HELLO`".




### App_ARIA (1.0.0)

* New additions



### App_ARIA_OTA (1.0.0)

* New additions



### App_Wings (1.1.6)

* Fixed a problem that App_Wings in the repeater setting does not relay when the Parent Node is not App_Wings.



## Act_samples

### BRD_ARIA

Sample that performs measurement and wireless transmission while driving intermittently with sleep.



### Unit_brd_ARIA

This sample uses SHT4x sensor object `.sns_SHT4x` included in board BEHAVIOR `ARIA` to operate only the sensor.



## mwx - 0.1.9

* Add TWELITE ARIA support.
  * Add board BEHAVIOR `<ARIA>` (`BrdARIA` class)
  * added `SnsSHT4x` sensor definition
* Modified Interactive settings mode `<STG_STD>` to work with Extra Menu (* In mwx0.1.8, the output via Serial is automatically suppressed when Interactive mode is displayed, but we prepared a procedure to allow output from the code of the built-in function). (* In mwx0.1.8, the output via Serial is automatically suppressed during interactive mode display.)



## twesettings - 0.2.6

No changes since MWSDK2021_09. Compiled libraries are still 2.5. Source code only.



## TWENET C - 1.3.05 (L1305)

No changes since MWSDK2021_09.



# MWSDK2021_09

* Enhanced VS Code support for each sample. 
  * Enabled code interpretation and build execution in VS Code without special name settings when environment variables are set from TWELITE STAGE and VS Code is opened via STAGE.



## TweApps

### App_CUE (1.0.3)

* Fixed problem that occurs when sleep time is set to 61 seconds or longer.



### App_IO (1.3.2)

* Fixed a problem that only up to 8 ports are reflected in UART command processing.



### App_PAL (1.3.3)

* Adjusted OTA messages
* Ensured that the MC3630 accelerometer initialization procedure waits for a certain amount of time.
* Added a setting to change the threshold value for MC3630 accelerometer interrupt.



### App_Tag (2.2.4) (2525A)

* When obtaining the sensor value for each sleep recovery time with the ADXL345 accelerometer, there were cases where the appropriate sensor value could not be obtained, so multiple samples were obtained to eliminate the influence of inappropriate samples.
* Explicitly gave appropriate parameters to the DUPCHK module for packet de-duplication.
* explicitly wait processing for sensor acquisition for MAX31855



### App_UART (1.4.1 -> 1.4.5)

* Added dedicated build for TWELITE UART
* Added a new option bit 0x0000:4000 to set the packet ID for high frequency transmission.
* Added Mode E and made it the default. In the previous Mode D (transparent mode), it was not possible to obtain information obtained from wireless packets, such as address information of arriving packets, but now various information can be included in the output in a customizable form.



## Act_samples

* Samples are maintained for easy understanding in the following step-by-step manner
  * act0..4
  * Scratch (send by serial input)
  * Slp_Wk_and_Tx (sleep -> wake -> send)
  * Parent_MONOSTICK (receive)
  * ...
* Expanded samples supporting Interactive settings mode.
* Support for `on_rx_packet()` and `on_tx_comp()` callback functions
* Enhanced `Parent_MONOSTICK` sample, interprets packet contents and displays sensor values, etc.
* Sleep samples now use the `SM_SIMPLE` state machine to complete the `loop()`.
* Addition of `Unit_Cue_MagBuz` sample (sounds piezoelectric buzzer when magnet leaves)
* Add `Unit_doint-bhv` sample (DIO interrupt detection using BEHAVIOR)
* Renaming of sample names
  * Rename `PAL_MOT` -> `PAL_MOT-fifo`.
  * `PAL_MOT-oneshot` -> `PAL_MOT-single`.
  * `PAL_AMB-behave` -> `PAL_AMB_bhv`.



## mwx - 0.1.8

* Port and alternate port definitions for `Serial1` were not appropriate.
* Added the ability to change the baud rate of `Serial` (UART0).
* Added event callbacks to notify received packets (`on_rx_packet()`) and completed transmission (`on_tx_comp()`)
  * Previous procedures can be used if callback functions are not defined.
* Wrong definition ID for `<STG_STD>` Interactive settings mode settings and some default values changed.
* added the ability to change default values of CHANNEL and logical device ID in addition to AppID in `<STG_STD>` Interactive settings mode.
* Added the ability to configure `the_twelite` and `<NWK_SIMPLE>` objects with the `<STG_STD>` object in Interactive mode for some settings.
* added support for setting the default value of retransmission count in `<NWK_SIMPLE>`.
* Do not input/output `Serial`(UART0) from the application while the `<STG_STD>` Interactive settings mode screen is displayed.
* `CUE::PIN_SET`, `PAL???""" Add `:PIN_SET` (because `PIN_BTN` is unnatural to use in CUE without buttons).
* moved namespace of `random()` to `mwx::` (Elias to global name)
* modified MONOSTICK's watchdog setting to be done in 32ms units



## twesettings - 0.2.6

No changes. Compiled libraries remain 2.5. Source code only.



## TWENET C - 1.3.05 (L1305)

* Fixed problem with radio operation not working until the first CHANNEL change cycle when CHANNEL manager is used.
* When a channel switchover fails, the variable indicating the pending channel switchover is not cleared to 0, but the channel is switched again within 64ms (currently the channel waits for polling until it can be switched, so the pending operation is not performed. (Currently, the channel waits for polling until it can be switched, so there is no pending operation.)
* (Currently, the channel waits for polling until it can be switched, so there is no pending behavior, but it is a natural process in the implementation.
* Stricter handling of interrupts in `ByteQyeue`.
* When multiple hardware interrupts occur almost at the same time and there are multiple events in the hardware event queue, there was a possibility that the event behind the event queue would be delayed (only the first event is processed and once it exits the processing loop).





# MWSDK2020_12

## TweApps

### App_CUE 1.0.1

* New

### App_CUE_OTA 1.0.1

* New

### App_Wings 1.1.4

* TWELITE CUE support

### App_PAL 1.3.2

* Added code for TWELITE CUE
* Implemented receive-only mode in NOTICEPAL



## Act_samples

* Add Unit\_PktParser (serial telegram parser)
* Add Unit\_EEPROM (read/write EEPROM)
* Add Unit\_ADC (read ADC)
* Unit\_CUE (Act support for TWELITE CUE)



## mwx - 0.1.7

- See https://mwx.twelite.info/v/latest/ for details
- Add pkt_parser (interpret serial messages)
- Add EEPROM class (read/write EEPROM)
- Added receive\_nwkless_pkt option, under NWK_SIMPLE network, Application ID of wireless packet, CHANNEL can receive matched normal packets.



## twesettings - 0.2.6

Compiled library is still 2.5. Source code only.

* Fixed a problem with the list display and selection behavior when the number of items exceeds 10 in the menu provided by tweinteractive_defmenus.c.



# MWSDK2020_10

Updated mwx library, added and updated samples, more practical samples Act Extra (ActEx\_????) recorded.

## TweApps.

### App_PAL 1.2.1

* NOTICE PAL support
* Enabled to set sending interval every second.

### App_Tag 2.2.2

* Fixed sensor value calculation formula for BME280.

### App_Twelite 1.9.1

* Added ACK response to determine life or death by serial message

### App_UART 1.4.1

* Adjusted timeout for duplicate packets (can receive at 100ms intervals, timeout for duplicate determination is 1 second and if the same packet arrives beyond this timeout, it will be displayed again)

### App_Wings 1.1.3

* NOTICE PAL support
* Other fixes



## Act_samples

* Check only single function Unit\_???? Add sample
* Add sample for LED driver control of Notify PAL (NOTICE PAL) (Unit_using_PAL_NOTICE)
* Add template code Slp_Wk_and_Tx to send after sleep wake-up
* Re-wrote loop() of Pal_MOT_oneshot with state transitions
* Added a sample Setting that uses settings (Interactive settings mode) in Act



## Act_extras

* Added ActEx_DIO_Trans (fast sample of DI state and reproduce to destination DO)
* Added ActEx_Sns_BME280_SHT30 (environmental sensor BME280, SHT30 device)
* ActEx_Sns_MLX90614 (uses infrared temperature sensor MLX90614)
* ActEx_Sns_VL53L1X (use of laser distance sensor VL53L1X)



## mwx - 0.1.6

### [0.1.6b4]

- Added output of div100(), which calculates the quotient and remainder, to Serial, etc.
- Changed implementation of smplbuf<> array class. Inheritance of mwx::stream was stopped for the purpose of reducing memory consumption, etc. Inheritance class and helper class were defined separately.
- added mwx_printf() mwx_snprintf() function
- added the_twelite.stop_watchdog() and the_twelite.restart_watchdog()
- mwx::stream maintenance: removed operator bool(). Disable timeout when 0xff is specified (.set_timeout(0xff)) in read timeout setting. Other << operator definitions added.

### [0.1.6b3]

- Added NOTICE PAL / PCA9632 support (description https://mwx.twelite.info/v/latest/boards/pal/pal_notice, Samples https://github.com/monowireless/Act_samples/tree/master/Unit_using_PAL_NOTICE)
- Added scale function between 8bit and 0..1000 without division.

### [6fd542c]

- Divisions by 10, 100, and 1000 (simultaneous computation of quotient and remainder) div10(), div100(), div1000() added. Restricted the value range and focused on multiplication and bit shifting.
- Added methods for encrypted packets
  - packet_rx::is_secure_pkt() : Checks if a received packet is encrypted or not.
  - STG_STD::u8encmode() : Obtain encryption settings in Interactive settings mode
  - STG_STD::pu8enckeystr() : Obtain encryption key byte sequence in Interactive settings mode

### [0.1.6b2]

- Serial1: Default ports are DIO11(TxD) and DIO9(RxD) because they are usually assigned to I2C, although DIO14 and 15 overlap with I2C in the semiconductor specification.

- Serial: The baud rate specification generates /100, but this calculation is omitted for the main baud rates.

- Serial: The proxy function to execute available() and read() externally is held only in void*, and the specification memory is reduced by 8 bytes.

- Added typedef boolean.

- Network: Added support for encryption.

  - To enable encryption, set NWK_SIMPLE::secure_pkt(const uint8_t*, bool = false). If the second parameter is true, plaintext packets are also received.

```
    auto&& nwk = the_twelite.network.use<NWK_SIMPLE>();
    nwk	<< NWK_SIMPLE::logical_id(0xFE) // set Logical ID. (0xFE means a child device with no ID)
    	<< NWK_SIMPLE::secure_pkt((const uint8_t*)"0123456789ABCDEF");
    	;
```

- Added sensor support for SHT3x and BME280

- Sensor: added a mechanism to exchange configuration parameters and status in legacy code (C library wrapper class).

- Sensor: I2C address can be specified for SHT3x and BME280.

- Configuration: added hide_items(). Unnecessary items can be deleted.

- Configuration: added H/W UTIL menu to display DI status, I2C probe, and PAL EEPROM contents.

- Configuration: Encryption related menus added.

### [4d8ce8b]

* I2C-related fixes (fixes to improve compatibility with code implemented using the TwoWire class)
  * Processing was not normal because there was no code to send NO_STOP message when processing requestFrom(false).
  * Added TwoWire class name aliases.
  * The begin() process is now not multiply initialized.
  * Added setClock() method (but it is a dummy function and does nothing).
  * Added WIRE_CONF::WIRE\_?KHZ added. Added major setting values for bus clocks.

## twesettings - 0.2.5

No updates.



## TWENET C - 1.3.04 (L1304)

No update



# MWSDK2020_08_UNOFFICIAL - 2020-08-05

Added settings (Interactive settings mode) to the mwx library.

## mwx - 0.1.5

* Added setting BEHAVIOR <STG_STD>.
  * Added settings behavior to main loop processing since there are now 4 built-in BEHAVIORs in the mwx library: network board application settings
  * BEHAVIOR takes Serial input, so a separate FIFO queue for input was added, and Serail.read() now retrieves data from this queue.
  * Added an interface to replace <STG_STD> table with fixed item name and label settings.
* Added support for channel managers (using multiple CHANNELs)

## twesettings - 0.2.5

* Maintained mwx to add Interactive settings mode.
  * Added table definitions for configuration name swapping
  * Moved table definitions for mwx to the mwx library

## TWENET C - 1.3.04 (L1304)

* Added support functions for registering internal modules from mwx
* Added header definitions to allow parametrization of MOD_Duplicate checker timeout and time scale values





# MWSDK2020_07_UNOFFICIAL - 2020-07-27

Added some issues and mwx library API.

## mwx - 0.1.4

* Enhanced initialization process for MC3630
* Improved accuracy of delay() during setup() and wakeup()
* Added declaration of Serial1 instance
* Addition of delayMilliseconds()
* Addition of digitalReadBitmap()
* Alalogue callback function was not called in some cases

## twesettings - 0.2.4

* Added μsec wait API TWESYSUTIL_vWaitPollMicro()

## TWENET C - 1.3.03 (L1303)

No change



# MWSDK2020_05 - 2020-05-29

The following is a list of subsequent changes starting with MWSDK2020_05.

## mwx - 0.1.3

## twesettings - 0.2.3

## TWENET C - 1.3.03 (L1303)

