# SSD1331 RGB OLED Driver for PIC18F26K42

A comprehensive graphics library for the SSD1331 96x64 RGB OLED display, designed for PIC18F26K42 microcontroller using MPLAB X and XC8 compiler. This project provides a complete port of the Adafruit GFX and SSD1331 libraries, optimized for embedded PIC applications.

## 🎥 Demo Video
**[Watch the SSD1331 Driver in Action](https://www.youtube.com/shorts/tiLEcln2PwQ)**

## 📋 Overview

The SSD1331 is a 96x64 full-color RGB OLED display controller that communicates via SPI and provides vibrant, high-contrast visuals perfect for compact embedded applications. This implementation features:

- **Hardware Abstraction Layer**: Modular GFX library that can work with multiple display types
- **Optimized Performance**: Hardware-accelerated drawing functions where available
- **Complete API**: Full compatibility with Adafruit's graphics functions
- **Memory Efficient**: Designed for resource-constrained microcontrollers

## 🏗️ Architecture

The library is organized in a two-layer architecture:

### GFX Layer (`gfx_pic.h/c`)
- **Hardware-independent graphics library**
- Drawing primitives (lines, rectangles, circles, triangles)
- Text rendering with built-in 5x7 font
- Bitmap image support (RGB565 format)
- Rotation and scaling support
- Fallback implementations for maximum compatibility

### SSD1331 Driver Layer (`ssd1331_pic.h/c`)
- **Hardware-specific implementation**
- SPI communication protocols
- Display initialization and configuration
- Hardware-accelerated functions (when available)
- Color space management (RGB565)
- Rotation and display control

## ✨ Features

### Graphics Capabilities
- **Pixel-level Control**: Individual pixel drawing and color manipulation
- **Line Drawing**: Bresenham's algorithm for smooth lines in any direction
- **Shape Primitives**: Rectangles, circles, triangles (filled and outlined)
- **Rounded Rectangles**: Corner radius support for modern UI elements
- **Text Rendering**: Multiple sizes, colors, background colors, and wrapping
- **Bitmap Images**: Display RGB565 formatted images

### Display Tests
The main application (`main.c`) includes comprehensive tests:

- **Color Test Patterns**: LCD test patterns with color bars
- **Line Drawing Tests**: Fan patterns from corners, grid patterns
- **Rectangle Tests**: Concentric outlines and filled rectangles
- **Circle Tests**: Grid patterns for filled and outlined circles  
- **Triangle Tests**: Progressive triangle patterns with color variations
- **Text Formatting**: Multiple sizes, colors, number formatting, and wrapping
- **Image Display**: Full-screen bitmap rendering demonstrations

### Hardware Features
- **SPI Communication**: Optimized for PIC18F26K42 SPI peripheral
- **Hardware Reset**: Proper initialization sequence
- **Rotation Support**: 0°, 90°, 180°, 270° display orientations
- **Color Orders**: RGB and BGR pixel format support

## 🚀 Quick Start

### Hardware Setup
1. Connect your SSD1331 OLED to the PIC18F26K42 SPI interface
2. Ensure proper power supply (3.3V recommended)
3. Configure MCC settings for SPI1 peripheral

### Software Setup
1. Include the library files in your MPLAB X project:
   ```c
   #include "gfx_pic.h"
   #include "ssd1331_pic.h"
   ```

2. Initialize the display:
   ```c
   SSD1331_t oled;
   SSD1331_Init(&oled);
   SSD1331_Begin(&oled);
   ```

3. Start drawing:
   ```c
   GFX_FillScreen(&oled.gfx, &oled, SSD1331_RED);
   GFX_SetCursor(&oled.gfx, 10, 10);
   GFX_Print(&oled.gfx, &oled, "Hello World!");
   ```

## 💾 Memory Considerations

**For Memory-Limited Applications:**

If your application has limited program memory, you can reduce memory usage by commenting out the bitmap image tests in `main.c`:

```c
// Comment out these lines to save memory:
// testimages();  
// __delay_ms(DELAY_MEDIUM);
// __delay_ms(DELAY_MEDIUM);    
```

The bitmap images (`bunmi_img` and `lena` arrays) consume significant program memory. Removing these tests will free up substantial space while maintaining all other graphics functionality.

## 📁 File Structure

```
├── gfx_pic.h           # Graphics library header
├── gfx_pic.c           # Graphics library implementation  
├── ssd1331_pic.h       # SSD1331 driver header
├── ssd1331_pic.c       # SSD1331 driver implementation
├── main.c              # Demo application with comprehensive tests
├── screens.h           # Bitmap image data (optional, memory-intensive)
└── README.md           # This documentation
```

## 🎨 Color Format

The library uses **RGB565** color format:
- **5 bits Red** (0-31): `color & 0xF800`
- **6 bits Green** (0-63): `color & 0x07E0` 
- **5 bits Blue** (0-31): `color & 0x001F`

Pre-defined colors are available:
```c
SSD1331_BLACK, SSD1331_WHITE, SSD1331_RED, SSD1331_GREEN, 
SSD1331_BLUE, SSD1331_CYAN, SSD1331_MAGENTA, SSD1331_YELLOW
```

## 🔧 Configuration Options

### Color Order Selection
Choose your display's pixel color order in `ssd1331_pic.h`:
```c
#define SSD1331_COLORORDER_RGB  // Most common
// #define SSD1331_COLORORDER_BGR  // Alternative
```

### Display Dimensions
Default configuration for standard SSD1331:
```c
#define SSD1331_WIDTH   96
#define SSD1331_HEIGHT  64
```

## 🛠️ Customization

### Adding Custom Fonts
The library includes a built-in 5x7 font, but you can add custom fonts by modifying the font array in `gfx_pic.c`.

### Hardware Optimization
For better performance, implement hardware-specific optimizations by assigning function pointers in the SSD1331 initialization:
```c
oled.gfx.drawPixel = (void*)YourOptimizedPixelFunction;
oled.gfx.fillRect = (void*)YourOptimizedFillFunction;
```

## 📚 API Reference

### Core Functions
- `GFX_DrawPixel()` - Draw single pixel
- `GFX_DrawLine()` - Draw line between two points
- `GFX_DrawRect()` - Draw rectangle outline
- `GFX_FillRect()` - Draw filled rectangle
- `GFX_DrawCircle()` - Draw circle outline
- `GFX_FillCircle()` - Draw filled circle
- `GFX_Print()` - Print text string

### Configuration Functions
- `GFX_SetTextColor()` - Set text color
- `GFX_SetTextSize()` - Set text scaling
- `GFX_SetCursor()` - Set text position
- `SSD1331_SetRotation()` - Set display orientation

## 🐛 Troubleshooting

### Common Issues
1. **Display not initializing**: Check SPI connections and power supply
2. **Wrong colors**: Verify color order setting (RGB vs BGR)
3. **Memory errors**: Comment out bitmap image tests if memory is limited
4. **Text not appearing**: Check cursor position and text color vs background

## 📄 License

This library is based on the original [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) and [Adafruit SSD1331](https://github.com/adafruit/Adafruit-SSD1331-OLED-Driver-Library-for-Arduino) libraries.

**MIT License**
```
Copyright (c) Adafruit Industries
Copyright (c) 2025 Bruno Rodriguez Tondin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
```

## 👨‍💻 Author

**Bruno Rodriguez Tondin** - [@btondin](https://github.com/btondin)

*PIC18F26K42 adaptation and optimization*

---

⭐ **If this library helped your project, please consider giving it a star!**