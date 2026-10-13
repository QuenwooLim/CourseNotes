## 3.迭代器
- 类别1：
- 输入迭代器 const 只能作为右值使用
- 输出迭代器 可作为左值
- 类别2：
- 正向迭代器 可以++ 不能--
- 容器：forward_list
- 双向迭代器 可以++和--
- 容器：sel multiset map multimap list
- 随机访问迭代器 ++ -- + - += -=...都可以
- 容器：array vector deque

```cpp
int main() {
	// DEMO1: read only iteraator
	const vector<int> iv {1,2,3,4,5,6,7};
	auto it1 = iv.begin();  
	// *it1 = 3; //read-only
	
	set<int> is {1,2,1,3,1,4};
	auto it2 = is.begin();
	// *it2 = 5;  // error: assignment of read-only location
	
	// DEMO2: forward, bidirection, random
	forward_list<int> ifl {1,2,3,4,5,6,7};
	list<int> ils {11,12,13,14,15,16,17};
	auto it3 = ifl.begin();
	it3++;
	// it3--;  //error forward_list正向迭代器不能--

	auto it4 = ils.begin();
	it4++;
    it4--;
	// it4 += 7;  //error list双向迭代器不能随机访问

	it1 += 7;//随机迭代器
	
	//Demo3: iterating over the elements
	auto _begin = ils.begin();
	auto _end = ils.end();
	for (auto it = _begin; it != _end; it++) {
		auto x = *it;
		cout << x << ","; 
	}
	cout << endl;
	
	for (auto x:ils) {
		cout << x << ",";
	}
	cout << endl;
	
	//Demo4: ref invalid
	vector<int> ivv {1,2,3};
	cout << ivv.capacity() << endl;
	//ivv.push_back(0);
	auto it_ivv = ivv.begin();
	int x = *it_ivv;
	for (int i = 0; i<=10; i++) {
		ivv.push_back(0);
		//cout << ivv.capacity() << endl;
		if (*it_ivv != x) {
			cout << *it_ivv << " at " << i << endl;
			break;
		}
	}
	
	//EX: delete all 3 in vector. 
	// 1.请指出程序中的缺陷，并指出数据案例说明
	// 2.请改正该程序 
	vector<int> ivv1 {1,2,3,4,5};
	for (auto it = ivv1.begin(); it != ivv1.end(); it++) {
		if (*it == 3) ivv1.erase(it);  
	}
	for (auto x : ivv1) cout << x << ",";
	cout << endl; 
} 
```
