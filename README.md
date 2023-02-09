# 操作系统课设

## （一）课设板块

### 1.    **进程管理：** 考虑进程的创建、状态转换（就绪、执行、阻塞）、调度、撤销等，设计PCB结构，设计进程调度算法。

**选做：使用信号量机制模拟多任务系统中的进程同步。**

 <br/>

### 2.    **内存管理** ：考虑内存的动态分配与回收，采取何种数据结构，采用何内存分配算法等。

**选做：内存的离散分配（分页式、分段式或段页式管理），以及基于此种机制下的虚拟内存管理，页面置换算法。**

 <br/>

### 3.    **文件管理：** 考虑文件及文件目录的创建、查询、删除等操作的实现，考虑磁盘空间分配采用的数据结构及算法（连续分配、显式链接、索引分配），考虑磁盘空闲区管理采用的数据结构及算法（空闲盘块表、空闲盘块链、位示图、成组链接法）和算法。

**选做：磁盘调度算法。**

 <br/>

### 4. 设备管理：考虑设备的分配和回收，采取何种数据结构，采用何种分配算法，使用银行家算法避免死锁。





<br/><br/>


## （二）项目文件

主窗口

mainwindow.h		//尽量不要把代码写在主窗口，分别写在各自的模板方便管理

mainwindow.cpp

mainwindow.ui

<br/>


1. 进程管理

* processtab.h          //进程管理页面
* processtab.cpp
* processtab.ui



* pcb.h				 //进程块的定义
* pcb.cpp



* processdialog.h       //创建进程的对话框
* processdialog.cpp
* processdialog.ui

<br/>

2. 内存管理

* memory.h
* memory.cpp
* memory.ui

<br/>

3. 文件管理

* file.h
* file.cpp
* file.ui

<br/>

4. 设备管理

* equipment.h
* equipment.cpp
* equipment.ui

<br/>

5. 磁盘管理

* disk.h
* disk.cpp
* disk.ui



此处这么设计，可能不错

![image-20230205210350357](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202302052103401.png)



<br/><br/>

## （三）开发流程

### 分模块

MainWindow主窗口定义了五个板块的接口：

```cpp
 ProcessTab *processTab;
 Memory *memoryTab;
 File *fileTab;
 Equipment *equipmentTab;
 Disk *diskTab;

```

<br/>

每个模块都有属于自己的类，分模块方便开发，减少混淆带来的麻烦

<br/>

### 关于数据共享

不同的类之间共享数据，通常可以考虑；

* 某变量设置为全局变量，然后extern声明

  如下，通过全局变量w, 共享readyQueue队列

```cpp
//根据优先级排序，放入就绪队列
extern MainWindow *w;
    w->processTab->readyQueue.push_back(process);
    sort(w->processTab->readyQueue.begin(),w->processTab->readyQueue.end(), ProcessTab::compare);

```

* 包含某个类的文件，然后使用其函数

* 利用信号与槽

  如下，在对话框更改了另一个process的界面

  ```cpp
  //更新表格
  connect(this,SIGNAL(sendToShowProcess()),w->processTab,SLOT(showProcess()));
  emit this->sendToShowProcess();
  ```

<br/><br/>

## （四）效果展示

![image-20230205212305169](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202302052123236.png)
