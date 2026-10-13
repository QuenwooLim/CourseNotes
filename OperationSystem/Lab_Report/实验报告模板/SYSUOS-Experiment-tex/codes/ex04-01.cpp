#include <iostream>
#include <vector>
using namespace std;
int main(){
    // 累加 20 以内的素数
    int sum = 0;
    for(int e : {2, 3, 5, 7, 11, 13, 17, 19}){ // 用 auto 类型更合理
        sum += e;
    }
    cout << sum << endl;
    // 输出结果 77
    int arr[] = {1, 3, 5, 7, 9};
    // 声明数组 arr,初始化为 5 个奇数
    for(auto &ele : arr){
        // 声明 ele,与数组 arr 关联在一起,用了 auto
        ele = ele * 2;
        // 修改数组每个元素的值
        cout << ele << " ";
        // 输出 ele,2 6 10 14 18
    }
    cout << endl;

    for(auto ele : arr){
        cout << ele << " ";
    }
    // 没有改变:1 3 5 7 9
    cout << endl;
    return 0;
}



