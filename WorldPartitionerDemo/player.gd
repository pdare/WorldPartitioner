extends Node3D

@export var move_speed : float

@onready var camera = $CameraRoot/CameraH/CameraV/Camera3D
@onready var far_camera = $CameraRoot/CameraH/CameraV/Camera3D2
@onready var camera_h = $CameraRoot/CameraH
@onready var camera_v = $CameraRoot/CameraH/CameraV

var sens_hor = 0.5
var sens_vert = 0.2
var curr_cam = 0
# Called when the node enters the scene tree for the first time.
func _ready():
	curr_cam = 0

func _input(event):
	if event is InputEventMouseMotion:
		camera_h.rotate_y(deg_to_rad(event.relative.x * sens_hor))
		camera_v.rotate_x(deg_to_rad(-event.relative.y * sens_vert))
	#if event is InputEventJoypadMotion:
		#camera_h.rotate_y(deg_to_rad(event. * sens_hor))
		#camera_v.rotate_x(deg_to_rad(-event.relative.y * sens_vert))
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_just_pressed("switch_camera"):
		if curr_cam == 0:
			far_camera.current = true
			curr_cam = 1
		else:
			camera.current = true
			curr_cam = 0
	if Input.is_action_pressed("move_backward"):
		self.position.z += move_speed * delta
	if Input.is_action_pressed("move_forward"):
		self.position.z -= move_speed * delta
	if Input.is_action_pressed("move_left"):
		self.position.x -= move_speed * delta
	if Input.is_action_pressed("move_right"):
		self.position.x += move_speed * delta
	if Input.is_action_pressed("move_down"):
		self.position.y -= move_speed * delta
	if Input.is_action_pressed("move_up"):
		self.position.y += move_speed * delta
	if Input.is_action_pressed("increase_move_speed"):
		move_speed += 0.5
	if Input.is_action_pressed("decrease_move_speed"):
		move_speed -= 0.5
	if Input.is_action_just_pressed("announce_player_loc"):
		print("player loc: {0}".format([self.global_position]))
