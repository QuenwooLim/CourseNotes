#导入所需的库 支持计算和画图
import matplotlib.pyplot as plt
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

class ExpressionWithX:
    def __init__(self, constant, coefficient_of_x):
        self.constant = constant                  #常数项
        self.coefficient_of_x = coefficient_of_x  #X的系数

    def __str__(self):
        # 返回表示该表达式的字符串
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

    def __add__(self, other):   #表达式相加
        return ExpressionWithX(self.constant + other.constant, self.coefficient_of_x + other.coefficient_of_x)
    def __sub__(self, other):   #表达式相减
        return ExpressionWithX(self.constant - other.constant, self.coefficient_of_x - other.coefficient_of_x)
    def __mul__(self, scalar):  #表达式与常数相乘
        return ExpressionWithX(self.constant * scalar, self.coefficient_of_x * scalar)
    def __truediv__(self, scalar):  #表达式与常数相除
        return ExpressionWithX(self.constant / scalar, self.coefficient_of_x / scalar)
    def solve_for_x(self):  #计算解X的值
        return -self.constant / self.coefficient_of_x

#存储每月月利率的列表
monthly_interest_rate_values = []

for month in range(1, 361):
    if month <= 60:
        #前60个月月利率为0
        monthly_interest_rate_values.append(0)
    else:
        #每12个月年利率递增1%（从5%到30%），转换为月利率
        year_rate = 0.05 + ((month - 61) // 12) * 0.01  #当前年利率
        monthly_rate = year_rate / 12   #转换为月利率
        monthly_interest_rate_values.append(monthly_rate)

# 初始贷款本金
P0 = 500000

#存储剩余支付金额的列表
total_reserved_list = []

#初始化前60个月的剩余支付金额
for month in range(1, 61):
    expr = ExpressionWithX(0, month)    #每月的还款额为X,1到60月的累计还款额为1X, 2X, ..., 60X
    remaining = ExpressionWithX(P0, 0) - expr   #剩余金额=贷款本金-累计还款额
    total_reserved_list.append(remaining)

#从第61个月开始，使用公式更新剩余支付金额
for month in range(60, 360):
    previous_remaining = total_reserved_list[month - 1]     #上个月的剩余金额P[n-1]
    monthly_interest_rate = monthly_interest_rate_values[month]  #当前月的月利率r
    current_payment = ExpressionWithX(0, 1)    #每月还款额度为X
    
    # 计算新的剩余金额 P[n] = P[n-1] + P[n-1] * r - X
    new_remaining = previous_remaining + (previous_remaining * monthly_interest_rate) - current_payment
    total_reserved_list.append(new_remaining)

#计算 X 值并记录
X_values = []
months = list(range(1, 361))  # 月份范围从1到360

for month in months:
    remaining = total_reserved_list[month - 1]
    X_value = remaining.solve_for_x()  #解出X
    X_values.append(X_value)  #记录X

#绘制X随月份变化的关系
plt.figure(figsize=(10, 6))
plt.plot(months[59:360], X_values[59:360], label="月还款额度 X 随期限变化的关系")
plt.xlabel("还款期限（月）")
plt.ylabel("月还款额度 X")
plt.title("还款期限与月还款额度 X 的关系")
plt.grid(True)
plt.legend()
plt.show()
print("30 年内总共付给银行的金额:",X_values[359]*360)
