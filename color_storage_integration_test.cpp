/**
 * Color Sorting + Storage Integration Test
 * 
 * This test demonstrates how the color sorting system integrates with
 * the 3-ball storage limit to ensure only desired balls are stored.
 */

#include "indexer.h"
#include "color_sensor.h"

void testColorSortingStorageIntegration() {
    printf("=== Color Sorting + Storage Integration Test ===\n");
    
    // Test requires both systems to be initialized
    IndexerSystem* indexer = nullptr;           // Get actual indexer instance
    ColorSensorSystem* color_sensor = nullptr;  // Get actual color sensor instance
    
    if (!indexer || !color_sensor) {
        printf("❌ ERROR: Systems not available for testing\n");
        return;
    }
    
    // Test 1: COLLECT_RED mode with storage counting
    printf("\n--- Test 1: COLLECT_RED Mode ---\n");
    
    // Setup: Reset storage and set to collect red balls
    indexer->resetStorageBallCount();
    color_sensor->setSortingMode(SortingMode::COLLECT_RED);
    
    printf("Initial storage: %d/3\n", indexer->getStorageBallCount());
    printf("Sorting mode: COLLECT_RED (keep red, eject blue)\n");
    
    // Simulate ball detection and processing
    printf("\n🔴 Simulating RED ball detection:\n");
    printf("Expected: Ball should be stored, count increases\n");
    // In real test: place red ball in intake
    // Result: Ball passes through sensors → added to storage automatically
    
    printf("\n🔵 Simulating BLUE ball detection:\n");  
    printf("Expected: Ball should be ejected via mid goal, count unchanged\n");
    // In real test: place blue ball in intake
    // Result: Ball detected → ejected via back mid goal → storage count unchanged
    
    // Test 2: Storage limit interaction
    printf("\n--- Test 2: Storage Limit with Color Sorting ---\n");
    
    // Set storage to 2/3
    indexer->addBallToStorage();
    indexer->addBallToStorage();
    printf("Storage set to: %d/3\n", indexer->getStorageBallCount());
    
    printf("\n🔴 Testing RED ball with nearly full storage:\n");
    printf("Expected: Ball stored if space available, rejected if full\n");
    
    printf("\n🔵 Testing BLUE ball with full storage:\n");
    printf("Expected: Ball ejected regardless of storage count\n");
    
    // Test 3: Mode switching
    printf("\n--- Test 3: Mode Switching ---\n");
    
    color_sensor->setSortingMode(SortingMode::COLLECT_BLUE);
    printf("Switched to COLLECT_BLUE mode\n");
    printf("Now: Keep blue balls, eject red balls\n");
    
    color_sensor->setSortingMode(SortingMode::COLLECT_ALL);
    printf("Switched to COLLECT_ALL mode\n");
    printf("Now: Keep all balls, no ejection\n");
    
    // Test 4: Statistics verification
    printf("\n--- Test 4: Statistics ---\n");
    
    int red_count, blue_count, ejected_count, false_count;
    color_sensor->getStatistics(red_count, blue_count, ejected_count, false_count);
    
    printf("Red balls detected: %d\n", red_count);
    printf("Blue balls detected: %d\n", blue_count);  
    printf("Total balls ejected: %d\n", ejected_count);
    printf("False detections: %d\n", false_count);
    printf("Final storage count: %d/3\n", indexer->getStorageBallCount());
    
    printf("\n=== Integration Test Complete ===\n");
}

/**
 * Live Testing Instructions for Color Sorting + Storage Integration:
 * 
 * 1. **Setup Phase**:
 *    - Initialize both color sensor and indexer systems
 *    - Verify PTO is in scorer mode (required for ejection)
 *    - Reset storage count (Left + R2 three times)
 *    - Clear color sensor statistics
 * 
 * 2. **COLLECT_RED Mode Test**:
 *    - Push left stick left → "SORT: RED" on controller
 *    - Place 2 red balls in intake → should store both (Storage: 2/3)
 *    - Place 1 blue ball in intake → should eject via mid goal (Storage: 2/3)
 *    - Place 1 red ball in intake → should store (Storage: 3/3)
 *    - Place another ball (any color) → storage full warning
 * 
 * 3. **COLLECT_BLUE Mode Test**:
 *    - Reset storage count to 0/3
 *    - Push right stick right → "SORT: BLUE" on controller  
 *    - Place 1 blue ball → should store (Storage: 1/3)
 *    - Place 1 red ball → should eject via mid goal (Storage: 1/3)
 *    - Place 2 blue balls → should store both (Storage: 3/3)
 *    
 * 4. **Mixed Color Test**:
 *    - Reset storage count to 0/3
 *    - Set to COLLECT_RED mode
 *    - Alternate red and blue balls:
 *      * Red ball → stored (1/3)
 *      * Blue ball → ejected (1/3)  
 *      * Red ball → stored (2/3)
 *      * Blue ball → ejected (2/3)
 *      * Red ball → stored (3/3)
 *      * Any ball → storage full
 * 
 * 5. **Verification Checklist**:
 *    ✅ Desired color balls are stored and counted
 *    ✅ Unwanted color balls are ejected (no storage impact)
 *    ✅ Storage limit (3 balls) is respected
 *    ✅ Ejection uses back mid goal effectively
 *    ✅ Controller shows correct sorting mode and storage count
 *    ✅ System preserves indexer state after ejection
 *    ✅ No conflicts between sorting and manual storage operations
 * 
 * 6. **Expected Debug Output**:
 *    ```
 *    🎨 Ball color confirmed: RED
 *    ✅ Desired color ball (RED) passed through - adding to storage
 *    💾 Ball added to storage by color sensor system
 *    DEBUG: Ball added to storage. Count: 1/3
 *    
 *    🎨 Ball color confirmed: BLUE
 *    🚨 BALL EJECTION STARTING - Using MID GOAL for unwanted ball color
 *    🎯 Unwanted ball color being ejected via BACK MID GOAL
 *    ⏹️ Ball ejection stopped - returning indexer to normal operation
 *    ```
 * 
 * 7. **Troubleshooting**:
 *    - No ejection: Check PTO mode, color thresholds, sensor cleanliness
 *    - Wrong storage count: Verify color detection, check ejection timing
 *    - System conflicts: Check for busy indexer messages
 *    - Color detection issues: Adjust lighting, clean sensors, check calibration
 */