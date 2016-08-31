var express = require('express');
var app = express();

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

var d;

connection.connect(function (err) {
	if (err) {
		console.error('error connecting: ' + err.stack);
		return;
	}
});

app.set('views', './views');
app.set('view engine', 'pug');

app.get('/', function(req, res) {
	connection.query('SELECT value from data', function(error, results, fields) {
		d = results[results.length-1].value;
	});
	res.render('index', {eventTitle : 'イベントの名前', level: 1});
});

app.listen(8080);
console.log('server running...');

