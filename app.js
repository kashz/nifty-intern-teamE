var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var fs = require('fs');
var db = require ('./update_db');
var heartrate = require ('./heartrate');


var hr = 120;
// setInterval(function(){
// 	connection.query('SELECT value from data where event_type_id=1', function(error, results, fields) {
// 		d = results[results.length-1].value;
// 		socketio.sockets.emit('level',getLevel(d));
// 	});
// }, 5000);

app.set('views', './views');
app.use(express.static('public'));

app.get('/', function(req, res) {
	res.sendFile (__dirname + '/views/index.html');
});

io.on('connection', function(socket){
	io.sockets.emit('name', ["@nifty IoT インターン", "18階", "15階"]);

	io.sockets.emit('level', [6, 5, 9]);
	io.sockets.emit('deviceCount', [0, 0, 0]);
	db.update();
  console.log('a user connected');
});

setInterval(function () {
	db.update();
	io.sockets.emit('level' db.level);
	io.sockets.emit('deviceCount', db.deviceCount);
}, 15000);

// Test Code
// setInterval(function () {
// 	var testLevel = [(Math.floor( Math.random() * 9 ) + 1), (Math.floor(Math.random() * 9) + 1), (Math.floor(Math.random() * 9) + 1)];
// 	io.sockets.emit('level', testLevel);
// 	io.sockets.emit('deviceCount', testLevel);
// }, 5000);

http.listen(8080, function () {
	console.log('server running...');
});
