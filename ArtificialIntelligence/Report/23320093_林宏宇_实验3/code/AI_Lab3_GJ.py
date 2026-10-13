from typing import Set, Tuple, List, Dict, Optional
import re
# 变量集合

def unify(term1: str, term2: str, theta: Dict[str, str]) -> Optional[Dict[str, str]]:
    """实现最小合一算法，返回替换规则"""
    if term1 == term2:
        return theta
    elif term1 in VAR_SET:  # term1 是变量
        return unify_var(term1, term2, theta)
    elif term2 in VAR_SET:  # term2 是变量
        return unify_var(term2, term1, theta)
    elif "(" in term1 and "(" in term2:  # 结构相同
        t1_name, t1_args = parse_predicate(term1)
        t2_name, t2_args = parse_predicate(term2)
        if t1_name != t2_name or len(t1_args) != len(t2_args):
            return None  # 结构不同，无法合一
        for a, b in zip(t1_args, t2_args):
            theta = unify(a, b, theta)
            if theta is None:
                return None
        return theta
    else:
        return None

def unify_var(var: str, term: str, theta: Dict[str, str]) -> Optional[Dict[str, str]]:
    """变量替换"""
    if var in theta:
        return unify(theta[var], term, theta)
    elif term in theta:
        return unify(var, theta[term], theta)
    elif occurs_check(var, term):
        return None
    else:
        theta[var] = term
        return theta

def occurs_check(var: str, term: str) -> bool:
    """检查变量是否出现在 term 中，防止无限替换"""
    if var == term:
        return True
    if "(" in term:
        _, args = parse_predicate(term)
        return any(occurs_check(var, arg) for arg in args)
    return False

def parse_predicate(expr: str) -> Tuple[str, List[str]]:
    """解析谓词 P(x,y) -> ('P', ['x', 'y'])"""
    match = re.match(r"(\w+)\((.*?)\)", expr)
    if match:
        name = match.group(1)
        args = match.group(2).split(",")
        return name, args
    return expr, []

def substitute(clause: Tuple[str, ...], theta: Dict[str, str]) -> Tuple[str, ...]: 
    """根据替换规则 theta 应用到子句中"""
    new_clause = []
    for lit in clause:
        negated = lit.startswith("~")  # 判断是否是取反的字面量
        core_lit = lit[1:] if negated else lit  # 去掉 `~` 号
        name, args = parse_predicate(core_lit)  # 解析谓词
        if args!=[]:  # 有参数的情况
            new_args = [theta.get(arg, arg) for arg in args]  # 变量替换
            new_lit = f"{name}({','.join(new_args)})"  # 重新构造字面量
        else:
            new_lit = f"{name}"
        if negated:
            new_lit = "~" + new_lit  # 还原 `~`
        new_clause.append(new_lit)
    return tuple(sorted(set(new_clause)))  # 规范化排序

def format_substitution(theta: Dict[str, str]) -> str:
    """将替换规则格式化为 'x'='sue' 形式"""
    return "{" + ", ".join(f"'{var}'='{val}'" for var, val in theta.items()) + "}" if theta else ""

def resolve(ci: Tuple[str, ...], cj: Tuple[str, ...], i: int, j: int) -> Optional[Tuple[str, str, Tuple[str, ...]]]:
    """尝试对两个子句进行归结"""
    for ai, lit1 in enumerate(ci):
        for aj, lit2 in enumerate(cj):
            if lit1.startswith("~"):
                pred1, args1 = parse_predicate(lit1[1:])
                pred2, args2 = parse_predicate(lit2)
                negated = True
            else:
                pred1, args1 = parse_predicate(lit1)
                pred2, args2 = parse_predicate(lit2[1:])
                negated = lit2.startswith("~")

            if negated and pred1 == pred2:
                theta = unify(",".join(args1), ",".join(args2), {})
                if theta is not None:
                    formatted_theta = format_substitution(theta)
                    new_clause = substitute(tuple(set(ci + cj) - {lit1, lit2}), theta)
                    
                    if len(ci) == 1 and len(cj) == 1:
                        step_label = f"R[{i+1},{j+1}] {formatted_theta} = "
                    elif len(ci) == 1:
                        step_label = f"R[{i+1},{j+1}{chr(97+aj)}] {formatted_theta} = "
                    elif len(cj) == 1:
                        step_label = f"R[{i+1}{chr(97+ai)},{j+1}] {formatted_theta} = "
                    else:
                        step_label = f"R[{i+1}{chr(97+ai)},{j+1}{chr(97+aj)}] {formatted_theta} = "
                    
                    return step_label.strip(), new_clause
    return None

def resolution(KB: Set[Tuple[str, ...]]) -> List[str]:
    clauses = list(KB)  # 转为列表
    steps = []
    for i, clause in enumerate(clauses):
        steps.append(f"{i+1} {clause}")
    
    step_num = len(clauses) + 1
    seen_clauses = set(clauses)  # 跟踪已经归结过的子句
    while True:
        new_clauses = list()
        for i in range(len(clauses)):
            for j in range(i + 1, len(clauses)):
                result = resolve(clauses[i], clauses[j], i, j)
                if result:
                    step_label, new_clause = result
                    #print(f"生成新子句：{new_clause}")  # 打印调试信息
                    if not new_clause:
                        steps.append(f"{step_num} {step_label}∅")
                        return steps
                    if new_clause not in seen_clauses:  # 确保新子句没有重复
                        steps.append(f"{step_num} {step_label}{new_clause}")
                        new_clauses.append(new_clause)
                        seen_clauses.add(new_clause)  # 记录已经归结过的子句
                        step_num += 1
        if not new_clauses:
            break 
        clauses.extend(new_clauses)
    return steps

def input_kb() -> Set[Tuple[str, ...]]:
    """从终端输入子句集"""
    KB = set()
    print("请输入子句，每行输入一个子句，使用逗号分隔谓词，输入 'done' 结束：")
    while True:
        line = input("子句: ").strip()
        if line.lower() == "done":
            break
        if line:
            clause = tuple(map(str.strip, line.split(",")))  # 解析子句
            KB.add(clause)
    return KB

def remove_redundant_steps(result):
    # 解析步骤并构建索引
    step_dict = {}
    dependencies = {}
    last_step = None

    for step in result:
        parts = step.split(" ", 1)  # 拆分格式 "编号 公式"
        step_idx = int(parts[0])
        step_dict[step_idx] = parts[1]

        # 记录 ∅ 公式的位置
        if "∅" in parts[1]:
            last_step = step_idx

        # 提取推导依赖
        # 修改正则表达式，匹配依赖步骤的格式，包括仅数字和数字加字母的情况
        match = re.search(r"R\[(\d+[a-z]?),(\d+[a-z]?)\]", parts[1])
        if match:
            # 提取两个依赖步骤，并转换为整数编号
            parent1, parent2 = match.groups()
            parent1 = int(re.sub(r"[a-z]", "", parent1))  # 移除字母部分，得到数字
            parent2 = int(re.sub(r"[a-z]", "", parent2))  # 移除字母部分，得到数字
            dependencies[step_idx] = {parent1, parent2}

    if last_step is None:
        return result  # 无 ∅ 推导，保持原样

    # 反向追踪所有必须保留的步骤
    needed_steps = set()
    queue = {last_step}
    while queue:
        step = queue.pop()
        if step not in needed_steps:
            needed_steps.add(step)
            queue.update(dependencies.get(step, set()))  # 向前追溯

    # 过滤步骤并重新编号
    filtered_steps = sorted(needed_steps)  # 按原顺序排序
    new_step_dict = {old: idx + 1 for idx, old in enumerate(filtered_steps)}

    result = []
    for old_idx in filtered_steps:
        new_formula = step_dict[old_idx]
        # 重新映射依赖关系中的步骤编号
        new_formula = re.sub(r"R\[(\d+[a-z]?),(\d+[a-z]?)\]",
                             lambda m: f"R[{new_step_dict[int(re.sub(r'[a-z]', '', m[1]))]}{m[1][-1] if m[1][-1].isalpha() else ''},"
                                       f"{new_step_dict[int(re.sub(r'[a-z]', '', m[2]))]}{m[2][-1] if m[2][-1].isalpha() else ''}]",
                             new_formula)
        result.append(f"{new_step_dict[old_idx]} {new_formula}")

    return result

#输入
#KB = input_kb()
KB = {('~HardWorker(sue)',), ('GradStudent(sue)',), ('~GradStudent(x)','Student(x)'), ('~Student(x)','HardWorker(x)')}
#KB={('FirstGrade',),('~FirstGrade','Child'),('~Child',)}

#设置变量集合
VAR_SET = {"x", "y", "z"}

# 运行归结推理
result = resolution(KB)

# 移除冗余步骤
result=remove_redundant_steps(result)

# 打印推理步骤
print("\n归结推理过程：")
for line in result:
    print(line)