class Solution {
public:
    int search(vector<int>& nums, int target) {
        if (nums.empty()) return -1;
        int start = 0, end = nums.size() - 1, mid = end / 2;
        
        while (start < end) {
            if (nums[mid] >= target) {
                end = mid;
            } else {
                start = mid + 1;
            }
            mid = start + (end - start) / 2;
        }
        
        return nums[start] == target ? start : -1;
    }
};
