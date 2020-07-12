// Load the AWS SDK for Node.js
var AWS = require('aws-sdk');
// Set the region 
AWS.config.update({region: 'ap-southeast-1'});
// Create the DynamoDB service object
var ddb = new AWS.DynamoDB({apiVersion: '2012-08-10'});
// Set table name
var table = "cypress-psoc";

exports.handler = async (event) => {
	console.log(event)

	if (event.direction == "0") {
		// Remove entry
		var params = {
			TableName: table,
			Key: {
				"data":   { S: event.data },
				"device": { S: event.device },
			},
		};

		// Call DynamoDB to delete the item from the table
		try {
			var result = await ddb.deleteItem(params).promise();
			console.log("Success deleting", result);
		} catch(err) {
			console.log("Error deleting", err);
			console.log(err);
		}
	} else {
		// Insert entry
		const time_now = String(Date.now()); // Unix timestamp in milliseconds

		var params = {
			TableName: table,
			Item: {
				"data":   { S: event.data },
				"device": { S: event.device },
				"time":   { S: time_now },
			}
		};

		// Call DynamoDB to add the item to the table
		try {
			var result = await ddb.putItem(params).promise();
			console.log("Success putting", result);
		} catch(err) {
			console.log("Error putting", err);
			console.log(err);
		}
	}

	// Success response
	const response = {
		statusCode: 200,
		body: JSON.stringify('Completed'),
	};
	return response;
};

