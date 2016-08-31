var socket = io();
socket.on('level', function(level) {
  console.log(level);
})
