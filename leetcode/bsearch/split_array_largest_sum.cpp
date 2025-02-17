class Solution {
public:
    int splitArray(vector<int>& nums, int m) {
        long long left = 0, right = 0;
        for (const auto& num : nums) {
            if (left < num) left = num;
            right += num;
        }

        while (left <= right) {
            const auto mid = left + (right - left) / 2;
            if (canSplit(nums, m, mid)) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return left;
    }

private:
    bool canSplit(vector<int>& nums, int m, int sum) {
        int cnt = 1;
        long long curr_sum = 0;
        for (const auto& num : nums) {
            curr_sum += num;
            if (curr_sum > sum) {
                curr_sum = num;
                ++cnt;
            }
        }
        return cnt <= m;
    }
};
