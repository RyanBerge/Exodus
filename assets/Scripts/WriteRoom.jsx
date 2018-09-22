#target photoshop
app.preferences.rulerUnits = Units.PIXELS;


var file = new File("C:/Users/Ryan Berge/Desktop/Exodus/assets/Scripts/room.txt");
file.open("w");
for (var i = 0; i < app.activeDocument.layers.length; i++)
{
	var index = app.activeDocument.layers.length - 1 - i;
	var layer_name = app.activeDocument.layers[index].name;
	layer_name = layer_name.replace(/ copy [0-9]*/g, '');
	//app.activeDocument.layers[i].name = layer_name;
	var x = app.activeDocument.layers[index].bounds[0].value;
	var y = app.activeDocument.layers[index].bounds[1].value;
	file.writeln(layer_name + " " + x + " " + y);
}

file.close();
alert("Done.");