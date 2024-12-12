# FThreadPool
线程池
可以自动将任务分配到各线程
没有任务时自动挂起线程

## 父类
`RefCounted`


## 属性
|类型|属性|默认值|
|:-|:-|:-|
|int|[max_thread_count](#max_thread_count)|?|

### max_thread_count
最大线程数量
自动分配的线程数量不会超过这个值
注意：修改该值只会从**下一次**分配线程时开始生效
**默认值为`用户操作系统的CPU核心数 - 1`**

## 信号
- [maxThreadCountChanged](#maxThreadCountChanged)

### maxThreadCountChanged
[最大线程数量](#max_thread_count)改变信号
在最大线程数量改变时发出


## 枚举
### FThreadPoolStateEnum
线程池状态枚举

|枚举|值|
|:-|:-:|
|[Stopped](#Stopped)|0|
|[Started](#Started)|1|
|[Paused](#Paused)|2|
|[Waiting](#Waiting)|3|

#### Stopped
已停止

#### Started
已启动

#### Paused
已暂停

#### Waiting
等待
正在等待线程退出


## 函数
- [addTask](#addtask)
- [removeTask](#removetask)
- [hasTask](#hastask)
- [start](#start)
- [stop](#stop)
- [pause](#pause)
- [resume](#resume)
- [getTaskCount](#gettaskcount)
- [getThreadCount](#getthreadcount)
- [getState](#getstate)

### addTask
```gdscript
FSignal0 addTask(new_task: FThreadTask)
```
添加任务
**线程安全**，**原子操作**

`new_task`
新任务

**返回值**
任务完成时发出的信号


### removeTask
```gdscript
void removeTask(task: FThreadTask)
```
移除任务
**线程安全**，**原子操作**

`task`
任务


### hasTask
```gdscript
bool hasTask(task: FThreadTask) const
```
查找任务
**线程安全**，**原子操作**
仅能查找尚未被执行的任务

`task`
任务

**返回值**
是否有任务


### clearTask
```gdscript
void clearTask()
```
清空任务
清空所有尚未被执行的任务
**线程安全**，**原子操作**


### start
```gdscript
void start()
```
启动线程池
**线程安全**，**原子操作**


### stop
```gdscript
void stop()
```
停止线程池
**线程安全**，**原子操作**
阻塞调用线程并等待其它线程退出


### pause
```gdscript
void pause()
```
暂停线程池
**线程安全**，**原子操作**


### resume
```gdscript
void resume()
```
恢复线程池
**线程安全**，**原子操作**


### getTaskCount
```gdscript
int getTaskCount() const
```
获取任务数量
**线程安全**，**原子操作**

**返回值**
当前剩余未被执行的任务数量


### getThreadCount
```gdscript
int getThreadCount() const
```
获取线程数量
**线程安全**，**原子操作**

**返回值**
当前线程数量


### getState
```gdscript
FThreadPoolStateEnum getState() const
```
获取线程池状态
**线程安全**，**原子操作**

**返回值**
线程池状态

