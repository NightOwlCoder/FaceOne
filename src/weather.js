var _temperature = "N/A";
var _conditions = "N/A";
var _city = "";
var _stocks = ["","","",""];
var _symbols_default = "'MSFT','AAPL','FB','AMZN'";
var sendingMessage=false;
var pendingMethod="";

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
  if (sendingMessage)
  {
    console.log ('message send in progress, skipping '+method);
    pendingMethod=method;
    return;
  }
  sendingMessage=true;
  console.log('sending '+method);
  Pebble.sendAppMessage(
    {
      'KEY_TEMPERATURE': _temperature,
      'KEY_CONDITIONS': _conditions,
      'KEY_CITY': _city,
      'KEY_STOCK_1': _stocks[0],
      'KEY_STOCK_2': _stocks[1],
      'KEY_STOCK_3': _stocks[2],
      'KEY_STOCK_4': _stocks[3]
    },
    function(e) {sendIfPending(method + '['+JSON.stringify(e)+'] sent to Pebble successfully!');},
    function(e) {sendIfPending(method + ' error ['+JSON.stringify(e)+'] sending to Pebble!');}
  );
}

function sendIfPending(msg)
{
  console.log(msg);
  sendingMessage=false; 
  if (pendingMethod) 
    SendMessage(pendingMethod);
  pendingMethod="";
}

function isEmpty(str)
{
  return (!str || 0 === str.length);
}

function locationSuccess(pos)
{
  var lat_long = pos.coords.latitude + ',' + pos.coords.longitude;
  var url_y = 'http://query.yahooapis.com/v1/public/yql?q=select%20woeid,county,city,street%20from%20geo.placefinder%20where%20text=%22'+lat_long+'%22%20and%20gflags=%22R%22&format=json';
  // var url = 'http://api.openweathermap.org/data/2.5/weather?APPID=375ab1310d41419b1b7df6d3a5564377&lat=' + pos.coords.latitude + '&lon=' + pos.coords.longitude;
  console.log('url_y: ' + url_y);

  xhrRequest(url_y, 'GET',
             function(responseText) {
               console.log(responseText);
               var json = JSON.parse(responseText);

               _city = json.query.results.Result.street;
               if (isEmpty(_city))
                 _city = json.query.results.Result.city;
               if (isEmpty(_city))
                 _city = json.query.results.Result.county;

               getAndSendWeather(json.query.results.Result.woeid);    
             }      
            );
}

function getAndSendWeather(woeid)
{
  var w_url = 'http://query.yahooapis.com/v1/public/yql?q=select%20item.condition%20from%20weather.forecast%20where%20woeid%3D'+woeid+'%20and%20u%3D%22f%22&format=json';
  xhrRequest(w_url, 'GET',
             function(responseText) {
               console.log(responseText);
               var json = JSON.parse(responseText);

               //_temperature = Math.round(json.main.temp * (9.0/5.0) - 459.67).toString();
               _temperature = json.query.results.channel.item.condition.temp;
               _conditions = json.query.results.channel.item.condition.text;

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

// Listen for when the watchface is opened
Pebble.addEventListener('ready', function(e) {
  getWeather();
  FetchQuote(_symbols_default);
});

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL('http://nightowlcoder.github.io/faceone.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
  console.log('Configuration window returned: ' + e.response);
});

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
                            //FetchSymbolsToFetch();
                            console.log('appmessage: fetching stocks ' + _symbols_default);
                            //_index = e.payload.GetQuote;
                            FetchQuote(_symbols_default);
                          }                          
                        }
                       );

////////////////////////////////////////////////////////////////
// STOCK STUFF
////////////////////////////////////////////////////////////////

function FetchSymbolsToFetch()
{
  var url = "http://faceone.azurewebsites.net/api/stocks";

  xhrRequest(url, 'GET', 
             function(responseText)
             {
               console.log(responseText);

               var response = JSON.parse(responseText);
               console.log(response);
               console.log(response[0].Symbol);
             }
            );
}

function FetchQuote(symbols)
{
  //var url = "http://dev.markitondemand.com/Api/Quote/json?symbol=" + symbol;
  var query = 'select * from yahoo.finance.quotes where symbol in ('+symbols+')';
  var url = "http://query.yahooapis.com/v1/public/yql?q=" + encodeURIComponent(query) + '&env=http://datatables.org/alltables.env&format=json';
  console.log(query);
  console.log(url);

  xhrRequest(url, 'GET', 
             function(responseText)
             {
               var response = JSON.parse(responseText);
               if (!response.query)
               {
                 SendMessage("price error");
               }

               setStockPrice(0, response.query.results.quote[0]);
               setStockPrice(1, response.query.results.quote[1]);
               setStockPrice(2, response.query.results.quote[2]);
               setStockPrice(3, response.query.results.quote[3]);
               SendMessage("quote");
             }
            );
}

function setStockPrice(index, data)
{
  _stocks[index] = data.symbol + ' ' + data.LastTradePriceOnly + ' (' + data.Change + ')';
}
