extends Node

@onready var stateLabel := %StateLabel as Label
@onready var startButton := %StartButton as Button
@onready var stopButton := %StopButton as Button
@onready var pauseButton := %PauseButton as Button
@onready var resumeButton := %ResumeButton as Button

@onready var task_countLabel := %TaskCountLabel as Label
@onready var add_taskButton := %AddTaskButton as Button
@onready var remove_taskButton := %RemoveTaskButton as Button
@onready var clear_taskButton := %ClearTaskButton as Button

@onready var testButton := %TestButton as Button
@onready var testLabel := %TestLabel as Label

var thread_pool := FThreadPool.new()
var taskArr := []

var numeberArr: PackedInt64Array = []

func _ready() -> void:
	self.connectSignal()
	pass

func connectSignal() -> void:
	self.startButton.pressed.connect(
		func():
			thread_pool.start()
			print("启动线程池")
			pass
	)
	
	self.stopButton.pressed.connect(
		func():
			thread_pool.stop()
			print("停止线程池")
			pass
	)
	
	self.pauseButton.pressed.connect(
		func():
			thread_pool.pause()
			print("暂停线程池")
			pass
	)
	
	self.resumeButton.pressed.connect(
		func():
			thread_pool.resume()
			print("恢复线程池")
			pass
	)
	
	self.add_taskButton.pressed.connect(
		func():
			self.addTask()
			print("往线程池中添加一个任务")
			pass
	)
	
	self.remove_taskButton.pressed.connect(
		func():
			self.removeTask()
			print("从线程池中移除一个任务")
			pass
	)
	
	self.clear_taskButton.pressed.connect(
		func():
			self.thread_pool.clearTask()
			self.removeInvalidTask()
			print("清空任务")
			pass
	)
	
	self.testButton.pressed.connect(
		func():
			# print("测试开始")
			self.test()
			print("测试")
			pass
	)
	pass

func _process(_delta: float) -> void:
	self.updateTaskCount()
	self.updateTestLabel()
	self.updateStateLabel()
	pass

func addTask() -> void:
	var task := FThreadTask.makeFromSlot(
		FSlot0.makeFromCallable(
			(func():
				print("hello thread pool")
				pass )
		)
	)
	self.taskArr.append(task)
	self.thread_pool.addTask(task).connect(
		FSlot0.makeFromCallable(
			(func():
				print("任务完成！")
				pass )
		)
	)
	pass

func removeTask() -> void:
	self.removeInvalidTask()
	
	if self.taskArr.is_empty():
		print("线程池中没有一个任务...")
		return
	
	var task := self.taskArr.pop_front() as FThreadTask
	if not self.thread_pool.hasTask(task):
		print("线程池中没有该任务...")
		return
	
	self.thread_pool.removeTask(task)
	pass

func removeInvalidTask() -> void:
	self.taskArr = self.taskArr.filter(
		func(task: FThreadTask) -> bool:
			return self.thread_pool.hasTask(task)
	)
	pass

func updateTaskCount() -> void:
	var textStr := "当前任务数量："
	
	textStr += str(self.thread_pool.getTaskCount())
	
	self.task_countLabel.text = textStr
	pass

func updateTestLabel() -> void:
	self.testLabel.text = str(self.numeberArr)
	pass

func updateStateLabel() -> void:
	var textStr := "线程池状态："
	
	match self.thread_pool.getState():
		FThreadPool.FThreadPoolStateEnum.Stopped:
			textStr += "已停止"
			pass
		FThreadPool.FThreadPoolStateEnum.Started:
			textStr += "已启动"
			pass
		FThreadPool.FThreadPoolStateEnum.Paused:
			textStr += "已暂停"
			pass
		FThreadPool.FThreadPoolStateEnum.Waiting:
			textStr += "等待线程退出"
			pass
		pass
	
	self.stateLabel.text = textStr
	pass

func initNumberArray() -> void:
	self.numeberArr.clear()
	self.numeberArr.resize(30)
	self.numeberArr.fill(0)
	pass

func test() -> void:
	self.initNumberArray()
	for i in self.numeberArr.size():
		var task := FThreadTask.makeFromSlot(
			FSlot0.makeFromCallable(
				(func():
					var rng := RandomNumberGenerator.new()
					for j in 100:
						self.numeberArr[i] += 1
						OS.delay_msec(rng.randi_range(10, 750))
						pass
					pass )
			)
		)
		self.thread_pool.addTask(task)
		task.completed.connect(
			FSlot0.makeFromCallable(
				(func():
					print("下标", i, "任务已完成")
					pass )
			)
		)
		pass
	pass
