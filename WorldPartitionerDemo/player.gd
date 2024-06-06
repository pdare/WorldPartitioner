extends Node3D


@onready var camera = $CameraRoot/CameraH/CameraV/Camera3D
@onready var camera_h = $CameraRoot/CameraH
@onready var camera_v = $CameraRoot/CameraH/CameraV

var sens_hor = 0.5
var sens_vert = 0.2
# Called when the node enters the scene tree for the first time.
func _ready():
	pass

func _input(event):
	if event is InputEventMouseMotion:
		camera_h.rotate_y(deg_to_rad(event.relative.x * sens_hor))
		camera_v.rotate_x(deg_to_rad(-event.relative.y * sens_vert))
	#if event is InputEventJoypadMotion:
		#camera_h.rotate_y(deg_to_rad(event. * sens_hor))
		#camera_v.rotate_x(deg_to_rad(-event.relative.y * sens_vert))
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_pressed("move_backward"):
		self.position.z += 20 * delta
	if Input.is_action_pressed("move_forward"):
		self.position.z -= 20 * delta
	if Input.is_action_pressed("move_left"):
		self.position.x -= 20 * delta
	if Input.is_action_pressed("move_right"):
		self.position.x += 20 * delta
	if Input.is_action_pressed("move_down"):
		self.position.y -= 20 * delta
	if Input.is_action_pressed("move_up"):
		self.position.y += 20 * delta
