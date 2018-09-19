#target photoshop
app.preferences.rulerUnits = Units.PIXELS;

HIDE_LAYERS = true;
SHOW_LAYERS = false;

var file = new File("C:/Users/Ryan Berge/Desktop/Exodus/assets/Scripts/animation.txt");
file.open("w");

var bounds_output = "Frames:";
var lights_output = "Lights:";
var hitbox_output = "Hitbox:";

for (var index = 0; index < app.activeDocument.layers.length; index++)
{
	if (app.activeDocument.layers[index].name == "Animation")
	{
		var layers = app.activeDocument.layers[index].layers;

		// For each frame
		for (var i = 0; i < layers.length; ++i)
		{
			var frame = layers[i];

			var light_list = [];
			var frame_bounds;
			var origin;
			var hitbox = null;

			// For each layer in the frame
			for (var j = 0; j < frame.layers.length; ++j)
			{
				if (frame.layers[j].name == "Lights")
				{
					var lights = frame.layers[j];
					for (var k = 0; k < lights.layers.length; ++k)
					{
						var light_x = lights.layers[k].bounds[0].value;
						var light_y = lights.layers[k].bounds[1].value;

						light_list.push({x: light_x, y: light_y, name: lights.layers[k].name})

						if (HIDE_LAYERS)
							lights.layers[k].visible = false;
						else if (SHOW_LAYERS)
							lights.layers[k].visible = true;
					}
				}
				else if (frame.layers[j].name == "Origin")
				{
					var x = frame.layers[j].bounds[0].value;
					var y = frame.layers[j].bounds[1].value;
					var width = frame.layers[j].bounds[2].value - x;
					var height = frame.layers[j].bounds[3].value - y;

					origin = {x: x, y: y, width: width, height: height};

					if (HIDE_LAYERS)
						frame.layers[j].visible = false;
					else if (SHOW_LAYERS)
						frame.layers[j].visible = true;
				}
				else if (frame.layers[j].name == "Hitbox")
				{
					var x = frame.layers[j].bounds[0].value;
					var y = frame.layers[j].bounds[1].value;
					var width = frame.layers[j].bounds[2].value - x;
					var height = frame.layers[j].bounds[3].value - y;

					hitbox = {x: x, y: y, width: width, height: height};

					if (HIDE_LAYERS)
						frame.layers[j].visible = false;
					else if (SHOW_LAYERS)
						frame.layers[j].visible = true;
				}
				else
				{
					var x = frame.layers[j].bounds[0].value;
					var y = frame.layers[j].bounds[1].value;
					var width = frame.layers[j].bounds[2].value - x;
					var height = frame.layers[j].bounds[3].value - y;

					frame_bounds = {x: x, y: y, width: width, height: height};

					if (SHOW_LAYERS)
						frame.layers[j].visible = true;
				}
			}

			lights_output += " [";
			for (var l = 0; l < light_list.length; ++l)
			{
				var light_x = light_list[l].x - frame_bounds.x;
				var light_y = light_list[l].y - frame_bounds.y;

				if (l != 0)
					lights_output += ", "

				lights_output += light_x + " " + light_y + " " + light_list[l].name;
			}
			lights_output += "]";

			if (hitbox == null)
			{
				hitbox_output += " []";
			}
			else
			{
				var x = hitbox.x - frame_bounds.x;
				var y = hitbox.y - frame_bounds.y;
				var width = hitbox.width;
				var height = hitbox.height;
				hitbox_output += " [" + x + " " + y + " " + width + " " + height + "]";
			}

			bounds_output += " [" + frame_bounds.x + " " + frame_bounds.y + " " + frame_bounds.width + " " + frame_bounds.height +
			" " + (origin.x - frame_bounds.x) + " " + (origin.y - frame_bounds.y) + "]";
		}

		break;
	}
}

file.writeln(bounds_output);
file.writeln("");
file.writeln(lights_output);
file.writeln("")
file.writeln(hitbox_output);
file.writeln("")

file.close();
alert("Done.");