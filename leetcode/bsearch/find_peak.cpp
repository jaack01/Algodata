class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int l = 0, r = nums.size() - 1;
        while (l < r) {
            int m = (r + l) / 2;
            if (nums[m] < nums[m + 1]) l = m + 1;
            else r = m;
        }
        
        return l;
    }
};
