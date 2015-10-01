// We use the fake "PBL" symbol as default
var defaultSymbol = "DIS";
var symbol = defaultSymbol;

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos)
{
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude;

  xhrRequest(url, 'GET', 
    function(responseText) {
      console.log(responseText);
      
      var json = JSON.parse(responseText);
      var temperature = Math.round(json.main.temp * (9.0/5.0) - 459.67);
      var conditions = json.weather[0].main;      
      var city = json.name;
      
      Pebble.sendAppMessage(
        {
          'KEY_TEMPERATURE': temperature.toString(),
          'KEY_CONDITIONS': conditions,
          'KEY_CITY': city
        },
        function(e) { console.log('Weather info sent to Pebble successfully!'); },
        function(e) { console.log('Error sending weather info to Pebble!'); }
      );
    }      
  );
}

function locationError(err)
{ 
  console.warn('location error (' + err.code + '): ' + err.message); 
  Pebble.sendAppMessage({ 
    'KEY_TEMPERATURE': 'N/A',
    'KEY_CONDITIONS': 'N/A',
    'KEY_CITY': 'Loc unavailable'
    }); 
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
      // Fetch saved symbol from local storage (using
    // standard localStorage webAPI)
    symbol = localStorage.getItem("symbol");
    if (!symbol) {
      symbol = "DIS";
    }    
    console.log('FETCH ' + symbol);
    FetchQuote(symbol);
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');

    getWeather();
    getQuotes();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    
    getWeather();
    getQuotes();
  }                     
);

////////////////////////////////////////////////////////////////
// STOCK STUFF
////////////////////////////////////////////////////////////////

function FetchQuote(symbol) {
  var url = "http://dev.markitondemand.com/Api/Quote/json?symbol=" + symbol;

  xhrRequest(url, 'GET', 
    function(responseText) {
      console.log(responseText);
      
        var response = JSON.parse(responseText);
        if (response.Message)
        {
          // the markitondemand API sends a response with a Message
          // field when the symbol is not found
          Pebble.sendAppMessage(
            { 
              'KEY_SYMBOL': symbol,
              'KEY_PRICE': 'N/A'
            });
        }
        if (response.Data)
        {
          var price = response.Data.LastPrice;
          var change = response.Data.Change.toString();
          if (response.Data.Change > 0)
            change = "+" + response.Data.Change;         
          
          Pebble.sendAppMessage(            
            { 
              'KEY_SYMBOL': symbol + ": $" + price + " (" + change + ")",
              'KEY_PRICE': "$" + price + " (" + change + ")"
            },
            function(e) { console.log('Sent stock info to Pebble!'); },
            function(e) { console.log('Error sending stock info to Pebble!'); }
      );
        }      }
  );
}

// Fetch stock data for a given stock symbol (NYSE or NASDAQ only) from markitondemand.com
// & send the stock price back to the watch via app message
// API documentation at http://dev.markitondemand.com/#doc
function fetchStockQuote(symbol, isInitMsg) {
  var response;
  var req = new XMLHttpRequest();
  // build the GET request
  req.open('GET', "http://dev.markitondemand.com/Api/Quote/json?" +
    "symbol=" + symbol, true);
  req.onload = function(e) {
    
        console.log('req state ' + req.readyState);

    if (req.readyState == 4) {
      // 200 - HTTP OK
      if(req.status == 200) {
        console.log(req.responseText);
        response = JSON.parse(req.responseText);
        var price;
        if (response.Message) {
          // the markitondemand API sends a response with a Message
          // field when the symbol is not found
          Pebble.sendAppMessage(
            { 
              'KEY_SYMBOL': symbol,
              'KEY_PRICE': 'N/A'
            });
        }
        if (response.Data) {
          // data found, look for LastPrice
          price = response.Data.LastPrice;
          console.log(price);

          var msg = {};
          if (isInitMsg) {
            msg.init = true;
            msg.symbol = symbol;
          }
          msg.price = "$" + price.toString();
          Pebble.sendAppMessage(            
            { 
              'KEY_SYMBOL': symbol,
              'KEY_PRICE': "$" + price.toString()
            });
        }
      } else {
        console.log("Request returned error code " + req.status.toString());
      }
    }
  };
  req.send(null);
}