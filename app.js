var express = require('express');
var app = express();

var http = require('http').Server(app);
var socketio = require('socket.io')(http);

var fs = require('fs');

var mysql = require('mysql');
var db_cfg = require('./config/db_settings.js');
var connection = mysql.createConnection({
	host    : db_cfg.host,
	port    : db_cfg.port,
	user    : db_cfg.user,
	password: db_cfg.passwd,
	database: db_cfg.name
});

var d = 0;
var getLevel = function (hb) {
	if (hb <= 79)
		return 1;
	else if (hb <= 90)
		return 2;
	else if (hb <= 100)
		return 3;
	else if(hb <= 110)
		return 4;
	else if(hb <= 120)
		return 5;
	else if(hb <= 125)
		return 6;
	else if(hb <= 130)
		return 7;
	else if(hb <= 135)
		return 8;
	else if(hb <= 140)
		return 9;
	else if(hb > 140)
		return 10;
}

setInterval(function(){
	connection.query('SELECT value from data where event_type_id=1', function(error, results, fields) {
		d = results[results.length-1].value;
		socketio.sockets.emit('level',getLevel(d));
	});
}, 5000);

connection.connect(function (err) {
	if (err) {
		console.error('error connecting: ' + err.stack);
		return;
	}
});

app.set('views', './views');

app.use(express.static('public'));

app.get('/', function(req, res) {
	connection.query('SELECT value from data where event_type_id=1', function(error, results, fields) {
		d = results[results.length-1].value;
		console.log(d);
		fs.readFile('./views/index.html','UTF-8',function(err,data){
			res.setHeader('Content-Type','text/html');
			res.write(data);
			res.end();
		});
	});
});

http.listen(8080, function () {
	console.log('server running...');
});
