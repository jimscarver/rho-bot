const Discord = require('discord.js');
const client = new Discord.Client();
const grpc = require('grpc');
var sys = require('sys')
var exec = require('child_process').exec;
var fs = require('fs');
var matchRecursive = require("match-recursive");

require('dotenv').config();
const {RNode, RHOCore, logged} = require('rchain-api');

let myNode;

// 'client.on('message')' commands are triggered when the
// specified message is read in a text channel that the bot is in.

client.on('ready', () => {
    myNode = RNode(grpc, {host: "rhobot.net", port: 40401});
    console.log(`Logged in as{host: "rhobot.net", port: 40401} ${client.user.tag}!`);
});

client.on('message', msg => {
    if (msg.content === 'ping') {
        console.log(msg);
        msg.reply('Pong!');
    }
});
client.on('message', msg => {
    //console.log(msg.author);
        console.log(msg.content);
    let content = msg.content;
    if (content.match(/^```rholang.*/)) {
console.log("matched!");
       content = "rholang:"+content.substring(10,content.length-4)
    }
    if (!msg.author.bot && content.match(/^rholang/i)) {
        let dir;
	const randid = Math.random().toString().substring(2);
        console.log(msg.content);
	var auxmsg = "";
        if (content.match(/^rholang:/i)) {
            console.log(content);
            let rholang = '@"'+ msg.channel.name+randid+'"!('+content.substring(8)+')';
            console.log(rholang);
            var deployData = {term: rholang,
	                      timestamp: new Date().valueOf()};
            myNode.doDeploy(deployData).then(result => {
              if(!result.success) {
	     //   msg.reply("Deploy error: "+result.message);
		console.log(result);
	        throw(result);
              }
	      return myNode.createBlock();
	    }).then(result => {
	      if(!result.success) {
	    //    msg.reply("createBlock error: "+result.message);
	        throw(result);
              }
	      auxmsg = result.message;
	      console.log(result.message);
              console.log("msg.channel.name is: ", msg.channel.name+randid);
              return myNode.listenForDataAtName(msg.channel.name+randid)
	    }).then(blockResults => {
              console.log("blocks received: " + blockResults.length)
              // TODO: If we got no blocks back, fail gracefully.
              var lastBlock = blockResults.pop()
              console.log("block results are:");
              console.log(JSON.stringify(blockResults));
              console.log("number of blocks received: ", blockResults.length);
              var lastDatum = lastBlock.postBlockData.slice(-1).pop()
              if (lastDatum) {
                msg.reply(auxmsg+"\n"+RHOCore.toRholang(lastDatum))
              }
              else {
                msg.reply(`No data on channel ${msg.channel.name+randid}`);
              }
            }).catch(oops => { console.log(oops); 
		/* if (!auxmsg) */ msg.reply(auxmsg+"\n"+oops.message + " (See the log: https://rhobot.net/rnode-log)"); 
	    });

        } else {
	    var n = Math.floor(Math.random() * 3);
            var answers = ["RHOlang is the language of RChain. Run a rholang program by typing:\nrholang: <your program>",
		"RHOlang is the language of the mobile asyncronous communicating process calculus.",
		"See the rholang tutorial at https://developer.rchain.coop/tutorial"];
            msg.reply(answers[n]);
	}
    }
    if (msg.content.match(/^eval:/i)) {
        const author = msg.author.username;
        console.log(msg.content);
        let rholang =  msg.content.substring(5);
	fs.writeFile("/tmp/"+author+".rho", rholang, function(err) {
         if(err) {
          return console.log(err);
         }

         console.log("The file was saved!");
         dir = exec(" rnode eval /tmp/"+author+".rho", function(err, stdout, stderr) {
           if ( ! err ) {
		msg.reply(stdout.substring(0,500)+"\n...\n"+stdout.substring(stdout.length-500));
           } else {
                msg.reply(stderr);
           }
        });
      });
    }
    if (msg.content.match(/^deploy:/i)) {
        const author = msg.author.username;
        console.log(msg.content);
        let rholang = msg.content.substring(8);
        dir = exec(" rnode deploy --from \"0x1\" --phlo-limit 0 --phlo-price 0 --nonce 0 /tmp/"+author+".rho", 
          function(err, stdout, stderr) {
            if (err) {
                msg.reply(stderr);
                // should have err.code here?
            }
            console.log(stdout);
            msg.reply(stdout + " (See the log: https://rhobot.net/rnode-log)");
        });
    }
    if (msg.content.match(/^propose:/i)) {
        console.log(msg.content);
        dir = exec("rnode propose", function(err, stdout, stderr) {
            if (err) {
                // should have err.code here?
            }
            console.log(stdout);
            msg.reply(stdout + " (See the log: https://rhobot.net/rnode-log)");
        });
    }
});
client.on('guildMemberAdd', member => {
    // Send the message to a designated channel on a server:
    const channel = member.guild.channels.find(ch => ch.name === 'general');
    // Do nothing if the channel wasn't found on this server
    if (!channel) return;
       let messages = [
	"Hello!",
	"It’s nice to meet you.",
	"It’s a pleasure to meet you.",
	"Hi!" ,
	"How are things ?",
	"What’s new?",
	"It’s good to see you." ,
	"G’day!" ,
	"Howdy!" ,
	"Welcome to the server"
      ];
      var  message = messages[Math.floor(Math.random() * messages.length()-1)];

    // Send the message, mentioning the member
    channel.send(`${message}. ${member}. Welocome to the cooperation laboratory.`);
});
client.login(process.env.TOKEN);

//fs.writeFile(""+author+".rho", rholang, function(err) {
//    if(err) {
 //       return console.log(err);
 //   }

//    console.log("The file was saved!");
// });
