# include <iostream>
# include <vector> 

class LandUsageManager {
private:
    // Data structure to store allocated ranges
    std::vector<std::pair<int, int>> allocatedRanges;

public:
    // Constructor
    LandUsageManager();

    // Allocate a range [start, end)
    void allocateLand(int start, int end);

    // Check if a range [start, end) is fully allocated
    bool checkAvailability(int start, int end);

    // Release a range [start, end)
    void releaseLand(int start, int end);

   // Return all individual allocated ranges
    std::vector<std::pair<int, int>> getIndividualAllocations();

    // Return merged allocation view
    std::vector<std::pair<int, int>> getMergedAllocations();
};