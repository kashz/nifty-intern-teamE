exports.getLevel = function (hr) {
	if (hr <= 79)
		return 1;
	else if (hr <= 90)
		return 2;
	else if (hr <= 100)
		return 3;
	else if(hr <= 110)
		return 4;
	else if(hr <= 120)
		return 5;
	else if(hr <= 125)
		return 6;
	else if(hr <= 130)
		return 7;
	else if(hr <= 135)
		return 8;
	else if(hr <= 140)
		return 9;
	else if(hr > 140)
		return 10;
}
