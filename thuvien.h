//=========Biến chứa mã HTLM Website==//
const char MainPage[] PROGMEM = R"=====(
  <!DOCTYPE html> 
  <html>
   <head> 
       <title>E-SMART HOME</title> 
       <style> 
          body {text-align:center;background-color:#222222; color:white}
          input {
            height:25px; 
            width:270px;
            font-size:20px;
            margin: 10px auto;
          }
          #content {
            border: white solid 1px; 
            padding:5px;  
            height:375px; 
            width:330px; 
            border-radius:20px;
            margin: 0 auto
          }
          .ledstatus{
              outline: none;
              margin: 15px 5px;
              width: 60px;
              height: 60px;
              border-radius: 50%;
              -moz-border-radius: 50%;
              -webkit-border-radius: 50%;
              color:white;
              font-size:20px;
              font-weight: bold;
          }
          #ledstatus1{
              border: solid 2px #23C48E;
              background: #23C48E;
          }
          #ledstatus2{
              border: solid 2px #ED9D00;
              background: #ED9D00;
          }
          #ledstatus3{
              border: solid 2px #5F7CD8;
              background: #5F7CD8;
          }
          #ledstatus4{
              border: solid 2px #03C0F8;
              background: #03C0F8;
          }
          #ledconnect{
              outline: none;
              margin: 0px 5px -1px 5px;
              width: 15px;
              height: 15px;
              border: solid 1px #00EE00;
              background-color: #00EE00;
              border-radius: 50%;
              -moz-border-radius: 50%;
              -webkit-border-radius: 50%;
          }
          .button {
            height:65px; 
            width:70px; 
            margin:10px 0;
            background-color:#3C89BC;
            border-radius:10px;
            outline:none;
            color:white;
            font-size:25px;
            font-weight: bold;
          }
          .button_all{
            height:70px; 
            width:140px; 
            margin:10px 0;
            outline:none;
            color:white;
            font-size:20px;
            font-weight: bold;
          }
          #button_on {
            background-color:#00BB00;
            border-radius:10px;
          }
          #button_off {
            background-color:#CC3300;
            border-radius:10px;
          }
          .button_setup {
            height:30px; 
            width:280px; 
            margin:10px 0;
            background-color:#3C89BC;
            border-radius:10px;
            outline:none;
            color:white;
            font-size:20px;
            font-weight: bold;
          }
          .button_wifi{
            height:50px; 
            width:90px; 
            margin:10px 0;
            outline:none;
            color:white;
            font-size:15px;
            font-weight: bold;
          }
          #button_save {
            background-color:#00BB00;
            border-radius:5px;
          }
          #button_restart {
            background-color:#FF9900;
            border-radius:5px;
          }
          #button_reset {
            background-color:#CC3300;
            border-radius:5px;
          }
       </style>
       <meta name="viewport" content="width=device-width,user-scalable=0" charset="UTF-8">
   </head>
   <body> 
      <div><h1>E-SMART HOME</h1></div>
      <div id="content"> 
        <div id="homecontrol" style="height:340px";>
          <div>
            <input id="ledstatus1" class="ledstatus" type="button" value="ON"/>
            <input id="ledstatus2" class="ledstatus" type="button" value="ON"/>
            
          </div>
           <div>
            <button class="button" onclick=getbutton(1)>1</button>
            <button class="button" onclick=getbutton(2)>2</button>
             
          </div>
          <div>
             
          </div>
          <div><input class="button_setup" type="button" onclick="configurewifi()" value="Configure WiFi"/></div>
          <div>IP connected: <b><pan id="ipconnected"></pan></b></div>
        </div>
        
        <div id="wifisetup" style="height:340px; font-size:20px; display:none";>
          <div style="text-align:left; width:270px; margin:5px 25px">SSID: </div>
          <div><input id="ssid"/></div>
          <div style="text-align:left; width:270px; margin:5px 25px">Password: </div>
          <div><input id="pass"/></div>
          <div>
            <button id="button_save" class="button_wifi" onclick="writeEEPROM()">SAVE</button>
            <button id="button_restart" class="button_wifi" onclick="restartESP()">RESTART</button>
            <button id="button_reset" class="button_wifi" onclick="clearEEPROM()">RESET</button>
          </div>
          <div><input class="button_setup" type="button" onclick="backHOME()" value="Back home"/></div>
          <div id="reponsetext"></div>
        </div>
        <div><input id="ledconnect" type="button"/>Connection status</div>
      </div>
      <div id="footer">
        <p><i>Copyright by <b>Điện thông minh E-SMART</b> 
          <br>Email: <b>blue.sky.le.huy@gmail.com</b></i>
        </p>
      </div>
      <script>
        //-----------Hàm khởi tạo đối tượng request----------------
        function create_obj(){
          td = navigator.appName;
          if(td == "Microsoft Internet Explorer"){
            obj = new ActiveXObject("Microsoft.XMLHTTP");
          }else{
            obj = new XMLHttpRequest();
          }
          return obj;
        }
        //------------Khởi tạo biến toàn cục-----------------------------
        var xhttp = create_obj();//Đối tượng request cho setup wifi
        var xhttp_statusD = create_obj();//Đối tượng request cho cập nhật trạng thái
        var d1,d2,d3,d4;
        var ledconnect = 1;
        //===================Khởi tạo ban đầu khi load trang=============
        window.onload = function(){
          document.getElementById("homecontrol").style.display = "block";
          document.getElementById("wifisetup").style.display = "none";
          getIPconnect();//Gửi yêu cầu lấy IP kết nối
          getstatusD();//Gửi yêu cầu lấy trạng thái các chân điều khiển
        }
        //===================IPconnect====================================
        //--------Tạo request lấy địa chỉ IP kết nối----------------------
        function getIPconnect(){
          xhttp.open("GET","/getIP",true);
          xhttp.onreadystatechange = process_ip;//nhận reponse 
          xhttp.send();
        }
        //-----------Kiểm tra response IP và hiển thị------------------
        function process_ip(){
          if(xhttp.readyState == 4 && xhttp.status == 200){
            //------Updat data sử dụng javascript----------
            ketqua = xhttp.responseText; 
            document.getElementById("ipconnected").innerHTML=ketqua;       
          }
        }
        //===========Cập nhật trạng thái Led===============================
        //-----------Gửi yêu cầu lấy trạng thái các chân D-----------------
        function getstatusD(){
          xhttp_statusD.open("GET","/getSTATUSD",true);
          xhttp_statusD.onreadystatechange = process_statusD;
          xhttp_statusD.send();
        }
        //-----------Kiểm tra và nhận response EEPROM----------------------
        function process_statusD(){
          if(xhttp_statusD.readyState == 4 && xhttp_statusD.status == 200){
            if(ledconnect == 1){
              ledconnect = 0;
              document.getElementById("ledconnect").style.background = "#222222";
            }else{
              ledconnect = 1;
              document.getElementById("ledconnect").style.background = "#00EE00";
            }
            //------Update trạng thái led tín hiệu lên panel điều khiển----
            var statusD = xhttp_statusD.responseText;
            var obj = JSON.parse(statusD);
            d1 = obj.D1;
            d2 = obj.D2;
            //d3 = obj.D3;
            //d4 = obj.D4;
            //updateLedstatus(d1,d2,d3,d4);
            updateLedstatus(d1,d2);
          }
        }
        //----------Hiển thị trạng thái Led lên trình duyệt---------------------
        //function updateLedstatus(D1,D2,D3,D4){
        function updateLedstatus(D1,D2){
          if(D1 == "1"){
              document.getElementById("ledstatus1").value = "OFF";
              document.getElementById("ledstatus1").style.background = "#222222";
            }else{
              document.getElementById("ledstatus1").value = "ON";
              document.getElementById("ledstatus1").style.background = "#23C48E";
            }
            if(D2 == "1"){
              document.getElementById("ledstatus2").value = "OFF";
              document.getElementById("ledstatus2").style.background = "#222222";
            }else{
              document.getElementById("ledstatus2").value = "ON";
              document.getElementById("ledstatus2").style.background = "#ED9D00";
            }
            //if(D3 == "1"){
              //document.getElementById("ledstatus3").value = "OFF";
             // document.getElementById("ledstatus3").style.background = "#222222";
            //}else{
             // document.getElementById("ledstatus3").value = "ON";
             // document.getElementById("ledstatus3").style.background = "#5F7CD8";
           // }
           // if(D4 == "1"){
            //  document.getElementById("ledstatus4").value = "OFF";
           //   document.getElementById("ledstatus4").style.background = "#222222";
           // }else{
           //   document.getElementById("ledstatus4").value = "ON";
            //  document.getElementById("ledstatus4").style.background = "#03C0F8";
            //}
        }
        //===========Tạo và gửi request khi ấn button============================
        //-----------Thiết lập dữ liệu và gửi request ON/OFF D4---
        function getbutton(n){
          switch (n){
            case 1:
                var ledstatus1 = document.getElementById("ledstatus1").value;
                if(ledstatus1 == "ON"){
                  xhttp_statusD.open("GET","/D1off",true);
                }else{
                  xhttp_statusD.open("GET","/D1on",true);
                }
                xhttp_statusD.onreadystatechange = process_statusD;
                xhttp_statusD.send();
                break;
            case 2:
                var ledstatus2 = document.getElementById("ledstatus2").value;
                if(ledstatus2 == "ON"){
                  xhttp_statusD.open("GET","/D2off",true);
                }else{
                  xhttp_statusD.open("GET","/D2on",true);
                }
                xhttp_statusD.onreadystatechange = process_statusD;
                xhttp_statusD.send();
                break;
            case 3:
                var ledstatus3 = document.getElementById("ledstatus3").value;
                if(ledstatus3 == "ON"){
                  xhttp_statusD.open("GET","/D3off",true);
                }else{
                  xhttp_statusD.open("GET","/D3on",true);
                }
                xhttp_statusD.onreadystatechange = process_statusD;
                xhttp_statusD.send();
                break;
            case 4:
               var ledstatus4 = document.getElementById("ledstatus4").value;
                if(ledstatus4 == "ON"){
                  xhttp_statusD.open("GET","/D4off",true);
                }else{
                  xhttp_statusD.open("GET","/D4on",true);
                }
                xhttp_statusD.onreadystatechange = process_statusD;
                xhttp_statusD.send();
                break;
          }
        }
        function getbuttonall(m){
          if(m == "on"){
            xhttp_statusD.open("GET","/Allon",true);
          }else if(m=="off"){
            xhttp_statusD.open("GET","/Alloff",true);
          }
          xhttp_statusD.onreadystatechange = process_statusD;
          xhttp_statusD.send();
        }
        //===========Configure WiFi=====================================
        function configurewifi(){
          document.getElementById("homecontrol").style.display = "none";
          document.getElementById("wifisetup").style.display = "block";
        }
        //-----------Thiết lập dữ liệu và gửi request ssid và password---
        function writeEEPROM(){
          if(Empty(document.getElementById("ssid"), "Please enter ssid!")&&Empty(document.getElementById("pass"), "Please enter password")){
            var ssid = document.getElementById("ssid").value;
            var pass = document.getElementById("pass").value;
            xhttp.open("GET","/writeEEPROM?ssid="+ssid+"&pass="+pass,true);
            xhttp.onreadystatechange = process;//nhận reponse 
            xhttp.send();
          }
        }
        function clearEEPROM(){
          if(confirm("Do you want to delete all saved wifi configurations?")){
            xhttp.open("GET","/clearEEPROM",true);
            xhttp.onreadystatechange = process;//nhận reponse 
            xhttp.send();
          }
        }
        function restartESP(){
          if(confirm("Do you want to reboot the device?")){
            xhttp.open("GET","/restartESP",true);
            xhttp.send();
            alert("Device is restarting! If no wifi is found please press reset!");
          }
        }
        //-----------Kiểm tra response -------------------------------------------
        function process(){
          if(xhttp.readyState == 4 && xhttp.status == 200){
            //------Updat data sử dụng javascript----------
            ketqua = xhttp.responseText; 
            document.getElementById("reponsetext").innerHTML=ketqua;       
          }
        }
       //============Hàm thực hiện chứ năng khác================================
       //--------Cập nhật trạng thái tự động sau 2000 giây----------------------
        setInterval(function() {
          getstatusD();
        },2000);
       //--------Load lại trang để quay về Home control-------------------------
        function backHOME(){
          window.onload();
        }
       //----------------------------CHECK EMPTY--------------------------------
       function Empty(element, AlertMessage){
          if(element.value.trim()== ""){
            alert(AlertMessage);
            element.focus();
            return false;
          }else{
            return true;
          }
       }
       //------------------------------------------------------------------------
      </script>
   </body> 
  </html>
)=====";
