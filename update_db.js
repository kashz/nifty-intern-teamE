var mysql = require('mysql');
var db_cfg = require('./config/db_settings.js');
var connection = mysql.createConnection({
	host    : db_cfg.host,
	port    : db_cfg.port,
	user    : db_cfg.user,
	password: db_cfg.passwd,
	database: db_cfg.name
});

connection.connect(function (err) {
	if (err) {
		console.error('error connecting: ' + err.stack);
		return;
	}
});


exports.level = {};
exports.deviceCount = {};
var heartrate = require('./heartrate');

exports.update = function (query) {
  connection.query('select * from data where (event_type_id, timestamp) in (select event_type_id, max(timestamp) from data group by event_type_id)', function(error, results, fields) {
		var ssid1_num = 0, ssid2_num = 0;
		var all_total_hr = 0, ssid1_total_hr = 0, ssid2_total_hr = 0;
		results.forEach(function(result) {
			var hr = result.value % 1000;
			all_total_hr = all_total_hr + hr;
			if( Math.floor(result.value / 1000) == 1 ) {
				ssid1_num = ssid1_num + 1;
				ssid1_total_hr = ssid1_total_hr + hr;
			} else {
				ssid2_num = ssid2_num + 1;
				ssid2_total_hr = ssid2_total_hr + hr;
			}
		});
		exports.deviceCount = [results.length, ssid1_num, ssid2_num];
		exports.level = [heartrate.getLevel(all_total_hr / results.length),
											heartrate.getLevel(ssid1_total_hr / ssid1_num),
											heartrate.getLevel(ssid2_total_hr / ssid2_num)
										];
  });
};
