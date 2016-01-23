# Parse.com as a backend for The Things Network
Example implementation of a web application for The Things Network, using Parse.com as a database to store datas comming from the nodes. This example will send push notifications to any device registered everytime a new row is inserted in the database.


## Setup your local NodeRED
`npm install -g node-red` then `node-red`.  
Servers runs on _127.0.0.1:1880_, just import the _node-red.json_ flow and change the output **Create request** with your custom header from Parse.  
You can filter which node address will be saved into the database.


## Google Cloud Messenging
To be able to send Push notifications, you need Google GCM:  
Create a new Google project in your [Developer Console](https://console.developers.google.com/project)  
Use the id of the project as your Google GCM senderID in manifest.json  
More info in [this tutorial](https://developers.google.com/web/fundamentals/getting-started/push-notifications)  


## Create / Setup a Parse application
Create a free acount on [parse.com](https://parse.com), then create an empty project.  

### Create your datas table
In this example we'll use a single table _datas_ that will conveniently save the _node_, _gateway_ and _value_ (all _strings_).

### Use Installations
The cloud code example included in this example sets a hook on _afterSave_ which will trigger after any new row is inserted in the _datas_ table, sending a push to every installations in the _Installations_ table.

Create the _Installations_ table from Parse's template classes.
You'll need to setup Parse with the API key for Google Cloud Messenging to send push notifications in **Push** settings. Also activate Push and REST push.  

### Configure Parse
In the App Settings of your project, fill a subdomain name in the **Email & Hosting** tab.
Copy your Parse APP ID and API key from **Security & Keys** in your manifest.json  
Use [Parse's CLI](https://parse.com/docs/cloudcode/guide) to deploy your project on the cloud. Everything in the _/public_ folder will be instantly live on the website.

### Register for push!
Finally you can go to the URL of your Parse project (eg: subdomain.parseapp.com), to register your current device for notifications.  
The script in _/public_ will run a javascript **service worker** sending your device token to Parse's _Installations_ class.
