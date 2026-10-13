"""
Project:二分查找法
Author:林宏宇
Date:2025.02.25
"""
nums=[1,2,4,6,9,11,15,19,20,21]
target=11
left=0 #左索引
right=len(nums)-1 #右索引
while left<=right:
    mid=(left+right)//2 #中间索引
    if nums[mid]==target:
        print(mid)
        break
    elif nums[mid]<target:
        left=mid+1
    else: 
        right=mid-1
if(left>right):
    print(-1)

"""
Project:矩阵加法、乘法
Author:林宏宇
Date:2025.02.25
"""
def MatrixAdd(A,B):
    #创建二维数组C
    C=[[0]*len(A[0]) for i in range(len(A))]
    for i in range(len(A)):
        for j in range(len(A[0])):
            C[i][j]=A[i][j]+B[i][j]
    return C

def MatrixMul(A,B):
    C=[[0]*len(B[0]) for i in range(len(A))]
    for i in range(len(A)):
        for j in range(len(B[0])):
            sum=0
            for k in range(len(A)):
                sum+=A[i][k]*B[k][i]
            C[i][j]=sum
    return C

Matrix_A=[[1,2,3],[4,5,6],[7,8,9]]
Matrix_B=[[2,3,4],[5,6,7],[8,9,10]]
print(MatrixAdd(Matrix_A,Matrix_B))
print(MatrixMul(Matrix_A,Matrix_B))

"""
Project:字典遍历
Author:林宏宇
Date:2025.02.25
"""
def ReverseKeyValue(dict0):  
    dict_res=dict()
    for key in dict0:
        dict_res[dict0[key]]=key
    return dict_res

dict1={'Alice':'001','Bob':'002'}
dict_res=ReverseKeyValue(dict1)
print(dict_res)