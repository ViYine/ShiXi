
# c++ 笔试题，以及竞赛中的一些，程序模板

１．引入标准库
c++标准库中实现了，方便的容器以及常用的算法实现。所以使用Ｃ＋＋进行编程笔试，以及在做一些竞赛的时候，对标准库的使用很大一部分上影响了后面算法的实现。所以第一步是引入标准库，由于很多ｃ＋＋的判题的编译器都是使用的gcc,所以可以利用gcc编译器的特性，通过一行直接将标准库进行导入，而不需要自己在使用的时候导入```iostream, vector ，string, map ```等头文件，同时引入std的命名空间（当然在实际做项目编程开发的时候，尽量少使用using导入命名空间，以免造成命名空间的污染）。所以代码的模板如下所示：

```c++
#include <bits/stdc++.h>

using namespace std;

int main() {
    //code
    return 0;
}
```

## c++ 处理输入问题

在笔试编程题中，现在国内的网站上是需要处理输入问题的，而c++又没有像python那样具有split函数，以及列表解析的高级的表达式的操作，所以很多时候，遇到比较复杂的输入的时候，c++处理起来就似乎比较吃力了，但是c++ y也不是没有能力处理输入的，只是可能平时在用的时候没有太关注过c++ 处理输入的问题。现在就把c++处理常见的笔试题中的输入问题理一理。

大部分笔试题，对于输入测试样例，一般是由多行组成，第一行一般就是要输入的测试样例的个数。
同时对于那种看似复杂的输入，其实认真读题，确定其单个的输入形式，就可以用scanf，或者cin进行接收了。

如
```c++
3
2 3 4
```
或者

```c++
3
abc def fhg
```

这时候接收这样的输入，好希望能有个python一样的split()函数一样可以整行读入分割。然而这是c++,无奈如果要用slpit的话，需要导入boost的东西,不然就得自己编写一个split了，除非平时编好了，随时准备用，不然笔试的时候哪有那个时间去重新编写一个split函数，不过大佬除外。然而如果是大佬的话肯定也不会去选择用split()，接收处理输入。其实c++里面处理的话基于cin标准输入流,和使用scanf可以接收这种一行里面用空格隔开的输入。

1. 输入数字型的多个数字或者字符

这种情况主要是读入控制台输入的一个数组的输入。一般第一个数字表示输入的数组的长度。
其实很简单。

### 使用scanf

```c++
int n;
scanf("%d",&n)//读入数组的长度

vector<int> datain;
    for (int i = 0; i < n ; ++i) {
        int te;
        scanf("%d",&te);
        datain.push_back(te);
    }

```
//这时　后面按照空格隔开的数字全部添加到数组里面来了
//以前一直有迷惑，没用这个是觉得因为如果数组的每个是回车隔开的话，才可以，其实不然scanf在遇到空格，tab,或者回车之后就会把之前的读进来而不管控制符号后面的,只是如何控制台是以回车进去的话，每次都会刷到缓冲区里面。同时使用scanf的时候还能处理那种输入的数之间不是空格分割的输入，如逗号隔开的scanf也能应对。

### 使用cin

```c++
int n;
cin >>n 
//读入数组的长度

vector<int> datain;
for (int i = 0; i < n ; ++i) {
        int te;
        cin>>te;
        datain.push_back(te);
    }
```
输入字符数组时和scanf用法一样。速度scanf更快，因为scanf更底层。

同时如果输入的数字之间不是以空格隔开的话，使用cin就需要，定义一个字符变量来接收中间的分隔符。使用方式：

```c++
int n;
cin>>n;
char tmp;
vector<int> Input;
for (int i = 0; i < n ; ++i) {
    int te;
    if (i!=n-1){
        cin>>te>>tmp;
        datain.push_back(te);
    }
    else {//由于最后以回车结束，所以直接接收输入的数字就行
        
        cin>>te;
        Input.push_back(te);
    }
    
}

```

但是如果是读入空格分割的字符串，每个空格将需要的字符分开的话，建议用cin.因为可以直接生产string的数组。
```c++
int n;
cin >> n;
//读入数组的长度a
vector<string> strin;
    for (int i = 0; i < n ; ++i) {
        string te;
        cin>>te;
        strin.push_back(te);
    }
```

2. 读入整行包含中间包含空格

这时候需要用到getline的函数了。c＋＋里面用两个getline(),一个是cin.getline()，一个是在std命名空间下的getline()全局函数。

```c++
string s;
getline(cin,s);
```

所以实际用了getline().但是这个在控制台的时候也得慎用。有可能，你之前按了回车之后　他就当你已经刷新一次缓冲区了，就可能读到空。
因为它阻塞只要遇到一次回车，就完了。

3. 当输入结束标志不清楚时使用

```c++
while (cin >> x) {
// code
}
```
一般在笔试题中不常见，同时这个处理，也更为复杂。

4. note:

当对于那种输入是程序性能的瓶颈的情况，可在输入前加上如下两个语句。

```c++
ios::sync_with_stdio(0);
cin.tie(0);
```
可以提高输入的性能

# c++中常用的容器和标准库算法　vector 和sort


## vector

vector是c++标准库里面的动态数组，插入删除操作随机访问具有与数组同样的时间复杂度，但是其具有动态增长的特性。其内部实现策略是，在vector进行构造的时候，会预先分配其一定的容量，当实际在数组中的元素个数，快达到其容量上限时，需要进行一次扩容操作：重新开辟一块空间，容量一般为原来容量的２倍，同时将原来的数据复制到新的空间，并回收原来的空间，最后返回新创建的空间的地址。


在实际笔试编程题中如果用到vector时主要用到其构造函数，往后插入元素
```c++
vector<int> vi;//空的数组
vector<int> vi1(10);//初始长度为10的数组,
vector<int> vi2(10, 2)//初始长度为10, 每个元素初始值为２
    
    
vi.push_back(10)//从末尾插入10

```
### sort

sort是c++标准库里面实现的快速排序的，模板算法，使用时可以通过传入迭代器进行操作，常见的用法有：

```c++
vector<int> v = {4,2,5,3,5,8,1};
sort(v.begin(),v.end());//从小到大排序
sort(v.rbegin(),v,rend());//反向排序，即从大到小排序

//接收常规数组进行排序
int n = 6;//数组的长度
int arr[] = {2,1,5,3,6,4};
sort(arr,arr+n);

//同时sort也支持字典序的排序
string s = "monkey";
sort(s.begin(), s.end());
//s为　：ekmnoy
```


```c++
//sort也支持自定义的比较函数的排序，对于数组中存的时元组时，也可以自定义按照第一个值进行排序
vector<pair<int,int>> v;
v.push_back({1,5});
v.push_back({2,3});
v.push_back({1,2});
sort(v.begin(), v.end());
```


```c++
v//默认情况下以第一个为关键字进行排序
```




    { {1 , 2}, {1 , 5}, {2 , 3} }



如果按照第二个关键子进行排序，可以通过定义比较函数进行实现

```c++
bool cmp(pair<int,int> p1, pair<int,int> p2){
    return p1.second <= p2.second;
}
```


```c++
bool cmp(pair<int,int> p1, pair<int,int> p2){
    return p1.second <= p2.second;
}
```


```c++
sort(v.begin(), v.end(),cmp);
```


```c++
v//以第二个关键字进行排序
```




    { {1 , 2}, {2 , 3}, {1 , 5} }



同时如果数组里面定义的是，自定义的类型，struct,或者class的话需要该类型重载操作符<.如：



```c++
struct P {
int x, y;
bool operator<(const P &p) {
    if (x != p.x) 
        return x < p.x;
    else 
        return y < p.y;
    }
P(int x, int y):x(x),y(y){}
};
```


```c++
vector<P> parry;
parry.push_back(P(1,2));
parry.push_back(P(1,3));
parry.push_back(P(2,2));
parry.push_back(P(2,1));

```


```c++
sort(parry.begin(),parry.end());
```


```c++
for(auto itr : parry){
    cout<<itr.x<<" "<<itr.y <<"\n";
}
```

    1 2
    1 3
    2 1
    2 2


# card 01 

以上的这些说明先做个总结，以便以后查起来方便。同时记录这个代码（c++）和笔记(Markdown),我使用的是jupyter notebook,其中添加了交互运行c++的解释器的内核。由于是交互式运行，所以在所以的代码单元里面，可以直接执行代码段，不能在自己新定义main函数。

关于jupyter botebook 添加c++解释器可以参考：https://github.com/QuantStack/xeus-cling

1. 　安装 miniconda：https://conda.io/miniconda.html
2.  使用安全的隔离环境命令为cling　
```
conda create -n cling
source activate cling
```
3. 使用conda进行组件的安装
```
conda install xeus-cling notebook -c QuantStack -c conda-forge
```
4. 由于之前使用的时安全的隔离环境进行安装的所以，以后每次新开一个终端进行jupyter notebook启动时需要先激活cling.

即：

```
source activate cling
jupyter notebook

```

之后就能想享受python 交互式运行一样操作c++代码啦！


