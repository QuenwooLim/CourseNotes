import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

class ExpressionWithX:
    def __init__(self, constant, coefficient_of_x):
        self.constant = constant              
        self.coefficient_of_x = coefficient_of_x  

    def __str__(self):
        if self.coefficient_of_x == 0:
            return f"{self.constant}"
        elif self.coefficient_of_x == 1:
            return f"{self.constant} + X"
        elif self.coefficient_of_x == -1:
            return f"{self.constant} - X"
        elif self.coefficient_of_x > 0:
            return f"{self.constant} + {self.coefficient_of_x}X"
        else:
            return f"{self.constant} - {-self.coefficient_of_x}X"

    def __add__(self, other):
        return ExpressionWithX(self.constant + other.constant, self.coefficient_of_x + other.coefficient_of_x)

    def __sub__(self, other):
        return ExpressionWithX(self.constant - other.constant, self.coefficient_of_x - other.coefficient_of_x)

    def __mul__(self, scalar):
        return ExpressionWithX(self.constant * scalar, self.coefficient_of_x * scalar)

    def __truediv__(self, scalar):
        return ExpressionWithX(self.constant / scalar, self.coefficient_of_x / scalar)
    
    def solve_for_x(self):
        return -self.constant / self.coefficient_of_x

# 年利率为8%，转换为月利率
monthly_interest_rate_values = 0.08 / 12  # 每月的月利率均为8%/12

P0 = 500000

total_payments_list = []

for month in range(1, 61):
    expr = ExpressionWithX(0, month) 
    total_payments_list.append(expr)

total_reserved_list = []

# 初始化前60个月的剩余支付金额
for month in range(60):
    remaining = ExpressionWithX(P0, 0) - total_payments_list[month]
    total_reserved_list.append(remaining)

# 从第61个月开始，使用公式更新剩余支付金额
for month in range(60, 180):  # 只遍历到第15年，即180个月
    previous_remaining = total_reserved_list[month - 1] 
    current_payment = ExpressionWithX(0, 1)
    new_remaining = previous_remaining + (previous_remaining * monthly_interest_rate_values) - current_payment
    total_reserved_list.append(new_remaining)

X_values = []
months = list(range(1, 181))  

for month in months:
    remaining = total_reserved_list[month - 1]
    X_value = remaining.solve_for_x() 
    X_values.append(X_value)

print(f"15 年内付给银行的月还贷金额: {X_values[179]:.2f} 元")
print(f"15 年内总共付给银行的金额: {X_values[179] * 180:.2f} 元")
