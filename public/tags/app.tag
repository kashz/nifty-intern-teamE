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
  var socket = io();
  const self = this;
  self.eventName = "@nifty IoT インターン";
  self.deviceNumAll = 0;

  self.areaName1 = "18階";
  self.areaName2 = "15階";
  self.deviceNumArea1 = 0;
  self.deviceNumArea2 = 0;

  socket.on('level', function (level) {
    console.log(level);
  });
</app>
