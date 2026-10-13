## 2.4算法 algorithm

### sort排序

```cpp
int main() {
    vector<int> vec = { 2,1,6,8,7,4,9,3,5 };
    sort(vec.begin(), vec.end());

    for (auto i : vec) {
        cout << i << " "; 
    }
    cout<<endl;
    
    vec = { 2,1,6,8,7,4,9,3,5 };
    sort(vec.begin(), vec.end(), [](int a, int b) { return a > b; });//lambda函数实现降序
    for (auto i : vec) {
        cout << i << " "; 
    }
    system("pause");
    return 0;
}
```

### count_if
- 计算大于15并小于20的元素数量

```cpp
int main() {
    using namespace std;

    vector<int> vec = { 1, 10, 20, 15, 30 };

    auto greater_then_20 = 
    count_if(vec.begin(), vec.end(), [](int i) { return i > 20; });
    auto greater_then_15 = 
    count_if(vec.begin(), vec.end(), [](int i) { return i > 15; });

    cout <<  greater_then_20 << "\n"
    <<  greater_then_15 << "\n";
    system("pause");
    return 0;
}
```

### accumulate计算总和

```cpp
int main() {
    using namespace std;
    vector<int> v = { 1, 2, 3, 4 };
    auto res = accumulate(v.begin(), v.end(), 1, [](decltype(v[0]) i, decltype(v[0]) j) { return i * j; });
    cout << res;
    system("pause");
    return 0;
}
```
