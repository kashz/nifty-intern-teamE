var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var fs = require('fs');
var db = require ('./update_db');
var heartrate = require ('./heartrate');


app.set('views', './views');
app.use(express.static('public'));

app.get('/', function(req, res) {
	res.sendFile (__dirname + '/views/index.html');
});

io.on('connection', function(socket){
	io.sockets.emit('name', ["@nifty IoT インターン", "エリア1", "エリア2"]);
	db.update();
	io.sockets.emit('level', db.level);
	io.sockets.emit('deviceCount', db.deviceCount);
  console.log('a user connected');
});

setInterval(function () {
	db.update();
	io.sockets.emit('level', db.level);
	io.sockets.emit('deviceCount', db.deviceCount);
}, 5000);

http.listen(8080, function () {
	console.log('server running...');
});
