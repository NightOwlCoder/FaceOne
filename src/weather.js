var _temperature = "N/A";
var _conditions = "N/A";
var _city = "locating...";
var _symbol = "DIS";
var _price = "retrieving...";
var _change = "0";

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function SendMessage(method)
{
  Pebble.sendAppMessage(
    {
      'KEY_TEMPERATURE': _temperature,
      'KEY_CONDITIONS': _conditions,
      'KEY_CITY': _city,
      'KEY_SYMBOL': _symbol,
      'KEY_PRICE': _price
    },
    function(e) { console.log(method + ' sent to Pebble successfully!'); },
    function(e) { console.log(method + ' error sending to Pebble!'); }
  );
}

function locationSuccess(pos)
{
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude;

  xhrRequest(url, 'GET', 
             function(responseText) {
               console.log(responseText);

               var json = JSON.parse(responseText);
               _temperature = Math.round(json.main.temp * (9.0/5.0) - 459.67).toString();
               _conditions = json.weather[0].main;      
               _city = json.name;

               SendMessage("weather");
             }      
            );
}

function locationError(err)
{ 
  _temperature = "N/A";
  _conditions = "N/A";
  _city = "location N/A";
  console.warn('location error (' + err.code + '): ' + err.message); 
  SendMessage("location error");
} 

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

function getQuotes()
{
  var symbol = localStorage.getItem("symbol");

  if (!symbol)
    symbol = "DIS";

  console.log('FETCH ' + symbol);
  FetchQuote(symbol);
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
                        function(e) {
                          console.log('ready: ' + JSON.stringify(e.payload));

                          getWeather();
                          getQuotes();
                        }
                       );

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
                        function(e) {
                          console.log('appmessage: ' + JSON.stringify(e.payload));

                          if (e.payload.RefreshWeather)
                          {
                            console.log('appmessage: fetching weather');
                            getWeather();                              
                          }

                          if (e.payload.GetQuote)
                          {
                            console.log('appmessage: fetching stock ' + e.payload.GetQuote);
                            _symbol = e.payload.GetQuote;
                            getQuotes();
                          }                          
                        }
                       );

////////////////////////////////////////////////////////////////
// STOCK STUFF
////////////////////////////////////////////////////////////////

function FetchQuote(symbol)
{
  _symbol = symbol;
  var url = "http://dev.markitondemand.com/Api/Quote/json?symbol=" + symbol;

  xhrRequest(url, 'GET', 
             function(responseText)
             {
               console.log(responseText);

               var response = JSON.parse(responseText);
               if (response.Message)
               {
                 _price = "N/A";
                 SendMessage("price error");
               }
               if (response.Data)
               {
                 _price = response.Data.LastPrice.toFixed(2);
                 _change = response.Data.Change.toFixed(2);
                 if (response.Data.Change > 0)
                   _change = "+" + _change;         

                 SendMessage("quote");
               }      
             }
            );
}