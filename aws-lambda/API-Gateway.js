// Load the AWS SDK for Node.js
var AWS = require('aws-sdk');
// Set the region 
AWS.config.update({region: 'ap-southeast-1'});
// Create the DynamoDB service object
var ddb = new AWS.DynamoDB({apiVersion: '2012-08-10'});
// Create the Document Client interface for DynamoDB
var ddbDocumentClient = new AWS.DynamoDB.DocumentClient();

// Set table name
var tableName = "cypress-psoc";

// Scan for all items in database
async function scanForResultsDdbDc(){
    try {
        var params = {
            TableName: tableName
        };
        var result = await ddbDocumentClient.scan(params).promise()
        return JSON.stringify(result);
    } catch (error) {
        console.error(error);
        return error;
    }
}

// handler
exports.handler = function (event, context, callback) {
	return await scanForResultsDdbDc();
}
