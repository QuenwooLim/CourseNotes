from typing import List


def is_possible(pages: List[int], m: int, max_allowed: int) -> bool:
    """判断在每个学生最多读 max_allowed 页的限制下，是否能用不超过 m 个学生分配所有书。

    使用贪心：从左到右累加，超过 max_allowed 时分配给下一个学生。
    """
    students_required = 1
    current_sum = 0
    for p in pages:
        if p > max_allowed:
            # 单本书页数已超过限制，不可行
            return False
        if current_sum + p <= max_allowed:
            current_sum += p
        else:
            students_required += 1
            current_sum = p
            if students_required > m:
                return False
    return True


def min_max_pages(pages: List[int], m: int) -> int:
    """返回将 pages 分配给 m 个学生时的最小可能的最大页数。"""
    if not pages:
        return 0
    
    # 如果学生数大于书本数，无法给每个学生分配连续的书
    if m > len(pages):
        raise ValueError(f"学生数({m})不能大于书本数({len(pages)})，无法给每个学生分配连续的书")
    
    # 如果学生数等于书本数，每个学生分配一本书
    if m == len(pages):
        return max(pages)
    
    left = max(pages)
    right = sum(pages)
    answer = right
    while left <= right:
        mid = (left + right) // 2
        if is_possible(pages, m, mid):
            answer = mid
            right = mid - 1
        else:
            left = mid + 1
    return answer


def get_allocation_details(pages: List[int], m: int, max_allowed: int) -> List[List[int]]:
    """返回具体的分配方案，用于调试和展示。"""
    # 特殊情况：学生数等于书本数
    if m == len(pages):
        return [[pages[i]] for i in range(len(pages))]
    
    allocation = []
    current_group = []
    current_sum = 0
    
    # 按贪心算法进行初始分配
    for p in pages:
        if current_sum + p <= max_allowed:
            current_group.append(p)
            current_sum += p
        else:
            if current_group:
                allocation.append(current_group)
            current_group = [p]
            current_sum = p
    
    if current_group:
        allocation.append(current_group)
    
    # 如果分配的组数少于学生数，需要进一步分割
    while len(allocation) < m:
        # 找到书本数最多且页数最多的组进行分割
        best_group_idx = -1
        best_books_count = 0
        best_sum = 0
        
        for i, group in enumerate(allocation):
            if len(group) > best_books_count or (len(group) == best_books_count and sum(group) > best_sum):
                if len(group) > 1:  # 只有多于一本书的组才能分割
                    best_group_idx = i
                    best_books_count = len(group)
                    best_sum = sum(group)
        
        if best_group_idx == -1:
            # 无法继续分割，说明已经达到最优分配
            break
        
        # 分割选中的组
        group_to_split = allocation[best_group_idx]
        # 将最后一本书分出来作为新组
        last_book = group_to_split.pop()
        allocation.append([last_book])
    
    return allocation


def run_test_case(pages: List[int], m: int, case_name: str):
    """运行单个测试用例并显示结果。"""
    print(f"\n=== {case_name} ===")
    print(f"输入: N={len(pages)}, pages={pages}, M={m}")
    
    res = min_max_pages(pages, m)
    print(f"输出: {res}")
    
    # 显示具体的分配方案
    allocation = get_allocation_details(pages, m, res)
    print(f"最优分配方案:")
    for i, group in enumerate(allocation, 1):
        group_sum = sum(group)
        print(f"  学生{i}: {group} (总页数: {group_sum})")
    
    max_pages = max(sum(group) for group in allocation)
    print(f"验证 - 所有学生的最大页数: {max_pages}")
    if max_pages == res:
        print("✓ 结果正确")
    else:
        print("✗ 结果错误")


def main():
    # 测试用例1：README示例
    run_test_case([12, 34, 67, 90], 2, "测试用例1（示例）")
    
    # 测试用例2：边界情况 - 学生数等于书本数
    run_test_case([10, 20, 30, 40], 4, "测试用例2（学生数=书本数）")
    
    # 测试用例3：边界情况 - 只有一个学生
    run_test_case([10, 20, 30, 40], 1, "测试用例3（只有1个学生）")
    
    # 测试用例4：所有书页数相同
    run_test_case([50, 50, 50, 50], 2, "测试用例4（页数相同）")
    
    # 测试用例5：正常情况，需要细分
    run_test_case([1, 2, 3, 100], 3, "测试用例5（需要细分分配）")
    
    # 测试用例6：学生数大于书本数的错误情况
    try:
        run_test_case([10, 20, 30], 5, "测试用例6（错误情况：学生数>书本数）")
    except ValueError as e:
        print(f"✓ 正确捕获错误: {e}")


if __name__ == "__main__":
    main()
