
##＃ 开篇　笔试的时候有的公司限定了编程语言，c++。这时候如果遇到字符串的处理和输入，就比较头疼了。有可能你算法是正确的但是，你接收输入参数的部分没写好。那么就是白搭咯。源于今天做了网易互娱的笔试后的感悟。输入搞不对，一切是惘然。所以对ｃ++，输入以及简单的字符穿操作需要做一点总结。


#### C++ 测试样例输入的接收

大部分笔试题，对于输入测试样例，一般是由多行组成，第一行一般就是，要输入的测试样例的个数。

如
```
3
2 3 4
```
或者

```
3
abc def fhg
```

这时候接收这样的输入，好希望能有个python一样的split()函数一样可以整行读入分割。然而这是c++,无奈如果要用slpit的话，需要导入boost的东西,不然就得自己编写一个split了，除非平时编好了，随时准备用，不然笔试的时候哪有那个时间去重新编写一个split函数，不过大佬除外。然而如果是大佬的话肯定也不会去选择用split()，接收处理输入。其实c++里面处理的话基于cin标准输入流,和使用scanf可以接收这种一行里面用空格隔开的输入。

1. 输入数字型的多个数字或者字符

这种情况主要是读入控制台输入的一个数组的输入。一般第一个数字表示输入的数组的长度。
其实很简单。

使用scanf

```
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
//以前一直有迷惑，没用这个是觉得因为如果数组的每个是回车隔开的话，才可以，其实不然scanf在遇到空格，tab,或者回车之后就会把之前的读进来而不管控制符号后面的。只是如何控制台是以回车进去的话，每次都会刷到缓冲区里面

使用cin

```
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
输入字符数组时和scanf用法一样。速度scanf更快(程序竞赛的书上说的)。

但是如果是读入空格分割的字符串，每个空格将需要的字符分开的话，建议用cin.因为可以直接生产string的数组。
```
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

本人不才cin.getline()从来没有用对过。。。
```
string s;
getline(cin,s);
```
所以实际用了getline().但是这个在控制台的时候也得慎用。有可能，你之前按了回车之后　他就当你已经刷新一次缓冲区了，就可能读到空。
因为它阻塞只要遇到一次回车，就完了。


3. c++ string 里面的字符串操作：

https://blog.csdn.net/u010412858/article/details/80474734
