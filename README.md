# 操作系统课设



> 详情请看课设报告



## （一）课设板块

 模块划分：
（1）、进程管理模块
（2）、内存管理模块
（3）、文件管理模块
（4）、设备管理模块
（5）、界面管理模块



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

mainwindow.h		

mainwindow.cpp

mainwindow.ui



OS.pro 项目配置文件

pics文件夹  存放图标




1. 进程管理

* process文件夹

2. 内存管理

* memory文件夹

3. 文件管理

* file文件夹

4. 设备管理

* equipment文件夹

5. 磁盘管理

* disk文件夹



<br/><br/>

## （三）模块间数据共享

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



## （四）流程图

​                                **进程管理**![在这里插入图片描述](https://img-blog.csdnimg.cn/eb5dd0aa3de84f62803d1dd1c9cbddb2.png)

​                               **内存管理**

![在这里插入图片描述](https://img-blog.csdnimg.cn/ff56e43b1b8f48e9b4a2ab83319aa834.png)



​                               **文件管理**

![在这里插入图片描述](https://img-blog.csdnimg.cn/c8475b07bfca40f4b9d6e1b3a292b3dd.png)



​                               **设备管理**



![在这里插入图片描述](https://img-blog.csdnimg.cn/080a26ed0923494286ef682a5d18dc06.png)



## （五）UML

​      **进程管理**

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102140484.png) 

 

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102140478.png) 

 

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102140465.png) 

 

​                    

 

​                           **内存管理**

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102140482.jpg) 

**文件管理**

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102140491.jpg)

 

​                         **设备管理**

![image-20230310214140115](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102141161.png)









 

## （六）效果展示

#### 1.进程模块

创建进程时，所有进程就绪

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145990.jpg) 

开始调度，由下图可见，进程“获取键值”在“键盘输入”未运行完，只执行了一部分时调入运行队列后阻塞。同时“键盘输入”在运行自己的第三个时间片（一个时间片2s，图上显示第6秒）时，请求调入第19号页面。  且运行完的进程所占用的内存块释放。

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145993.jpg) 

 

磁盘调度时显示磁盘调度信息

 

访问磁道号2，磁盘模块第三行正在被访问，变为红色

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145998.jpg)![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145997.jpg) 

 

#### 2.内存模块

内存初始化状态

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145003.jpg) 

 

虚拟内存初始化状态：

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145004.jpg) 

 

运行时：

![img](file:///C:\Users\Clarence\AppData\Local\Temp\ksohtml7232\wps50.jpg) 

 

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145372.jpg) 

 

 

#### 3.文件模块

文件初始创建时

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145448.jpg) 

 

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145478.jpg) 

 

**扩展各个文件后**

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145487.jpg) 

操作删除文件

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145579.jpg) 

 

操作后

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145013.jpg) 

 

 

#### 4. 设备管理模块

**4.1 进程队列的生成**

  点击生成按钮，产生一个进程个数为24的进程队列。

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145011.jpg) 

  点击每个进程可以查看该进程详情

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145010.jpg) 

 

**4.2 进程资源矩阵展示**

  点击开始，展示资源的分配以及进程请求的详细情况

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145026.jpg) 

 

**4.3 发送请求**

点击发送请求按钮，开始发出请求。

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145032.jpg) 

若请求资源大于need向量，则拒绝请求。

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145276.jpg) 

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145383.jpg) 

 

请求合理则执行安全性算法检测

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145413.jpg) 

 

**4.4 安全性算法**

执行安全性算法，

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145450.jpg) 



**4.5 资源分配**

通过则分配资源，进程的need减少，allocati增加，available减少。

例如：

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145458.jpg) 

请求后（request = (1,1,1,1)）

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145609.jpg))

 

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145767.jpg) 

 

 

**4.资源回收**

资源占有时间到后，则回收，该进程allocation和need置零，available=available+allocationi

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145768.jpg) 

 

![img](https://xiehangblog.oss-cn-beijing.aliyuncs.com/pic/202303102145776.jpg) 

 

 
