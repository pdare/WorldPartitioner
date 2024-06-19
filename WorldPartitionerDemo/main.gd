extends Node3D


# Called when the node enters the scene tree for the first time.
func _ready():
	var lower = 1.4
	var upper = 1.7
	var lower_int = ceili(lower)
	var upper_int = ceili(upper)
	print('test round')
	print(str(lower_int))
	print(str(upper_int))

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
