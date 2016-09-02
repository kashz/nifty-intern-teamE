var mysql = require('mysql');
var db_cfg = require('./config/db_settings.js');
var connection = mysql.createConnection({
	host    : db_cfg.host,
	port    : db_cfg.port,
	user    : db_cfg.user,
	password: db_cfg.passwd,
	database: db_cfg.name
});
var retData = {};

connection.connect(function (err) {
	if (err) {
		console.error('error connecting: ' + err.stack);
		return;
	}
});

exports.update = function (query) {
  connection.query('SELECT value from data where event_type_id=1', function(error, results, fields) {
    retData = results;
    // exports.heartRateLevel = [int, int, int]
  });
};

setInterval (function () {

}, 15000);
