# Zephyr Graphing Calculator

A simple graphing calculator built with Zephyr RTOS for the Nordic nRF52840 DK.
Facilitates input of functions using a GPIO keypad, and plots them on a 320x240
display with LVGL.

## Features

- Plot up to 5 functions at once
- Supported mathematical operations: `+ - * / ^`, parentheses, and the `x`
  input variable
- Supported built-in functions: `sin`, `cos`, `tan`, `abs`, `sqrt`, `ln`
- Supported constants: `pi`, `e`

## Usage

1. Power on the board. The cartesian plane axes will be drawn automatically.
2. Press the `y=` button to start entering a function. Enter your function, and
   then press `y=` again to confirm the input.
3. Repeat to add more functions, or press the `del` key to delete the most
   recently added one.
