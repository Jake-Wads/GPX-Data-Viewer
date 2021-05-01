
var latList;
var lonList;

// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function() {
    // On page-load AJAX Example
    jQuery.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/readUploads',   //The server endpoint we are connecting to
        data: {
            data1: "Value 1",
            data2:1234.56
        },
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            for (let list of data.dataList) {
                jQuery('#fileTable').append('<tr> <th><a href="'+list.file+'" download >'+list.file+'</a></th> <th>'+list.version+'</th> <th>'+list.creator+'</th> <th>'+list.numWaypoints+'</th> <th>'+list.numRoutes+'</th> <th>'+list.numTracks+'</th> </tr>');
            }
            //We write the object to the console to show that the request was successful
            //console.log(data);

        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error); 
        }
    });

    $(document).ready(function() {

        document.getElementById('filename').value = 'example.gpx';
        document.getElementById('creator').value = 'Name';
        document.getElementById('version').value = '1.1';

        document.getElementById('lat').value = '';
        document.getElementById('lon').value = '';
        document.getElementById('routeName').value = '';

        document.getElementById('slat').value = '';
        document.getElementById('slon').value = '';
        document.getElementById('elat').value = '';
        document.getElementById('elon').value = '';
        document.getElementById('delta').value = '';

        latList = [];
        lonList = [];

        // On page-load AJAX Example
        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/readDropDown',   //The server endpoint we are connecting to
            data: {
                data1: "Value 1",
                data2:1234.56
            },
            success: function (data) {
                /*  Do something with returned object
                    Note that what we get is an object, not a string, 
                    so we do not need to parse it on the server.
                    JavaScript really does handle JSONs seamlessly
                */
                for (let name of data.dataList) {
                    var x = document.getElementById("gpxFiles");
                    var y = document.getElementById("gpxFiles2");
                    var option = document.createElement("option");
                    var option2 = document.createElement("option");
                    option.text = name;
                    option2.text = name;
                    x.add(option);
                    y.add(option2);
                }
                
    
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });
    });


    $('#LoadButton').click(function() {
        // On page-load AJAX Example
        let x = document.getElementById('gpxFiles');
        let name = x.value;

        let table = document.getElementById('compTable');
        let j = 0;
        for (j = table.rows.length-1; j > 0; j--) {
            table.deleteRow(j);
        }

        let select = document.getElementById("compList");
        let length = select.options.length;
        let p = 0;
        for (p = length-1; p >= 0; p--) {
            select.options[p] = null;
        }

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/loadGPX',   //The server endpoint we are connecting to
            data: {
                data1: name
            },
            success: function (data) {
                /*  Do something with returned object
                    Note that what we get is an object, not a string, 
                    so we do not need to parse it on the server.
                    JavaScript really does handle JSONs seamlessly
                */
                let i = 0;
                for (i = 0; i < data.dataItem1.length; i++) {
                    jQuery('#compTable').append('<tr> <th> Route '+ (i+1) +' </th> <th>'+data.dataItem1[i].name+'</th> <th>'+data.dataItem1[i].numPoints+'</th> <th>'+data.dataItem1[i].len+'m </th> <th>'+data.dataItem1[i].loop+'</th> </tr>');
                }
                for (i = 0; i < data.dataItem2.length; i++) {
                    jQuery('#compTable').append('<tr> <th> Track '+ (i+1) +' </th> <th>'+data.dataItem2[i].name+'</th> <th> '+data.dataItem2[i].numPoints+' </th> <th>'+data.dataItem2[i].len+'m </th> <th>'+data.dataItem2[i].loop+'</th> </tr>');
                }

                let compTable = document.getElementById('compTable');
                var drop = document.getElementById("compList");
                for (i = 1; i < compTable.rows.length; i++) {
                    let compName = compTable.rows[i].cells[0].innerHTML;
                    var option = document.createElement("option");
                    option.text = compName;
                    drop.add(option);
                }
    
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

               
    });

    $('#showDataButton').click(function() {

        let x = document.getElementById('compList');
        let name = x.value;

        let y = document.getElementById('gpxFiles');
        let filename = y.value;

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/showData',   //The server endpoint we are connecting to
            data: {
                data1: name,
                data2: filename
            },
            success: function (data) {

                let str = '';
                
                let i = 0;
                for (i = 0; i < data.dataItem1.length; i++) {
                    str += ('Name: ' + data.dataItem1[i].name + '\nContents: ' + data.dataItem1[i].value + '\n');
                }

                if (data.dataItem1.length == 0) {
                    alert('No other data in component');
                } else {
                    alert(str);
                }
                
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    $('#renameButton').click(function() {

        let name = prompt("Please enter the new name for the component:", "Component Name");
        let x = document.getElementById('compList');
        let comp = x.value;
        let y = document.getElementById('gpxFiles');
        let filename = y.value;

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/rename',   //The server endpoint we are connecting to
            data: {
                data1: name,
                data2: comp,
                data3: filename
            },
            success: function (data) {
                
                alert('Success, refreshing page');
                location.reload();
                
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    $('#createGPXButton').click(function() {

        let filename = document.getElementById('filename').value;
        let name = document.getElementById('creator').value;
        let version = document.getElementById('version').value;

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/createGPX',   //The server endpoint we are connecting to
            data: {
                data1: filename,
                data2: name,
                data3: version
            },
            success: function (data) {

                if (data.dataItem1 == 'fileError') {
                    console.log('help me');
                    alert("Error: filename already exists");
                } else if (data.dataItem1 == 'nameError') {
                    alert("Error: filename does not end with \'.gpx\'");
                } else if (data.dataItem1 == 'writeError') {
                    alert("Error: file write failed");
                } else {
                    alert('Success, refreshing page');
                    location.reload();
                }
                
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    $('#addWaypointButton').click(function() {

        let lat = document.getElementById('lat').value
        let lon = document.getElementById('lon').value

        let counter = document.getElementById('waypointCount');

        if (!isNaN(lat) && lat != '' && parseInt(lat) >= -90 && parseInt(lat) <= 90) {
            if (!isNaN(lon) && lon != '' && parseInt(lon) >= -180 && parseInt(lon) <= 180) {
                latList.push(lat);
                lonList.push(lon);
                counter.innerText = ('Current Waypoint Count: ' + latList.length);
                document.getElementById('lat').value = '';
                document.getElementById('lon').value = '';
            } else {
                alert('Error: Longitude invalid');
            }
        } else {
            alert('Error: Latitude invalid');
        }

    });

    $('#addRouteButton').click(function() {

        let routeName = document.getElementById('routeName').value;
        let filename = document.getElementById('gpxFiles2').value;

        if (routeName == '') {
            alert('Please enter a name for the route');
        } else {

            jQuery.ajax({
                type: 'get',            //Request type
                dataType: 'json',       //Data type - we will use JSON for almost everything 
                url: '/addRoute',   //The server endpoint we are connecting to
                data: {
                    data1: latList,
                    data2: lonList,
                    data3: routeName,
                    data4: filename
                },
                success: function (data) {
                    console.log('back');
                
                    if (data.dataItem1) {
                        alert('Success, refreshing page');
                        location.reload();
                    } else {
                        alert('Error adding to file');
                    }
                
                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    $('#blah').html("On page load, received error from server");
                    console.log(error); 
                }
            });

        }

    });

    $('#findPathButton').click(function() {

        let startLat = document.getElementById('slat').value;
        let startLon = document.getElementById('slon').value;
        let endLat = document.getElementById('elat').value;
        let endLon = document.getElementById('elon').value;
        let delta = document.getElementById('delta').value;

        if (isNaN(startLat) || startLat == '' || parseInt(startLat) < -90 || parseInt(startLat) > 90) {
            alert('Error: Start Latitude invalid');
        } else if (isNaN(startLon) || startLon == '' || parseInt(startLon) < -180 || parseInt(startLon) > 180) {
            alert('Error: Start Longitude invalid');
        } else if (isNaN(endLat) || endLat == '' || parseInt(endLat) < -90 || parseInt(endLat) > 90) {
            alert('Error: End Latitude invalid');
        } else if (isNaN(endLon) || endLon == '' || parseInt(endLon) < -180 || parseInt(endLon) > 180) {
            alert('Error: End Longitude invalid');
        } else if (isNaN(delta) || delta == '' || parseInt(delta) <= 0) {
            alert('Error: Delta invalid');
        } else {
            alert('Start Latitude: ' + startLat + '\nStart Longitude: ' + startLon + '\nEnd Latitude: ' + endLat + '\nEnd Longitude: ' + endLon + '\nDelta: ' + delta);
        }

    });

    $('#connectDatabaseButton').click(function() {
        console.log('Connecting to Database');

        let username = document.getElementById('username').value;
        let password = document.getElementById('password').value;
        let database = document.getElementById('database').value;

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/connectToDatabase',   //The server endpoint we are connecting to
            data: {
                data1: username,
                data2: password,
                data3: database
            },
            success: function (data) {
                if (data.dataItem1 == 'false') {
                    console.log('Login Error');
                    alert('Login Error: Please re-enter your username, password and database');
                } else if (data.dataItem1 == 'true') {
                    console.log('Success');
                } else {
                    console.log('what?');
                }
                
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    $('#storeFilesButton').click(function() {
        console.log('Storing all Files');

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/storeFiles',   //The server endpoint we are connecting to
            data: {
                data1: 'test'
            },
            success: function (data) {
                if (data.dataItem1 == 'false') {
                    console.log('Store Error');
                    alert('Store Error: Please try again');
                } else if (data.dataItem1 == 'true') {
                    console.log('Success');
                } else {
                    console.log('what?');
                }
                
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    $('#clearDataButton').click(function() {
        console.log('Clearing all Data');

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/clearData',   //The server endpoint we are connecting to
            data: {
                data1: 'test'
            },
            success: function (data) {
                if (data.dataItem1 == 'false') {
                    console.log('Clear Error');
                    alert('Clear Error: Please try again');
                } else if (data.dataItem1 == 'true') {
                    console.log('Success');
                    alert('Data successfully cleared');
                } else {
                    console.log('what?');
                }
                
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    $('#displayDBButton').click(function() {
        console.log('Displaying DB Status');

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/displayDBData',   //The server endpoint we are connecting to
            data: {
                data1: 'test'
            },
            success: function (data) {
                
                alert('Database has ' + data.dataItem1 + ' files, ' + data.dataItem2 + ' routes, and ' + data.dataItem3 + ' points');
                
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    $('#displayAllRoutesButton').click(function() {
        console.log('Displaying All Routes');

        let table = document.getElementById('routesTable');
        let j = 0;
        for (j = table.rows.length-1; j > 0; j--) {
            table.deleteRow(j);
        }

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/displayAllRoutes',   //The server endpoint we are connecting to
            data: {
                data1: 'test'
            },
            success: function (data) {
                
                let rows = data.dataItem1;

                for (let row of rows) {
                    jQuery('#routesTable').append('<tr> <th> '+ row.route_id +' </th> <th>'+row.route_name+'</th> <th>'+ row.route_len +'m </th> <th>'+row.gpx_id+' </th> </tr>');
                }
                
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    $('#displayAllRoutesButton2').click(function() {
        console.log('Displaying All Routes2');

        let table = document.getElementById('routesTable');
        let j = 0;
        for (j = table.rows.length-1; j > 0; j--) {
            table.deleteRow(j);
        }

        jQuery.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/displayAllRoutes2',   //The server endpoint we are connecting to
            data: {
                data1: 'test'
            },
            success: function (data) {
                
                let rows = data.dataItem1;

                for (let row of rows) {
                    jQuery('#routesTable').append('<tr> <th> '+ row.route_id +' </th> <th>'+row.route_name+'</th> <th>'+ row.route_len +'m </th> <th>'+row.gpx_id+' </th> </tr>');
                }
                
            },
            fail: function(error) {
                // Non-200 return, do something with error
                $('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });
});