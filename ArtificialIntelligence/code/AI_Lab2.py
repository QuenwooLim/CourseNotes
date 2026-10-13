"""
Project:StuData类
Author:林宏宇
Date:2025.03.08
"""

class StuData:
    def __init__(self,filename):
        file=open(filename)
        for line in file:
            line=line.strip()
            data=line.split(' ')
            data[3]=int(data[3])
            print(data)

s1=StuData('student_data.txt')

