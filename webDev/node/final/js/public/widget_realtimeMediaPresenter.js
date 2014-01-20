
var Raphael
define(['raphael-min'], function(newRaphael){
var widget = function(dependencies){
	Raphael=newRaphael
	var files = []
	var paperDOM = $("<div id='raphael' />")
	this.view = $("<div id='raphaelcontainer' />") //document.getElementById('Container')
	var paper = new Raphael(paperDOM.get(0), 975, 400)
	this.view.css({
		'position':'absolute',
		'align':'center',
		'left':'200px',
		'border':'1px solid black',
		
	})
	this.view.append(paperDOM)
	var fileGraphics = paper.set()
	var fileTexts = paper.set()

	var video = $("<video id='webcam' />")
	video.css({
		'background-color':'#000',
		'position':'absolute',
		'zIndex':7,
		'left':'200px',
		'top':'10px',
		'border':'1px solid'
	}).width(160).height(120)
	this.view.append(video)
//	video.style.zIndex = 7
	
	var video2 = $("<video id='screen' />")
	video2.css({
		'background-color':'#000',
		'position':'absolute',
		'zIndex':7,
		'left':'10px',
		'top':'300px',
		'border':'1px solid'
	}).width(160).height(120)
	this.view.append(video2)
	//video2.style.zIndex = 7
	console.log("position:",video,video.position())
	console.log("height:",video.height())	
	var rect = paper.rect(video.position().left, video.position().top, video.width(), video.height() + 20).attr("fill","#CCF").attr("stroke-width","2")
	rect.drag(dragRect,startDrag,endDrag)
	var t = paper.text(parseInt(video.position().left)+5, rect.getBBox().y + rect.getBBox().height-10, "Self - (Webcam)").attr({'text-anchor': 'start','stroke-width':2})
	rect.ox = parseInt(rect.attr("x"))
	rect.oy = parseInt(rect.attr("y"))
	
	var rect2 = paper.rect(parseInt(video2.position().left),parseInt(video2.offset().top), video2.width(), video.height() + 20).attr("fill","#CCF").attr("stroke-width","2")
	rect2.drag(dragRect2,startDrag2,endDrag2)
	var r2bb = rect2.getBBox()
	var t2 = paper.text(parseInt(video2.position().left)+5, r2bb.y + r2bb.height - 10, "Screen Capture").attr({'text-anchor': 'start','stroke-width':2})
	rect2.ox = parseInt(rect2.attr("x"))
	rect2.oy = parseInt(rect2.attr("y"))
	
	var rect3 = paper.rect(200,300, video2.width(), video.height()+20).attr("fill","#CCF").attr("stroke-width","2")
	rect3.drag(dragRect3,startDrag3,endDrag3)
	var t3 = paper.text(parseInt(rect3.getBBox().x)+5, rect3.getBBox().y+rect3.getBBox().height-10, "Shared Files").attr({'text-anchor': 'start','stroke-width':2})
	rect3.ox = parseInt(rect3.attr("x"))
	rect3.oy = parseInt(rect3.attr("y"))
	
	var rect4 = paper.rect(390,300, video2.width(), video.height()+20).attr("fill","#CCF").attr("stroke-width","2")
	rect4.drag(dragRect4,startDrag4,endDrag4)
	var t4 = paper.text(parseInt(rect4.getBBox().x)+5, rect4.getBBox().y+rect4.getBBox().height-10, "File System").attr({'text-anchor': 'start','stroke-width':2})
	rect4.ox = parseInt(rect4.attr("x"))
	rect4.oy = parseInt(rect4.attr("y"))
	
	this.videoStreamLocal;
	var videoStreamLocal2;
	
	navigator.getMedia = ( navigator.getUserMedia ||
						   navigator.webkitGetUserMedia ||
						   navigator.mozGetUserMedia ||
						   navigator.msGetUserMedia)
	window.URL = (	window.URL || 
					window.webkitURL)

	if(navigator.getMedia != null){
		navigator.getMedia (
		 
			// constraints
			{
				video: {
					mandatory: {
						chromeMediaSource: 'screen'
					}
				}
			},
		 
			// successCallback
			function(localMediaStream) {
				console.log('successfully aquired video/audio capture permissions screen')
				videoStreamLocal2 = localMediaStream;
				console.log(localMediaStream.getVideoTracks()[0].width);
				video2.attr('src', (navigator.getMedia) ? window.URL.createObjectURL(localMediaStream) : localMediaStream)
				//video.src = (window.URL && window.URL.createObjectURL(videoStreamLocal)) || videoStreamLocal;
				video2[0].onloadedmetadata = function(e) {
					// Do something with the video here.
					console.log('meta data2 loaded')
				};
				video2[0].play()
			},
		 
			// errorCallback
			function(err) {
				console.log("The following error occured: " + err);
				console.log(err);
			}
		 
		);
	}else{
		console.log('no media')
	}
	
	if(navigator.getMedia != null){
		navigator.getMedia (
		 
			// constraints
			{
				video: true,
				audio: true
			},
		 
			// successCallback
			function(localMediaStream) {
				console.log('successfully aquired video/audio capture permissions webcam')
				this.videoStreamLocal = localMediaStream;
				console.log("LMS:",localMediaStream,video)
				video.attr('src', (navigator.getMedia) ? window.URL.createObjectURL(localMediaStream) : localMediaStream)
				//video.src = (window.URL && window.URL.createObjectURL(videoStreamLocal)) || videoStreamLocal;
				video[0].onloadedmetadata = function(e) {
					// Do something with the video here.
					console.log('meta data loaded')
				};
			//	video[0].load()
				video[0].play()
			},
		 
			// errorCallback
			function(err) {
				console.log("The following error occured: " + err);
				console.log(err);
			}
		 
		);
	}else{
		console.log('no media')
	}
	
	var line = connect(rect,rect2,line)
	var line2 = connect(rect,rect3,line2)
	var line3 = connect(rect,rect4,line3)

	function dragRect(dx,dy){
		this.attr({x: parseInt(this.ox + dx), y: parseInt(this.oy + dy)})
		video.css({'left': video.ox + dx + "px"})
		video.css({'top': video.oy + dy + "px"})
		t.attr({x:parseInt(t.ox+dx), y:parseInt(t.oy+dy)})
		if(line) line.remove()
		if(line2) line2.remove()
		if(line3) line3.remove()
		line=connect(rect,rect2,line);
		line2=connect(rect,rect3,line2);
		line3=connect(rect,rect4,line3);

	}
	function startDrag(){
		this.ox = parseInt(this.attr("x"))
		this.oy = parseInt(this.attr("y"))
		video.ox = parseInt(this.attr("x"))
		video.oy = parseInt(this.attr("y"))
		t.ox = parseInt(t.attr("x"))
		t.oy = parseInt(t.attr("y"))
		
		console.log('dragging')
	}
	function endDrag(){
	console.log('done dragging')
	}
	
	function dragRect2(dx,dy){
		this.attr({x: parseInt(this.ox + dx), y: parseInt(this.oy + dy)})
		video2.css({'left': (video2.ox + dx) + "px"})
		video2.css({'top': (video2.oy + dy) + "px"})
		t2.attr({x:parseInt(t2.ox+dx), y:parseInt(t2.oy+dy)})
		if(line) line.remove()
		line= connect(rect,rect2,line);

	}
	function startDrag2(){
		this.ox = parseInt(this.attr("x"))
		this.oy = parseInt(this.attr("y"))
		video2.ox = parseInt(this.attr("x"))
		video2.oy = parseInt(this.attr("y"))
		t2.ox = parseInt(t2.attr("x"))
		t2.oy = parseInt(t2.attr("y"))

		console.log('dragging')
	}
	function endDrag2(){
	console.log('done dragging')
	}

	function dragRect3(dx,dy){
		this.attr({x: parseInt(this.ox + dx), y: parseInt(this.oy + dy)})
		t3.attr({x:parseInt(t3.ox+dx), y:parseInt(t3.oy+dy)})
		
		for(var i = 0; i < files.length; i++){
			console.log(files[i],i)
			fileGraphics[i].attr({x: parseInt(fileGraphics[i].ox + dx), y: parseInt(fileGraphics[i].oy + dy)})
			fileTexts[i].attr({x: parseInt(fileTexts[i].ox + dx), y: parseInt(fileTexts[i].oy + dy)})
		}
		
		if(line2) line2.remove()
		line2 = connect(rect,rect3,line2)

	}
	function startDrag3(){
		this.ox = parseInt(this.attr("x"))
		this.oy = parseInt(this.attr("y"))
		t3.ox = parseInt(t3.attr("x"))
		t3.oy = parseInt(t3.attr("y"))
		
		for(var i in files){

			fileGraphics[i].ox= parseInt(fileGraphics[i].attr("x"))
			fileGraphics[i].oy= parseInt(fileGraphics[i].attr("y"))
			fileTexts[i].ox= parseInt(fileTexts[i].attr("x"))
			fileTexts[i].oy= parseInt(fileTexts[i].attr("y"))
			
		}
		
		console.log('dragging')
	}
	function endDrag3(){
	console.log('done dragging')
	}

/*	function dragRectFile(dx,dy){
		this.attr({x: parseInt(this.ox + dx), y: parseInt(this.oy + dy)})
		fileTexts[this.oi].attr({x:parseInt(fileTexts[this.oi].ox + dx),y: parseInt(fileTexts[this.oi].oy + dy)})
		console.log('dragging file')

	}
	function startDragFile(){
		this.ox = parseInt(this.attr("x"))
		this.oy = parseInt(this.attr("y"))
		console.log('dragging index ',this.oi)
		fileTexts[this.oi].ox = parseInt(fileTexts[this.oi].attr('x'))
		fileTexts[this.oi].oy = parseInt(fileTexts[this.oi].attr('y'))

		console.log('dragging file')
	}
	function endDragFile(){
	console.log('done dragging')
	}
	
*/	function dragRect4(dx,dy){
		this.attr({x: parseInt(this.ox + dx), y: parseInt(this.oy + dy)})
		t4.attr({x:parseInt(t4.ox+dx), y:parseInt(t4.oy+dy)})
		if(line3) line3.remove()
		line3 = connect(rect,rect4,line3)

	}
	function startDrag4(){
		this.ox = parseInt(this.attr("x"))
		this.oy = parseInt(this.attr("y"))
		t4.ox = parseInt(t4.attr("x"))
		t4.oy = parseInt(t4.attr("y"))
		
		console.log('dragging')
	}
	function endDrag4(){
	console.log('done dragging')
	}
	
	function connect(rect, rect2, line){
		if(rect.getBBox().y > rect2.getBBox().y){
			path = [	"M",
						(rect2.getBBox().x+ rect2.getBBox().width/2), 
						(rect2.getBBox().y+ rect.getBBox().height),
						
						"C",
						(rect2.getBBox().x + rect2.getBBox().width/2), 
						(rect2.getBBox().y + rect2.getBBox().height*2), 
						(rect.getBBox().x + rect.getBBox().width/2), 
						(rect.getBBox().y - rect.getBBox().height), 
						(rect.getBBox().x+ rect.getBBox().width/2),
						(rect.getBBox().y)
					]
		}else{
			path = [	"M",
						(rect.getBBox().x+ rect.getBBox().width/2),
						(rect.getBBox().y+ rect.getBBox().height),
						
						"C",
						(rect.getBBox().x + rect.getBBox().width/2), 
						(rect.getBBox().y + rect.getBBox().height*2), 
						(rect2.getBBox().x + rect2.getBBox().width/2), 
						(rect2.getBBox().y - rect2.getBBox().height), 
						(rect2.getBBox().x+ rect2.getBBox().width/2), 
						(rect2.getBBox().y)
					]
		}
	line = paper.path(path).attr("stroke-width","2").attr("stroke","#CCCCCC")
	return line
	}
	
	window.requestFileSystem  = window.requestFileSystem || window.webkitRequestFileSystem;

//	window.requestFileSystem(window.PERSISTENT, 5*1024*1024, onInitFs, errorHandler)

	function toArray(list) {
	  return Array.prototype.slice.call(list || [], 0);
	}
	
/*	function listResults(entries) {
		var fragment = document.createDocumentFragment();
		console.log('listing')
		for (var i=0; i < entries.length; i++) {
				var img = entries[i].isDirectory ? '<img src="folder-icon.gif">' :
											  '<img src="img/file_ico.png" height="10" width="8">';

				console.log('index ',i)

				files.push({name: entries[i].name, dir: entries[i].isDirectory});
				var rect = paper.rect(rect3.getBBox().x+5,rect3.getBBox().y + 8, 150, 15).attr('fill','#AAC');
				var elem = paper.text(rect3.getBBox().x+10, rect3.getBBox().y + (i*20)+15, entries[i].name).attr({'text-anchor': 'start','stroke-width':2})
				fileGraphics.push(rect)
				fileTexts.push(elem)
				rect.drag(dragRectFile,startDragFile,endDragFile)
				rect.oi = i;
		}
		updatefs()
	}
	
	function onInitFs(fs){
		fs.root.getFile('log.txt', {create: true}, function(fileEntry) {

		// Create a FileWriter object for our FileEntry (log.txt).
		fileEntry.createWriter(function(fileWriter) {
			  fileWriter.onwriteend = function(e) {
				console.log('Write completed.');
			  };

			  fileWriter.onerror = function(e) {
				console.log('Write failed: ' + e.toString());
			  };

			  // Create a new Blob and write it to log.txt.
			  var blob = new Blob(['Lorem Ipsum'], {type: 'text/plain'});

			  fileWriter.write(blob);

			}, errorHandler);  
		}, errorHandler);
	  
		fs.root.getFile('log.txt', {}, function(fileEntry) {
			fileEntry.file(function(file) {
				var reader = new FileReader();

				reader.onloadend = function(e) {
				 var txtArea = document.createElement('textarea');
				 txtArea.value = this.result;
				 document.body.appendChild(txtArea);
				};

				reader.readAsText(file);
			}, errorHandler);

		}, errorHandler);
		
			console.log('5mb local storage initialized')
				var dirReader = fs.root.createReader();
				var entries = [];

				// Call the reader.readEntries() until no more results are returned.
				var readEntries = function() {
					dirReader.readEntries (function(results) {
						if (!results.length) {
							listResults(entries.sort());
						} else {
							entries = entries.concat(toArray(results));
							readEntries();
						}
				}, errorHandler);
			  };

		readEntries(); // Start reading dirs.
	}

	function errorHandler(){
		var msg = '';

		switch (e.code) {
			case FileError.QUOTA_EXCEEDED_ERR:
				msg = 'QUOTA_EXCEEDED_ERR';
			break;
			case FileError.NOT_FOUND_ERR:
				msg = 'NOT_FOUND_ERR';
			break;
			case FileError.SECURITY_ERR:
				msg = 'SECURITY_ERR';
			break;
			case FileError.INVALID_MODIFICATION_ERR:
				msg = 'INVALID_MODIFICATION_ERR';
			break;
			case FileError.INVALID_STATE_ERR:
				msg = 'INVALID_STATE_ERR';
			break;
			default:
				msg = 'Unknown Error';
			break;
		};

		console.log('Error: ' + msg);
	}

	window.webkitStorageInfo.requestQuota(PERSISTENT, 1024*1024, function(grantedBytes) {
	  window.requestFileSystem(PERSISTENT, grantedBytes, onInitFs, errorHandler);
	  console.log('fs created')
	}, function(e) {
	  console.log('Error', e);
	});

	function updatefs(){
		
	}*/
}
return widget
})
