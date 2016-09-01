<app>
  <div class="l-container">
    <div class="all_wrapper">
      <h2>{ eventName }</h2>
      <div class="container">
        <div class="heart_wrapper heart1">
          <div class="all_heart"></div>
        </div>
        <div class="device_count">{ deviceNumAll }</div>
      </div>
    </div>
    <div class="area_wrapper">
      <div class="area">
        <h3>{ areaName1 }</h3>
        <div class="container">
          <div class="heart_wrapper heart1">
            <div class="heart"></div>
          </div>
          <div class="device_count">{ deviceNumArea1 }</div>
        </div>
      </div>
      <div class="area">
        <h3>{ areaName2 }</h3>
        <div class="container">
          <div class="heart_wrapper heart1">
            <div class="heart"></div>
          </div>
          <div class="device_count">{ deviceNumArea2 }</div>
        </div>
      </div>
    </div>
  </div>



  <script>

  var socket = io();
  const self = this;

  socket.on('level', function (level) {
    console.log(level);
  });
  socket.on('deviceCount', function (count) {
    self.deviceNumAll = count[0];
    self.deviceNumArea1 = count[1];
    self.deviceNumArea2 = count[2];
    self.update();
  });
  socket.on ('name', function (name) {
    // [EventName, Area1, Area2]
    self.eventName = name[0];
    self.areaName1 = name[1];
    self.areaName2 = name[2];
    self.update();
  });

  </script>
</app>
