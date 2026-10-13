import re
from collections import OrderedDict

"""判断是否是变量"""
def is_variable(x):
    return x in VAR_SET

"""判断是否是谓词"""
def is_compound(x):
    return "(" in x and ")" in x

"""解析谓词，返回谓词名和参数列表"""
def parse_predicate(expr):
    match = re.match(r'(\w+)\((.*)\)', expr)
    if match:
        func, args = match.groups()
        args = split_args(args)
        return func, args
    return expr, []

"""按逗号分割参数，得到参数列表"""
def split_args(arg_str):
    args, depth, current = [], 0, []
    for char in arg_str:
        if char == ',' and depth == 0:
            args.append("".join(current).strip())
            current = []
        else:
            if char == '(':
                depth += 1
            elif char == ')':
                depth -= 1
            current.append(char)
    if current:
        args.append("".join(current).strip())
    return args

"""检查变量是否出现在term中，防止无限替换"""
def occurs_check(var, value):
    if var == value:
        return True
    if is_compound(value):
        _, args = parse_predicate(value)
        return any(occurs_check(var, arg) for arg in args)
    return False

"""应用替换规则到term中"""
def apply_substitution(term, theta):
    if term in theta:
        return apply_substitution(theta[term], theta)  # 递归替换，确保间接绑定的变量也被替换
    elif is_compound(term):
        func, args = parse_predicate(term)
        new_args = [apply_substitution(arg, theta) for arg in args]
        return f"{func}({','.join(new_args)})"
    return term  # 不是变量，也不是复合项，直接返回

"""变量处理, 统一变量名"""
def unify_var(var, value, theta):
    value = apply_substitution(value, theta)  #替换 value 里的变量
    if var in theta:
        return unify(theta[var], value, theta)  #变量已绑定 替换并继续合一
    elif value in theta:
        return unify(var, theta[value], theta)  #反向替换
    elif occurs_check(var, value):
        return None  # 避免循环绑定
    else:
        theta[var] = value  # **按顺序记录绑定**
        # **更新 theta 内所有已绑定变量**
        for key in list(theta.keys()):
            theta[key] = apply_substitution(theta[key], theta)
        return theta

"""合一算法"""
def unify(x, y, theta):
    if theta is None:
        return None
    #应用替换规则theta到x和y
    x = apply_substitution(x, theta)
    y = apply_substitution(y, theta)
    
    if x == y:
        return theta
    elif is_variable(x):#x是变量
        return unify_var(x, y, theta)
    elif is_variable(y):#y是变量
        return unify_var(y, x, theta)
    elif is_compound(x) and is_compound(y):#x和y都是复合项
        #解析谓词，返回谓词名和参数列表
        x_pred, x_args = parse_predicate(x)
        y_pred, y_args = parse_predicate(y)
        if x_pred != y_pred or len(x_args) != len(y_args):#谓词名不同或参数个数不同
            return None
        for x_i, y_i in zip(x_args, y_args):#逐个统一参数
            theta = unify(x_i, y_i, theta)
            if theta is None:
                return None
        return theta
    else:
        return None
    
"""输入"""
def input_kb():
    print("请输入知识库（KB）中的谓词，每个谓词用空格分隔，例如：")
    print("P(xx,a) P(b,yy)")
    user_input = input("输入 KB: ").strip()
    return set(user_input.split())

# 变量集合（假设小写字母为变量）
VAR_SET = {"xx", "yy", "zz", "uu"}

#输入知识库
#KB=input_kb()
#例子1
#KB = {'P(a,xx,f(g(yy)))', 'P(zz,f(zz),f(uu))'}
#例子2
KB = {'P(xx,a)','P(b,yy)'}

#转换为列表 执行合一
kb_list = list(KB)

if len(kb_list) == 2:
    result = unify(kb_list[0], kb_list[1], {})
    print(result)

#测试运行时间
import timeit
time = timeit.timeit("unify(kb_list[0], kb_list[1], {})", globals=globals(), number=10) / 10
print(f"Unify 平均运行时间: {time:.6f} 秒")
