# Color Sorter & Counter Test Mode

## Overview
Test mode #15 provides real-time color detection and storage counting display on the controller. This helps you configure and verify the color sorter system is working correctly.

## How to Select This Test

### During Startup (Development Mode):
1. Power on the robot (not connected to competition switch)
2. Wait for initialization to complete
3. You'll have 10 seconds for autonomous selection
4. Press **UP** or **DOWN** to navigate to mode **15: Test: Color Sorter**
5. Press **A** to confirm selection
6. The test will run automatically

### During Competition Disabled Period:
1. Connect to competition switch
2. During disabled period, press **UP** or **DOWN** arrows
3. Navigate to mode **15: Test: Color Sorter**
4. Press **A** to confirm
5. Test will run when autonomous period starts

## Controller Display Format

### Line 0: `[COLOR] St:X/3 M:MODE`
- **COLOR**: Last detected ball color
  - `RED` - Red ball detected
  - `BLUE` - Blue ball detected
  - `---` - No ball currently detected
  - `???` - Unknown/ambiguous reading
- **St:X/3**: Storage count (X balls out of 3 maximum)
- **M:MODE**: Current sorting mode
  - `ALL` - COLLECT_ALL (all balls kept)
  - `RED` - COLLECT_RED (keep red, eject blue)
  - `BLU` - COLLECT_BLUE (keep blue, eject red)
  - `NON` - EJECT_ALL (eject all balls)

### Line 1: `R:X B:X E:X F:X`
- **R:X** - Total red balls detected
- **B:X** - Total blue balls detected
- **E:X** - Total balls ejected
- **F:X** - False detections (inconsistent readings)

## Controller Controls During Test

| Button | Function |
|--------|----------|
| **Left stick left** | Switch to COLLECT_RED mode |
| **Right stick right** | Switch to COLLECT_BLUE mode |
| **X button** | Reset all statistics |
| **B button** | Exit test mode |

## Test Procedure

### Basic Color Detection Test:
1. Start the test (mode 15)
2. Keep sorting mode on **COLLECT_ALL** (default)
3. Feed red balls through intake
4. Watch controller display update with:
   - `[RED]` on detection
   - R:1, R:2, R:3... increment
   - Storage count increment (St:1/3, St:2/3, St:3/3)
5. Feed blue balls through intake
6. Watch controller display show:
   - `[BLUE]` on detection
   - B:1, B:2, B:3... increment
   - Storage count continue incrementing

### Sorting Mode Test:
1. Press **Left stick left** to switch to COLLECT_RED
   - Controller will show `M:RED`
   - Controller rumbles once
2. Feed a blue ball
   - Should display `[BLUE]`
   - E:X counter increments (ball ejected)
   - Storage count does NOT increment
3. Feed a red ball
   - Should display `[RED]`
   - R:X counter increments
   - Storage count DOES increment
4. Press **Right stick right** to switch to COLLECT_BLUE
   - Test opposite behavior

### Calibration Testing:
1. Clean both sensors (ports 19 and 20)
2. Start test in COLLECT_ALL mode
3. Feed 10 mixed balls slowly (5 red, 5 blue)
4. After test, check final statistics:
   - **R:5 B:5** → Perfect detection ✅
   - **F:0** → No false detections ✅
   - **Storage: 10/3** → All balls counted (will exceed limit) ✅

### Troubleshooting Detection:
- **High false detections (F:X > 2)**:
  - Clean sensors with microfiber cloth
  - Check lighting conditions
  - Adjust MIN_SATURATION or MIN_BRIGHTNESS thresholds
- **Wrong colors detected**:
  - Check hue thresholds (RED_HUE_MIN/MAX, BLUE_HUE_MIN/MAX)
  - Verify sensor LED brightness (should be 100%)
- **No detection at all**:
  - Verify sensors are on ports 19 (lower) and 20 (upper)
  - Check sensor proximity values (should be < 100 when ball present)
  - Test sensors with `color_sensor_system->testSensors()`

## Expected Console Output

```
=== COLOR SORTER & COUNTER TEST ===
This test displays real-time ball detection and storage count
Feed balls through the intake to test color detection

✅ Color sensor system ready
✅ Storage counter ready
📊 Initial display format:
   Line 0: [Color] Count X/3
   Line 1: R:X B:X E:X

Controller controls during test:
  LEFT stick left  → COLLECT_RED mode
  RIGHT stick right → COLLECT_BLUE mode
  X button → Reset statistics
  B button → Exit test

🎯 Ball detected | Color: RED | Storage: 1/3 | Mode: ALL
🎯 Ball detected | Color: BLUE | Storage: 2/3 | Mode: ALL
🔴 Switched to COLLECT_RED mode
🎯 Ball detected | Color: BLUE | Storage: 2/3 | Mode: RED
...

=== TEST COMPLETE ===
📊 Final Statistics:
   Red balls detected: 5
   Blue balls detected: 3
   Balls ejected: 2
   False detections: 0
   Final storage count: 6/3
   Total balls seen: 8

✅ Ejection system working: 2 balls ejected
```

## Success Criteria

### Good Detection Performance:
- False detections < 10% of total balls
- Correct color identification > 95%
- Storage count matches balls kept (not ejected)
- Ejected count matches unwanted balls in sorting modes

### System Ready for Competition:
- ✅ Red and blue balls correctly identified
- ✅ False detections near zero
- ✅ Storage counter accurate (max 3 balls)
- ✅ Ejection working in sorting modes
- ✅ Mode switching responds correctly

## Related Configuration

### Color Thresholds (in `color_sensor.h`):
```cpp
#define RED_HUE_MIN             0
#define RED_HUE_MAX             30
#define BLUE_HUE_MIN            200
#define BLUE_HUE_MAX            250
#define MIN_SATURATION          50
#define MIN_BRIGHTNESS          30
#define MAX_PROXIMITY_THRESHOLD 100
```

### Sensor Ports (in `color_sensor.h`):
```cpp
#define COLOR_SENSOR_1_PORT     19  // Lower sensor (entry)
#define COLOR_SENSOR_2_PORT     20  // Upper sensor (confirmation)
```

## Tips for Best Results

1. **Clean sensors before testing** - fingerprints affect readings
2. **Test in match lighting** - fluorescent vs LED makes a difference
3. **Feed balls slowly** - gives sensors time to confirm readings
4. **Watch for consistent patterns** - same ball type should give similar readings
5. **Use both sensors** - dual confirmation reduces false positives
6. **Test full storage** - verify system handles 3-ball limit correctly
7. **Test all modes** - COLLECT_RED, COLLECT_BLUE, COLLECT_ALL, EJECT_ALL

## Next Steps After Testing

If color detection is accurate:
1. Run integration test with actual game balls
2. Test during driver control (not just autonomous)
3. Verify ejection timing and accuracy
4. Test at competition field if possible

If detection needs improvement:
1. Adjust thresholds in `color_sensor.h`
2. Re-clean and re-test sensors
3. Check sensor mounting (stable, 6" from ball path)
4. Consider ambient light compensation
