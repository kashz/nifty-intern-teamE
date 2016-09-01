'use strict'

var path = require ('path')
var gulp = require ('gulp')
var $ = require ('gulp-load-plugins')({ pattern: ['gulp-*', '*'] })
require('es6-promise').polyfill();

gulp.task('stylus', function ()  {
	gulp.src('front-dev/styles/**/*.styl')
		.pipe($.sourcemaps.init())
		.pipe($.plumber())
		.pipe($.filter([ '**/*', '!**/_*.styl' ]))
		.pipe($.stylus({'include css': true}))
		.pipe($.autoprefixer('last 3 version', 'ie >= 9', 'Android 4.0'))
		.pipe($.minifyCss())
		.pipe($.sourcemaps.write('.'))
		.pipe(gulp.dest('public/styles'))
		.pipe($.logger({ beforeEach: '[stylus] wrote: ' }))
})

/*
gulp.task( 'riot', () => {
	gulp.src('front-dev/tags/*.tag')
}
*/
gulp.task ('default', ['stylus'])

gulp.task('watch', function () {
	$.watch(['front-dev/**/**'], function (files) { gulp.start('default') })
})
