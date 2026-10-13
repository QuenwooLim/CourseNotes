"""
给定字符串，找出其中最长的重复出现并且非重叠的子字符串
输入：str = "geeksforgeeks"
输出：geeks
输入：str = "aabaabaaba"
输出：aaba
"""

def longest_repeated_substring_dp(s):
    """
    思路：
    1. 创建二维DP表 dp[i][j] 表示从位置i和j开始的最长公共前缀长度
    2. 只考虑非重叠的情况（i + dp[i][j] <= j）
    3. 找出满足条件的最长子字符串
    """
    n = len(s)
    if n < 2:
        return ""

    # 初始化DP表
    dp = [[0] * n for _ in range(n)]
    
    max_length = 0
    ending_pos = 0
    
    # 从后往前填充DP表
    for i in range(n - 1, -1, -1):
        for j in range(i + 1, n):
            # 如果字符相同，则在之前的基础上+1
            if s[i] == s[j]:
                if i + 1 < n and j + 1 < n:
                    dp[i][j] = dp[i + 1][j + 1] + 1 # 继承后续的公共前缀长度
                else:
                    dp[i][j] = 1
            else:
                dp[i][j] = 0
            
            # 检查是否满足非重叠条件，并更新最大长度
            if dp[i][j] > max_length and i + dp[i][j] <= j:
                max_length = dp[i][j]
                ending_pos = i
    
    return s[ending_pos:ending_pos + max_length]

# Driver Code
if __name__ == "__main__":
    str1 = "geeksforgeeks"
    str2 = "aabaabaaba"
    str3 = "aab"
    str4 = "aaaaaaaaaaa"
    str5 = "banana"
    print("Longest Repeated Non-Overlapping Substring:")
    print(f"1. '{str1}': {longest_repeated_substring_dp(str1)}")  # Output: geeks
    print(f"2. '{str2}': {longest_repeated_substring_dp(str2)}")  # Output: aaba
    print(f"3. '{str3}': {longest_repeated_substring_dp(str3)}")  # Output: a
    print(f"4. '{str4}': {longest_repeated_substring_dp(str4)}")  # Output: aaaaa
    print(f"5. '{str5}': {longest_repeated_substring_dp(str5)}")  # Output: ana