# SSD1331 OLED PIC18F26K42 Library

Welcome to the SSD1331 OLED Library for PIC18F26K42.  
This project adapts the original Adafruit GFX and SSD1331 libraries for use with a PIC18F26K42 microcontroller using the MPLAB-X and XC8 environment.

The SSD1331 is a 96x64 full-color RGB OLED display that supports SPI communication and provides high-contrast visuals suitable for compact embedded applications. This implementation demonstrates basic drawing functions, text rendering, pixel-level control, and image display using a 16-bit RGB565 color space.

## Features

This application includes the following graphical demonstrations:

- **Color Fill Tests**: Sequential fills with red, green, blue, white, and black.
- **Vertical RGB Stripes**: 3-column demonstration in primary colors.
- **Line and Shape Drawing**: Diagonal lines and bordered rectangles.
- **Text Rendering**: Different text sizes, colors, and background highlights.
- **Geometric Primitives**: Rectangles and circles, both filled and outlined.
- **Checkerboard Pixel Pattern**: Per-pixel control with alternating colors.
- **Bitmap Display**: Rendering of RGB565 image arrays.
- **Counter & Progress Bar**: Dynamic numerical display with a progress bar animation.

All functionalities are implemented using the `GFX` abstraction layer and `SSD1331` driver, adapted for direct compatibility with Microchip’s SPI and delay mechanisms.

## How to Use

To test or modify the example, refer to the `main.c` file.  
Ensure your OLED display is connected to the appropriate SPI interface on the PIC18F26K42, and that the configuration bits and MCC settings match your hardware.

## Licensing

This library is based on the original [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) and [Adafruit SSD1331](https://github.com/adafruit/Adafruit-SSD1331-OLED-Driver-Library-for-Arduino) libraries.

Portions of this code are under the **MIT License**:

Copyright (c) Adafruit Industries

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction...


The remaining source files, including adaptations for PIC and MPLAB-X, are released under the MIT license by the author of this repository.

## Author

Adaptation and PIC integration by Bruno Rodriguez Tondin  
Contact: [btondin](https://github.com/btondin)
