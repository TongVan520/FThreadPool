# FThreadTask
线程任务

---
---

## 父类
`RefCounted`

---
---

## 信号
- [started](#started)
- [completed](#completed)

---

### started
任务开始信号
任务开始执行时发出

---

### completed
任务完成信号
任务执行完成时发出

---
---

## 函数
- [makeFromSlot](#makefromslot)
- [execute](#execute)

---

### makeFromSlot
```gdscript
FThreadTask makeFromSlot(init_callbackSlot0: FSlot0) static
```
从槽函数构造

`init_callbackSlot0`
初始化回调函数

**返回值**
该类的新实例

---

### execute
```gdscript
void execute()
```
执行任务

---
---
