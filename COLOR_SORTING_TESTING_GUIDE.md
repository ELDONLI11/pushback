# Color Sorting System Testing Guide

## Overview
The color sorting system automatically detects ball colors and ejects unwanted balls through the **BACK MID GOAL**. This ensures only the desired color balls are collected and stored.

## Color Sorting Modes

### Available Modes:
1. **COLLECT_RED** - Keep red balls, eject blue balls via mid goal
2. **COLLECT_BLUE** - Keep blue balls, eject red balls via mid goal  
3. **COLLECT_ALL** - Keep all balls (sorting disabled)
4. **EJECT_ALL** - Eject all balls (defensive mode)

## Controller Setup

### Color Mode Selection:
- **Left Stick X (Left)** - Set mode to COLLECT_RED (keep red, eject blue)
- **Right Stick X (Right)** - Set mode to COLLECT_BLUE (keep blue, eject red)

### Control Buttons:
- **RIGHT Arrow** - Toggle sorting ON/OFF (overrides front flap control when active)
- **LEFT Arrow** - Manual ejection trigger (for testing)
- **L1** - Increase ejection duration (+50ms)
- **L2** - Decrease ejection duration (-50ms)

## Test Scenarios

### **Scenario 1: Red Ball Collection Mode**
**Setup:**
1. Push left stick to the left (activates COLLECT_RED mode)
2. Controller displays: "SORT: RED"
3. Place blue and red balls in intake

**Expected Behavior:**
- **Red balls**: Pass through normally, go to storage
- **Blue balls**: Detected by color sensors → ejected via BACK MID GOAL
- **Storage count**: Only increases when red balls enter storage
- **Display**: Shows "SORT: RED" on controller

**Debug Output:**
```
🎨 Ball color confirmed: RED
Ball added to storage. Count: 1/3

🎨 Ball color confirmed: BLUE  
🚨 BALL EJECTION STARTING - Using MID GOAL for unwanted ball color
🎯 Unwanted ball color being ejected via BACK MID GOAL
```

### **Scenario 2: Blue Ball Collection Mode**
**Setup:**
1. Push right stick to the right (activates COLLECT_BLUE mode)
2. Controller displays: "SORT: BLUE" 
3. Place red and blue balls in intake

**Expected Behavior:**
- **Blue balls**: Pass through normally, go to storage
- **Red balls**: Detected by color sensors → ejected via BACK MID GOAL
- **Storage count**: Only increases when blue balls enter storage
- **Display**: Shows "SORT: BLUE" on controller

### **Scenario 3: Sorting Disabled Mode**
**Setup:**
1. Press RIGHT arrow to toggle sorting OFF
2. Controller displays: "SORT: OFF"
3. Place mixed color balls in intake

**Expected Behavior:**
- **All balls**: Pass through normally, go to storage
- **No ejection**: Color detection still works but no ejection occurs
- **Storage count**: Increases for all balls regardless of color

### **Scenario 4: Manual Ejection Testing**
**Setup:**
1. Enable any sorting mode (RED or BLUE)
2. Place a ball at the color sensors
3. Press LEFT arrow for manual ejection

**Expected Behavior:**
- **Immediate ejection**: Ball is ejected via BACK MID GOAL
- **Controller feedback**: "MANUAL EJECT" + rumble
- **System state**: Returns to previous operation after ejection

### **Scenario 5: Integration with Storage Limit**
**Setup:**
1. Set storage count to 2/3 (using Left + R1)
2. Enable COLLECT_RED mode
3. Feed 1 red ball + 1 blue ball

**Expected Behavior:**
- **Red ball**: Enters storage, count becomes 3/3
- **Blue ball**: Ejected via mid goal, count stays 3/3
- **Storage full**: Next storage attempt blocked with "STORAGE FULL!"

## Ejection Mechanism Details

### How Mid Goal Ejection Works:
1. **Color Detection**: Sensors detect unwanted ball color
2. **Timing**: System waits for ball to reach ejection point (sensor 2)
3. **Mode Switch**: Temporarily switches to MID_GOAL mode
4. **Execution**: Executes BACK indexer for mid goal scoring
5. **Restoration**: Returns to previous indexer state

### Key Features:
- **No Storage Impact**: Ejected balls don't affect storage count
- **State Preservation**: Previous indexer operation is restored after ejection
- **Conflict Prevention**: Won't eject if indexer is busy with scoring

## Advanced Configuration

### Ejection Duration Tuning:
- **L1**: Increase ejection time (+50ms) - for larger/heavier balls
- **L2**: Decrease ejection time (-50ms) - for faster ejection
- **Range**: 300ms to 800ms (automatically clamped to safe limits)

### Color Detection Settings:
Located in `include/color_sensor.h`:
```cpp
#define RED_HUE_MIN             0     // Red hue range minimum
#define RED_HUE_MAX             30    // Red hue range maximum
#define BLUE_HUE_MIN            200   // Blue hue range minimum  
#define BLUE_HUE_MAX            250   // Blue hue range maximum
#define MIN_SATURATION          50    // Minimum saturation for valid color
#define COLOR_CONFIRMATION_COUNT 3    // Consistent readings required
```

## Troubleshooting

### **Color Detection Issues**
1. Check LED brightness (should be 100%)
2. Verify color thresholds in config.h
3. Clean sensor lenses
4. Test with known good colored balls

### **Ejection Not Working**
1. Verify PTO is in scorer mode
2. Check ejection duration settings (L1/L2)
3. Look for "indexer busy" messages
4. Test manual ejection (LEFT arrow)

### **Storage Count Issues with Sorting**
1. Ejected balls should NOT affect storage count
2. Only kept balls should increase storage count
3. Check debug output for "Ball added to storage" vs "EJECTION"

### **Button Conflicts**
- RIGHT arrow prioritizes color sorting when active
- Front flap control available when sorting is OFF
- Use toggle to switch between functions

## Performance Monitoring

### Statistics Available:
- Red balls detected
- Blue balls detected  
- Total balls ejected
- False detections
- Current ejection duration

### Debug Commands:
```cpp
color_sensor_system->printStatus();     // Print current status
color_sensor_system->getStatistics();   // Get detection counts
color_sensor_system->resetStatistics(); // Reset counters
```

## Expected Integration Results

✅ **Proper Color Sorting**: Unwanted colors ejected via mid goal
✅ **Storage Counting**: Only desired balls count toward storage limit  
✅ **Mid Goal Efficiency**: Fast ejection without blocking main operations
✅ **State Management**: Seamless return to previous operations
✅ **User Feedback**: Clear controller messages and rumble patterns
✅ **Safety**: Automatic conflict detection and ejection timing