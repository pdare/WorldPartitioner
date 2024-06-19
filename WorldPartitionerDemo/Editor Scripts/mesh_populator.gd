@tool
extends EditorScript


var props_num = 10000
var max_neg = Vector2(-500.0, -500.0)
var max_pos = Vector2(500.0, 500.0)

var tree_mesh = load("res://Meshes/tree-autumn-tall_tree-autumn-tall.res")
# Called when the script is executed (using File -> Run in Script Editor).
func _run():
	var parent = get_scene().get_node(^"WorldPartition")
	var rand = RandomNumberGenerator.new()
	for i in range(0, props_num):
		var x_loc : float
		var z_loc : float
		x_loc = rand.randf_range(max_neg.x, max_pos.x)
		z_loc = rand.randf_range(max_neg.y, max_pos.y)
		
		var prop = MeshInstance3D.new()
		parent.add_child(prop)
		prop.owner = get_scene()
		prop.mesh = tree_mesh
		prop.global_position.x = x_loc
		prop.global_position.z = z_loc
		prop.scale = Vector3(5.0, 5.0, 5.0)
