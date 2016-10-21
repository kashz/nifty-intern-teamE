var mysql = require('mysql');
var fs = require('fs');
var moment = require("moment");
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

var sum = function(arr, fn) {
    if (fn) {
        return sum(arr.map(fn));
    }
    else {
        return arr.reduce(function(prev, current, i, arr) {
                return prev+current;
        });
    }
};

var average = function(arr, fn) {
    return sum(arr, fn)/arr.length;
};

var all_csv = function () {
	var all_avg_data_csv = "value,time\n";
	connection.query('select * from data order by data.timestamp asc', function(error, results, fields) {
		var i = 0;
		var prev_timestamp;
		var sum_array = [];

		results.forEach(function(result) {
			if (i == 0) {
				prev_timestamp = result.timestamp;
				sum_array.push(result.value % 1000);
			}else {
				if (moment(result.timestamp).isBetween(prev_timestamp, moment(prev_timestamp).add(15, 'minutes').format())) {
					sum_array.push(result.value % 1000);
					if (i == results.length-1) {
						all_avg_data_csv = all_avg_data_csv + Math.floor(average(sum_array)) + ',' + moment(prev_timestamp).format("YYYY-MM-DD HH:mm:ss");
					}
				} else {
					all_avg_data_csv = all_avg_data_csv + Math.floor(average(sum_array)) + ',' + moment(prev_timestamp).format("YYYY-MM-DD HH:mm:ss") + '\n';
					prev_timestamp = result.timestamp;
					sum_array = [];
					sum_array.push(result.value % 1000);
					if (i == results.length-1) {
						all_avg_data_csv = all_avg_data_csv + Math.floor(average(sum_array)) + ',' + moment(prev_timestamp).format("YYYY-MM-DD HH:mm:ss");
					}
				}
			}
			i = i + 1;
		});
		fs.writeFile("./views/data/all_avg_data.csv", all_avg_data_csv);
	});
}

var area_csv = function () {
	var area = 1;
	for (var i = 1; i <= 2; ++i) {
		connection.query('select * from data where FLOOR(data.value/1000) = ? order by data.timestamp asc', [i], function(error, results, fields) {
			var prev_timestamp;
			var sum_array = [];
			var area_csv = "value,time\n";
			var j = 0;
			results.forEach(function(result) {
				if (j == 0) {
					prev_timestamp = result.timestamp;
					sum_array.push(result.value % 1000);
				} else {
					if (moment(result.timestamp).isBetween(prev_timestamp, moment(prev_timestamp).add(15, 'minutes').format())) {
						sum_array.push(result.value % 1000);
						if (j == results.length-1) {
							area_csv = area_csv + Math.floor(average(sum_array)) + ',' + moment(prev_timestamp).format("YYYY-MM-DD HH:mm:ss");
						}
					} else {
						area_csv = area_csv + Math.floor(average(sum_array)) + ',' + moment(prev_timestamp).format("YYYY-MM-DD HH:mm:ss") + '\n';
						prev_timestamp = result.timestamp;
						sum_array = [];
						sum_array.push(result.value % 1000);
						if (j == results.length-1) {
							area_csv = area_csv + Math.floor(average(sum_array)) + ',' + moment(prev_timestamp).format("YYYY-MM-DD HH:mm:ss");
						}
					}
				}
				j = j + 1;
			});
			var path = "./views/data/area"+ area +"_data.csv";
			fs.writeFile(path, area_csv);
			area = area + 1;
		});
	}
}

exports.csv = function () {
	all_csv();
	area_csv();
}

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
		var all_level = ssid1_level = ssid2_level = 1;
		// 0で割ることを防ぐためのif文
		if( all_total_hr !== 0 || results.length !== 0 ) {
			all_level = heartrate.getLevel(all_total_hr / results.length);
		}
		if( ssid1_num !== 0 || ssid1_total_hr !== 0 ) {
			ssid1_level = heartrate.getLevel(ssid1_total_hr / ssid1_num);
		}
		if( ssid2_num !== 0 || ssid2_total_hr !== 0 ) {
			ssid2_level = heartrate.getLevel(ssid2_total_hr / ssid2_num);
		}
		exports.level = [all_level, ssid1_level, ssid2_level];
  });
};
