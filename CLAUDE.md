# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

# BikeIno documentation
- UX specification: @.claude/specs/ux-design.md
- Architecture: @.claude/specs/architecture.md

## Project Overview

BikeIno is a bike computer for M5Stack devices. It records bike rides to GPX files, displays speed/distance/duration, and provides logbook and statistics features.

## Build Commands

This is a PlatformIO project. Common commands:

```bash
# Build for M5Stack Core2 (default environment)
pio run

# Upload to device
pio run --target upload

# Serial monitor (115200 baud)
pio device monitor

# Build and upload in one step
pio run --target upload && pio device monitor

# Clean build
pio run --target clean
```