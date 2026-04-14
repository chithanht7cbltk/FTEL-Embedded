/*
 * LeetCode 88 - Merge Sorted Array
 * Difficulty: Easy
 * Approach: Two pointers from the end
 * Time Complexity: O(m + n)
 * Space Complexity: O(1)
 */

void merge(int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n) {
    int i = m - 1;          // last element of nums1 (valid part)
    int j = n - 1;          // last element of nums2
    int k = m + n - 1;      // last position of nums1

    // merge from the end
    while (i >= 0 && j >= 0) {
        if (nums1[i] > nums2[j]) {
            nums1[k] = nums1[i];
            i--;
        } else {
            nums1[k] = nums2[j];
            j--;
        }
        k--;
    }

    // nếu nums2 còn phần tử thì copy nốt
    while (j >= 0) {
        nums1[k] = nums2[j];
        j--;
        k--;
    }
}