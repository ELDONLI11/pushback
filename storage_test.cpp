/**
 * Storage Ball Limit Testing Script
 * 
 * This file demonstrates how to test the new 3-ball storage limit feature.
 * Use this as a reference for testing the implementation.
 */

#include "api.h"
#include "indexer.h"

void testStorageLimit() {
    printf("=== Storage Ball Limit Test ===\n");
    
    // Test storage ball count functionality
    IndexerSystem* indexer = nullptr; // You'll need to get the actual indexer instance
    
    if (indexer) {
        // Test 1: Add balls to storage
        printf("Test 1: Adding balls to storage\n");
        printf("Initial count: %d/3\n", indexer->getStorageBallCount());
        
        // Add 3 balls
        for (int i = 1; i <= 3; i++) {
            if (indexer->addBallToStorage()) {
                printf("Ball %d added. Count: %d/3\n", i, indexer->getStorageBallCount());
            } else {
                printf("Failed to add ball %d - storage full!\n", i);
            }
        }
        
        // Test 2: Try to add a 4th ball (should fail)
        printf("\nTest 2: Attempting to add 4th ball (should fail)\n");
        if (!indexer->addBallToStorage()) {
            printf("✅ Correctly rejected 4th ball - storage limit working!\n");
        } else {
            printf("❌ ERROR: 4th ball was added - storage limit not working!\n");
        }
        
        // Test 3: Check if storage is full
        printf("\nTest 3: Storage full check\n");
        if (indexer->isStorageFull()) {
            printf("✅ Storage correctly reports as full\n");
        } else {
            printf("❌ ERROR: Storage should be full but reports as not full\n");
        }
        
        // Test 4: Remove balls
        printf("\nTest 4: Removing balls from storage\n");
        for (int i = 3; i >= 1; i--) {
            if (indexer->removeBallFromStorage()) {
                printf("Ball removed. Count: %d/3\n", indexer->getStorageBallCount());
            } else {
                printf("Failed to remove ball - storage empty!\n");
            }
        }
        
        // Test 5: Reset storage count
        printf("\nTest 5: Resetting storage count\n");
        indexer->resetStorageBallCount();
        printf("Storage count after reset: %d/3\n", indexer->getStorageBallCount());
        
        printf("\n=== Storage Ball Limit Test Complete ===\n");
    } else {
        printf("❌ ERROR: Indexer system not available for testing\n");
    }
}

/**
 * Manual Testing Instructions:
 * 
 * 1. Use controller buttons to test storage limit:
 *    - Left + R1: Add ball to storage count
 *    - Left + R2: Remove ball from storage count
 *    - Watch display for current count (e.g., "2/3")
 * 
 * 2. Test storage operations:
 *    - Set count to 2/3, try normal storage operation (should work)
 *    - Set count to 3/3, try normal storage operation (should be blocked)
 * 
 * 3. Expected behaviors:
 *    - Storage full warning: "STORAGE FULL!" + short rumble
 *    - Normal operation: Shows current count on display
 *    - Ball enters storage: Count increases automatically
 *    - Ball leaves storage: Count decreases automatically
 * 
 * 4. Testing checklist:
 *    ✅ Storage accepts up to 3 balls
 *    ✅ Storage rejects 4th ball with warning
 *    ✅ Display shows current count (X/3)
 *    ✅ Manual controls work (Left + R1/R2)
 *    ✅ Automatic counting during scoring
 *    ✅ Reset functionality works
 */