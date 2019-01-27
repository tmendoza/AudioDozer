var myDoc = app.activeDocument;  
var myLayer = myDoc.layers.add();  //in this case, You can keep your layer in ths variable.  
  
var sel = app.selection; //sel as selections array.  
var str = "";

myDoc.rulerOrigin = [0,380]

for (var i=0;i<sel.length;i++) { //loop and check each selected objects  
    str += sel[i].name + "/" + sel[i].position[0].toFixed(2) + "/" + Math.abs(sel[i].position[1].toFixed(3)) + "\n";  
}

var tx = myLayer.textFrames.add();  

tx.contents = str;  
app.executeMenuCommand("deselectall"); //deselect all abjects  
tx.selected = true;  
app.cut();  
  
for (i=0;i<sel.length;i++) sel[i].selected = true;  //select all of last selections   
  
myLayer.remove();
