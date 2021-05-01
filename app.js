'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname + '/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// get the client
const mysql = require('mysql2/promise');
let connection;

// Send HTML at root, do not change
app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname + '/public/index.html'));
});

// Send Style, do not change
app.get('/style.css', function (req, res) {
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname + '/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js', function (req, res) {
  fs.readFile(path.join(__dirname + '/public/index.js'), 'utf8', function (err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, { compact: true, controlFlowFlattening: true });
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function (req, res) {
  if (!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function (err) {
    if (err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function (req, res) {
  fs.stat('uploads/' + req.params.name, function (err, stat) {
    if (err == null) {
      res.sendFile(path.join(__dirname + '/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: ' + err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 


let sharedLib = ffi.Library('./parser/bin/libgpxparser.so', {
  'getGPXJSON': ['String', ['String']],	//return type first, argument list second
  'getRoutesJSON': ['String', ['String']],
  'getTracksJSON': ['String', ['String']],
  'gpxDataListToJSON': ['String', ['String', 'String']],
  'renameComp': ['void', ['String', 'String', 'String']],
  'createNewGPX': ['bool', ['String', 'String']],
  'addRouteToFile': ['bool', ['String', 'String', 'String', 'int']],
  'getPointsJSON': ['String', ['String']]

});


app.get('/readUploads', function (req, res) {
  var list = [];

  const fs = require('fs');

  let dir = './uploads';
  let files = fs.readdirSync(dir);

  for (let file of files) {
    if (file != '.DS_Store') {
      let gpxJSON = sharedLib.getGPXJSON("./uploads/" + file);

      let data = JSON.parse(gpxJSON);
      data.file = file;
      list.push(data);
    }
  }
  res.send(
    {
      dataList: list
    }
  );

});

app.get('/readDropDown', function (req, res) {
  var list = [];

  const fs = require('fs');

  let dir = './uploads';
  let files = fs.readdirSync(dir);

  for (let file of files) {
    if (file != '.DS_Store') {
      list.push(file);
    }
  }
  res.send(
    {
      dataList: list
    }
  );

});

app.get('/loadGPX', function (req, res) {
  const fs = require('fs');

  let dir = './uploads';
  let files = fs.readdirSync(dir);

  let filename = req.query.data1;

  for (let file of files) {
    if (filename == file) {
      let routesJSON = sharedLib.getRoutesJSON("./uploads/" + file);
      let tracksJSON = sharedLib.getTracksJSON("./uploads/" + file);
      var JSONDataR = JSON.parse(routesJSON);
      var JSONDataT = JSON.parse(tracksJSON);
    }
  }
  res.send(
    {
      dataItem1: JSONDataR,
      dataItem2: JSONDataT
    }
  );

});

app.get('/showData', function (req, res) {
  let compName = req.query.data1;
  let filename = req.query.data2;

  let dataJSON = sharedLib.gpxDataListToJSON("./uploads/" + filename, compName);
  var JSONData = JSON.parse(dataJSON);

  res.send(
    {
      dataItem1: JSONData
    }
  );
});

app.get('/rename', function (req, res) {
  let name = req.query.data1;
  let comp = req.query.data2;
  let filename = req.query.data3;

  sharedLib.renameComp("./uploads/" + filename, name, comp);

  let temp = 'success';

  res.send(
    {
      dataItem1: temp
    }
  );

});

app.get('/createGPX', function (req, res) {

  let filename = req.query.data1;
  let creator = req.query.data2;
  let version = req.query.data3;

  let flag = 'nameError';

  if (filename.endsWith('.gpx')) {
    flag = 'succ';
  }

  const fs = require('fs');

  let dir = './uploads';
  let files = fs.readdirSync(dir);
  for (let file of files) {
    if (filename == file) {
      flag = 'fileError';
    }
  }

  let test = true;

  if (flag == 'succ') {
    test = sharedLib.createNewGPX("./uploads/" + filename, '{\"version\":' + version + ',\"creator\":\"' + creator + '\"}');
  }

  if (test == false) {
    flag = 'writeError';
  }

  res.send(
    {
      dataItem1: flag
    }
  );
});

app.get('/addRoute', function (req, res) {

  let latList = req.query.data1;
  let lonList = req.query.data2;
  let JSONList = '';
  let filename = req.query.data4;

  let i = 0;
  for (i = 0; i < latList.length; i++) {
    JSONList += ('{\"lat\":' + latList[i] + ',\"lon\":' + lonList[i] + '}|');
  }

  let flag = sharedLib.addRouteToFile("./uploads/" + filename, req.query.data3, JSONList, latList.length);

  res.send(
    {
      dataItem1: flag
    }
  );

});

app.get('/connectToDatabase', async function (req, res) {

  try {
    let dbConf = {
      host: 'dursley.socs.uoguelph.ca',
      user: req.query.data1,
      password: req.query.data2,
      database: req.query.data3
    };

    //SQL
    connection = await mysql.createConnection(dbConf);

    await connection.execute('CREATE TABLE IF NOT EXISTS FILE(gpx_id INT AUTO_INCREMENT PRIMARY KEY, file_name VARCHAR(60) NOT NULL, ver DECIMAL(2,1) NOT NULL, creator VARCHAR(256) NOT NULL);');
    await connection.execute('CREATE TABLE IF NOT EXISTS ROUTE(route_id INT AUTO_INCREMENT PRIMARY KEY, route_name VARCHAR(256), route_len FLOAT(15,7) NOT NULL, gpx_id INT NOT NULL, FOREIGN KEY(gpx_id) REFERENCES FILE(gpx_id) ON DELETE CASCADE);');
    await connection.execute('CREATE TABLE IF NOT EXISTS POINT(point_id INT AUTO_INCREMENT PRIMARY KEY, point_index INT NOT NULL, latitude DECIMAL(11,7) NOT NULL, longitude DECIMAL(11,7) NOT NULL, point_name VARCHAR(256), route_id INT NOT NULL, FOREIGN KEY(route_id) REFERENCES ROUTE(route_id) ON DELETE CASCADE);');

    res.send(
      {
        dataItem1: 'true'
      }
    );
  } catch (e) {
    res.send(
      {
        dataItem1: 'false'
      }
    );
  }

});

app.get('/storeFiles', async function (req, res) {

  await connection.execute('DELETE FROM POINT');
  await connection.execute('DELETE FROM ROUTE');
  await connection.execute('DELETE FROM FILE');
  await connection.execute('ALTER TABLE FILE AUTO_INCREMENT = 1;');
  await connection.execute('ALTER TABLE ROUTE AUTO_INCREMENT = 1;');
  await connection.execute('ALTER TABLE POINT AUTO_INCREMENT = 1;');

  const fs = require('fs');

  let dir = './uploads';
  let files = fs.readdirSync(dir);

  try {

    let fileID = 0;
    let routeID = 0;
    let pointID = 0;
    
    for (let file of files) {
      if (file != '.DS_Store') {
        console.log(file);

        //FILE
        fileID++;
        let fileJSON = sharedLib.getGPXJSON("./uploads/" + file);
        let JSONdata = JSON.parse(fileJSON);
        await connection.execute("INSERT INTO FILE (gpx_id, file_name, ver, creator) VALUES ('" + fileID + "', '" + file + "', '" + JSONdata.version + "', '" + JSONdata.creator + "');");

        //ROUTE
        fileJSON = sharedLib.getRoutesJSON("./uploads/" + file);
        JSONdata = JSON.parse(fileJSON);
        for (let data of JSONdata) {
          routeID++;
          await connection.execute("INSERT INTO ROUTE (route_id, route_name, route_len, gpx_id) VALUES ('" + routeID + "', '" + data.name + "', '" + data.len + "', '" + fileID + "');");
        }

        //POINT
        fileJSON = sharedLib.getPointsJSON("./uploads/" + file);
        JSONdata = JSON.parse(fileJSON);
        console.log(JSONdata);
        for (let data of JSONdata) {
          pointID++;
          await connection.execute("INSERT INTO POINT (point_id, point_index, latitude, longitude, point_name, route_id) VALUES ('" + pointID + "', '" + data.index + "', '" + data.lat + "', '" + data.lon + "', '" + data.name + "', '" + routeID + "');");
        }
      }
    }

    const [rows1, fields1] = await connection.execute("SELECT * from FILE ORDER BY file_name");
    for (let row of rows1) {
      console.log("gpx_id: " + row.gpx_id + " creator: " + row.creator + " ver: " + row.ver + " file_name: " + row.file_name);
    }
    const [rows2, fields2] = await connection.execute("SELECT * from ROUTE ORDER BY route_name");
    for (let row of rows2) {
      console.log("route_id: " + row.route_id + " route_name: " + row.route_name + " route_len: " + row.route_len + " gpx_id: " + row.gpx_id);
    }
    const [rows3, fields3] = await connection.execute("SELECT * from POINT ORDER BY point_name");
    for (let row of rows3) {
      console.log("point_id: " + row.point_id + " point_index: " + row.point_index + " lat: " + row.latitude + " lon: " + row.longitude + " name: " + row.point_name + " route_id: " + row.route_id);
    }

    res.send(
      {
        dataItem1: 'true'
      }
    );
  } catch (e) {
    res.send(
      {
        dataItem1: 'false'
      }
    );
  }
});

app.get('/clearData', async function (req, res) {

  try {

    await connection.execute('DELETE FROM POINT');
    await connection.execute('DELETE FROM ROUTE');
    await connection.execute('DELETE FROM FILE');
    await connection.execute('ALTER TABLE FILE AUTO_INCREMENT = 1;');
    await connection.execute('ALTER TABLE ROUTE AUTO_INCREMENT = 1;');
    await connection.execute('ALTER TABLE POINT AUTO_INCREMENT = 1;');

    res.send(
      {
        dataItem1: 'true'
      }
    );
  } catch (e) {
    res.send(
      {
        dataItem1: 'false'
      }
    );
  }

});

app.get('/displayDBData', async function (req, res) {

  let fileNum = 0;
  let routeNum = 0;
  let pointNum = 0;

  const [rows1, fields1] = await connection.execute('SELECT COUNT(*) AS num FROM FILE');
  fileNum = rows1[0].num;
  const [rows2, fields2] = await connection.execute('SELECT COUNT(*) AS num FROM ROUTE');
  routeNum = rows2[0].num;
  const [rows3, fields3] = await connection.execute('SELECT COUNT(*) AS num FROM POINT');
  pointNum = rows3[0].num;

  res.send(
    {
      dataItem1: fileNum,
      dataItem2: routeNum,
      dataItem3: pointNum
    }
  );

});

app.get('/displayAllRoutes', async function (req, res) {

  const [rows, fields] = await connection.execute("SELECT * from ROUTE ORDER BY route_name");

  res.send(
    {
      dataItem1: rows
    }
  );

});

app.get('/displayAllRoutes2', async function (req, res) {

  const [rows, fields] = await connection.execute("SELECT * from ROUTE ORDER BY route_len");

  res.send(
    {
      dataItem1: rows
    }
  );

});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);