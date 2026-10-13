from typing import Set, Tuple, List, Dict, Optional 
import re

"""实现最小合一算法，返回替换规则"""
def unify(term1: str, term2: str, theta: Dict[str, str]) -> Optional[Dict[str, str]]:
    term1, term2 = deep_substitute(term1, theta), deep_substitute(term2, theta)
    term1_list, term2_list = term1.split(","), term2.split(",")
    #将term1和term2按逗号分隔，防止 'tony,snow' 这样的字符串无法解析

    if len(term1_list) != len(term2_list):
        return None

    #逐个统一参数 theta统一参数记录替换规则
    for t1, t2 in zip(term1_list, term2_list):
        if t1 == t2:  #参数相同 跳过
            continue
        elif t1 in VAR_SET:
            theta = unify_var(t1, t2, theta)
        elif t2 in VAR_SET:
            theta = unify_var(t2, t1, theta)
        else:
            return None  #无法统一

        if theta is None:
            return None

    return theta

def unify_var(var: str, term: str, theta: Dict[str, str]) -> Optional[Dict[str, str]]:
    if var in theta:
        return unify(theta[var], term, theta)
    elif term in theta:
        return unify(var, theta[term], theta)
    elif occurs_check(var, term):
        return None
    theta[var] = term
    return theta

"""递归替换变量"""
def deep_substitute(term: str, theta: Dict[str, str]) -> str:
    while term in theta:
        term = theta[term]
    return term

"""检查变量是否出现在 term 中，防止无限替换"""
def occurs_check(var: str, term: str) -> bool:
    if var == term:
        return True
    if "," in term:
        return any(occurs_check(var, arg) for arg in term.split(","))
    return False

"""解析谓词 P(x,y) -> ('P', ['x', 'y'])"""
def parse_predicate(expr: str) -> Tuple[str, List[str]]:
    match = re.match(r"(\w+)\((.*?)\)", expr)
    if match:
        name = match.group(1)
        args = [arg.strip() for arg in match.group(2).split(",")]
        return name, args
    return expr, []

"""根据替换规则 theta 应用到子句中"""
def substitute(clause: Tuple[str, ...], theta: Dict[str, str]) -> List[str]:
    new_clause = []
    for lit in clause:
        negated = lit.startswith("~")
        core_lit = lit[1:] if negated else lit
        name, args = parse_predicate(core_lit)
        if args:
            new_args = [theta.get(arg, arg) for arg in args]
            new_lit = f"{name}({','.join(new_args)})"
        else:
            new_lit = f"{name}"
        if negated:
            new_lit = "~" + new_lit
        new_clause.append(new_lit)
    return tuple(sorted(set(new_clause)))  # 使用 List 存储并去重

"""对两个子句进行归结"""
def resolve(ci: Tuple[str, ...], cj: Tuple[str, ...], i: int, j: int) -> Optional[Tuple[str, List[str]]]:
    for ai, lit1 in enumerate(ci):
        for aj, lit2 in enumerate(cj):
            #拆解句子，得到谓词和参数
            if lit1.startswith("~"):
                pred1, args1 = parse_predicate(lit1[1:])
                pred2, args2 = parse_predicate(lit2)
                negated = True
            else:
                pred1, args1 = parse_predicate(lit1)
                pred2, args2 = parse_predicate(lit2[1:])
                negated = lit2.startswith("~")

            if negated and pred1 == pred2: #判断是否可以归结
                theta = unify(",".join(args1), ",".join(args2), {})
                if theta is not None:
                    new_clause = substitute(tuple(set(ci + cj) - {lit1, lit2}), theta)
                    formatted_theta = format_substitution(theta) #归结步骤标签

                    #添加[]内标号格式
                    if len(ci) == 1 and len(cj) == 1:
                        step_label = f"R[{i+1},{j+1}] {formatted_theta} = "
                    elif len(ci) == 1:  #只有ci一个谓词 使用标号j+1和子标号aj
                        step_label = f"R[{i+1},{j+1}{chr(97+aj)}] {formatted_theta} = "
                    elif len(cj) == 1:  
                        step_label = f"R[{i+1}{chr(97+ai)},{j+1}] {formatted_theta} = "
                    else:  #正常情况
                        step_label = f"R[{i+1}{chr(97+ai)},{j+1}{chr(97+aj)}] {formatted_theta} = "

                    return step_label.strip(), new_clause
    return None

"""将替换规则格式化为 'x'='sue' 形式"""
def format_substitution(theta: Dict[str, str]) -> str:
    return "{" + ", ".join(f"'{var}'='{val}'" for var, val in theta.items()) + "}" if theta else ""

"""执行归结推理"""
def resolution(KB: Set[Tuple[str, ...]]) -> List[str]:
    clauses = list(KB) #子句集采用list按顺序存储，便于观察
    seen_clauses = set(clauses) #seen_caluse存储归结过的句子，set存储便于判断

    steps = [f"{i+1} {clause}" for i, clause in enumerate(clauses)] #归结步
    step_num = len(clauses) + 1

    #遍历子句集 进行归结
    while True:
        new_clauses = list()
        for i in range(len(clauses)):
            for j in range(i + 1, len(clauses)):
                result = resolve(clauses[i], clauses[j], i, j)
                if result:
                    step_label, new_clause = result #将归结步和归结得到的句子分开存储

                    #判断是否归结得到空子句
                    if not new_clause:
                        steps.append(f"{step_num} {step_label} ∅")
                        return steps

                    #防止归结得的句子重复加入子句集
                    new_clause_tuple = tuple(new_clause)
                    if new_clause_tuple not in seen_clauses:
                        steps.append(f"{step_num} {step_label} {new_clause}")
                        new_clauses.append(new_clause_tuple)
                        seen_clauses.add(new_clause_tuple)
                        step_num += 1
        if not new_clauses:
            break

        clauses.extend(new_clauses)  #将初步归结得到的句子放回子句集

    return steps


"""去除冗余步骤 重新编号"""
def remove_redundant_steps(result):
    step_dict = {}
    dependencies = {}
    last_step = None

    for step in result:
        parts = step.split(" ", 1)  #拆分格式 编号+公式
        step_idx = int(parts[0])
        step_dict[step_idx] = parts[1]

        if "∅" in parts[1]:#记录 ∅ 的位置
            last_step = step_idx

        #提取两个依赖步骤，并转换为整数编号
        match = re.search(r"R\[(\d+[a-z]?),(\d+[a-z]?)\]", parts[1])
        if match:
            parent1, parent2 = match.groups()
            parent1 = int(re.sub(r"[a-z]", "", parent1))
            parent2 = int(re.sub(r"[a-z]", "", parent2))
            dependencies[step_idx] = {parent1, parent2}

    if last_step is None:#无 ∅ 推导 保持原样
        return result  

    #反向追踪所有必须保留的步骤
    needed_steps = set()
    queue = {last_step}
    while queue:
        step = queue.pop()
        if step not in needed_steps:
            needed_steps.add(step)
            queue.update(dependencies.get(step, set())) #向前追溯

    #过滤步骤并重新编号
    filtered_steps = sorted(needed_steps)
    new_step_dict = {old: idx + 1 for idx, old in enumerate(filtered_steps)}

    #重新映射依赖关系中的步骤编号
    result = []
    for old_idx in filtered_steps:
        new_formula = step_dict[old_idx]
        new_formula = re.sub(r"R\[(\d+[a-z]?),(\d+[a-z]?)\]",
                             lambda m: f"R[{new_step_dict[int(re.sub(r'[a-z]', '', m[1]))]}{m[1][-1] if m[1][-1].isalpha() else ''}," 
                                       f"{new_step_dict[int(re.sub(r'[a-z]', '', m[2]))]}{m[2][-1] if m[2][-1].isalpha() else ''}]",
                             new_formula)
        result.append(f"{new_step_dict[old_idx]} {new_formula}")
    return result
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
#样例需要的变量集合
VAR_SET = {"x", "y", "z", "u", "v", "w","xx","yy","zz"}

"""
#样例1 第三周第一个作业例子
KB={('FirstGrade',),('~FirstGrade','Child'),('~Child',)}

#样例2 第四周第一个作业例子
KB = {('~HardWorker(sue)',), ('GradStudent(sue)',),
      ('~GradStudent(x)','Student(x)'),
      ('~Student(x)','HardWorker(x)')}

#样例3 第四周第二个作业例子
KB = {
    ('A(tony)',), ('A(mike)',), ('A(john)',),
    ('L(tony,rain)',), ('L(tony,snow)',),
    ('~A(x)', 'S(x)', 'C(x)'),
    ('~C(y)', '~L(y,rain)'),
    ('L(z,snow)', '~S(z)'),
    ('~L(tony,u)', '~L(mike,u)'),
    ('L(tony,v)', 'L(mike,v)'),
    ('~A(w)', '~C(w)', 'S(w)')
}

#样例4 第四周第三个作业例子
KB={('On(tony,mike)',),('On(mike,john)',),
    ('Green(tony)',),('~Green(john)',),
    ('~On(xx,yy)','~Green(xx)','Green(yy)')}
"""

#KB=input_kb()
KB={('On(tony,mike)',),('On(mike,john)',),
    ('Green(tony)',),('~Green(john)',),
    ('~On(xx,yy)','~Green(xx)','Green(yy)')}
#运行归结推理
result = resolution(KB)

#去重
filtered_result = remove_redundant_steps(result)

import timeit
#测试 resolution 运行时间
resolution_time = timeit.timeit("resolution(KB)", globals=globals(), number=10) / 10
print(f"Resolution 平均运行时间: {resolution_time:.6f} 秒")

#测试去冗余处理时间
redundant_removal_time = timeit.timeit("remove_redundant_steps(result)", globals=globals(), number=10) / 10
print(f"remove_redundant_steps 平均运行时间: {redundant_removal_time:.6f} 秒")

#打印处理后的推理步骤
print("\n归结推理过程：")
for line in filtered_result:
    print(line)

