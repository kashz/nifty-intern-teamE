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
app.get('/history', function(req, res) {
	res.sendFile (__dirname + '/views/history.html');
});
app.get('/all_csv', function(req, res) {
	res.sendFile (__dirname + '/views/data/all_avg_data.csv');
});

io.on('connection', function(socket){
	io.sockets.emit('name', ["@nifty IoT インターン", "エリア1", "エリア2"]);
	db.update();
	db.csv();
	io.sockets.emit('level', db.level);
	io.sockets.emit('deviceCount', db.deviceCount);
	io.sockets.emit('reloadGraph');
  console.log('a user connected');
});

setInterval(function () {
	db.update();
	db.csv();
	io.sockets.emit('level', db.level);
	io.sockets.emit('deviceCount', db.deviceCount);
	io.sockets.emit('reloadGraph');
}, 5000);

http.listen(8000, function () {
	console.log('server running...');
});
