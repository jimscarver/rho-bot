const Discord = require('discord.js');
const client = new Discord.Client();
const grpc = require('grpc');
var sys = require('util')
var exec = require('child_process').exec;
var fs = require('fs');
//var matchRecursive = require("match-recursive");

require('dotenv').config();
const {RNode, RHOCore, logged} = require('rchain-api');

let myNode;

const Tail = require('tail').Tail;
 
const tail = new Tail("/home/rchain/rnode.log");
 
var debug = 3;
var currentMessage;
var lastMessage;
tail.on("line", function(data) {
  let hide = 0;
  if ( data.match(/^Unforgeable|^bundle|^@|^Nil$|^\(|^"|^[0-9.][0-9.]*$|^\[|^\{|^\(|^`|^Syntax Error/)) { // if stdout data
     data =  data.replace(/Unforgeable\(0x([0-9a-z]{6})[^)]*\)/g,'<$1..>');
     console.log("log: "+data);
     if ( data.match(/\["#define \$/) ) {
       hide = 0;
       let matches = data.match(/(#define[^"]*)", (.*)\]/);
       exec( "echo '"+matches[1]+' '+matches[2]+"' >>global.h");
     } 
     else if (  data.match(/^\["event:",/)) {  
       let match = data.match(/^\["event:", "at:", "([^"]*)", "([0-9]*)"\]/);
       if ( match ) {
         console.log("echo ./trigger.sh "+match[2]+"|at "+match[1]+" >/tmp/at.out 2>&1");
         exec("echo ./trigger.sh "+match[2]+"|at "+match[1]+" >/tmp/at.out",
	         function(err, stdout, stderr) {
           if ( ! err ) {
             debug > 0 ? currentMessage.channel.send(stderr):0;
           }
         });
       } else {
         match = data.match(/^\["event:", "trigger:", "([0-9]*)"\]/);
         exec("echo ./trigger.sh "+match[2]+" >/tmp/trigger.out",
	         function(err, stdout, stderr) {
           if ( ! err ) {
             currentMessage.channel.send("error:"+stderr);
           }
         });
       }
     }
     hide = data.match(/^"locker.get\(...\)"|^\(?\["debug:|^Error: .*: Unrecognized interpreter errorError: coop.rchain.rholang.interpreter.errors$LexerError: Illegal Character <$>/) ? 2:hide;
     if ( currentMessage ) {
       hide<=debug ? currentMessage.channel.send(data):0;
     } else if ( lastMessage ) { //TODO this sould report to a user by DB matching reportto:, useris
       hide<=debug ? lastMessage.channel.send(data):0;
       currentMessage = lastMessage
     }
   }
});
//tail.unwatch()
 
tail.on("error", function(error) {
  console.log('ERROR: ', error);
});

function rhoformat(rho) {
     if ( ! rho.match(/\n./) ) { // add new lines to code without any
       rho = rho.replace(/^  */,"")
       rho = rho.replace(/\{\s/g,"{\n")
       rho = rho.replace(/\s\} */g,"\n}")
       rho = rho.replace(/\| */g,"|\n")
     }
     let rholang2 = ""; let indent = ""; let last = "";
     for (const c of rho) { // cpp removes whitespace so add back indenting
       if ( c == "}" ) indent = indent.substring(2);
       if (last.match(/\n/)) rholang2 += indent;
       if ( c == '{' ) indent += "  ";
       rholang2 += c;
              last = c;
     }
     return rholang2 += last;
}
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
  console.log("message: "+msg.channel.name+": "+msg.author.username+": "+new Date().toISOString()+" "+msg.content);
  let content = msg.content;
  let repeat = true;
  let retry = 0;
  while (repeat && retry < 2) {
    repeat = false;
    if (content.match(/^```rholang.*/)) {
       content = "rholang:"+content.substring(10,content.length-4)
    }
    if (content.match(/^```eval.*/)) {
       content = "eval:"+content.substring(7,content.length-4)
    }
    if (content.match(/^```.*/)) {
       content = content.substring(/\r|\n/.exec(content).index+1,content.length-4)
    }
    if (!msg.author.bot && content.match(/^rholang/i)) { // not working
        let dir;
	const randid = Math.random().toString().substring(2);
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
            var answers = ["RHOlang is the language of RChain. Run a rholang program by typing:\neval: <your program>",
		"RHOlang is the language of the mobile asyncronous communicating process calculus.",
		"See the rholang tutorial at https://developer.rchain.coop/tutorial"];
            msg.reply(answers[n]);
	}
    }
    else if (content.match(/^debug: /)) {
      debug = parseInt(content.substring(content.match(/ /).index));
      debug ? msg.channel.send("Debug level "+debug+" on."): msg.channel.send("Debugging off.");
    }
    else if (content.match(/^define:/)) {
      if (! content.match(/^define:\s*\$/) ) {
        msg.reply("error: start macro names with a dollar sign $");
      } else {
        let command = "echo '#define "+content.substring(7).replace(/(?:\r\n|\r|\n)/, ' ').replace(/(?:\r\n|\r|\n)/g, '"~~"').replace("'","'\"'\"'")+"' >>global.h";
        exec(command);
        msg.reply("defined "+content.substring(7,(content+"\n").indexOf("\n")));
      }
    }

    else if (content.match(/^find:/i)) {
         let search = content.substring(6);
         console.log("search "+search);
         if (search.match(/^$/))
           search = ".*";
         let command = "" +
           "sed -n 's/^#define  *//;s/, /,/g;s/( /(/g;s/ )/)/g;s/ .*//;"+
           "/locker_nonce/d;/"+search+"/Ip' global.h|sort -u";
         console.log(command)
         exec(command,
           function(err, stdout, stderr) {
           if ( ! err ) {
             msg.reply(":\n"+stdout.substring(0,1900));
           } else {
                msg.reply("error:"+stderr);
           }
           })
    }
    else if (content.match(/^echo:/i)) {
      // if a command colon, make is a function call
      if (content.match(/^echo:  *[a-zA-Z0-9_-]*:(\s|$)/) && !msg.author.bot) {
          let match = content.match(/^echo:  *([^:]*): *(.*)/);
          content = "echo: $"+match[1]+'('+match[2]+")";
      }
      console.log(content);
         let rholang = "" +
           "echo '"+content.substring(5).replace("'","'\"'\"'")+"'|"+
           "cat global.h end.h - |cpp 2>/tmp/cpp-error|tee cpp-out|"+
           "sed -n '/^#/d;/^_end_$/,$p'|tail +2|sed 's/\"~~\" */\\\n/g;"+
	     "/.\\{1900,\\}/s/\\(.{1900\\}\\).*/\\1... too long for discord/'"; // TODO doesnt work
         //console.log(rholang)
         exec(rholang,
           function(err, stdout, stderr) {
           if ( ! err ) {
	     let rholang2 = rhoformat(stdout);
             msg.reply("```scala\n"+rholang2+"\n```");
           } else {
                msg.reply("error:"+stderr);
           }
           })
    }
    else if (content.match(/^show:/i)) {
      console.log(content);
         let rholang = "" +
         "grep -i '^#define[ \\$]*"+content.substring(6).replace("'","'\"'\"'")+"[ \\(]' global.h|"+
           "tail -1|sed 's/\"~~\" */\\n/g'";
	 console.log(rholang);
         exec(rholang,
           function(err, stdout, stderr) {
           if ( ! err ) {
	     let rho = stdout.substring(7);
	     console.log(rho);
	     rho= rhoformat(rho.replace(/\)  *\(/,") (").replace(/^  */," "));
	     console.log(rho);
	     if ( ! rho.match(/\n./) ) { // add new lines to code without any
	       rho = rho.replace(/([^ ])  /g,"$1\n  ")
	     }
             msg.reply("```scala\n-> define: "+rho+"\n```");
           } else {
                msg.reply("error:"+stderr);
           }
           })
    }


    else if (content.match(/^eval:\s\s*/i)) {
        currentMessage = msg;
        tail.watch(); // turn on reporting log output as msg.reply
        const author = msg.author.username;
         let rholang = '#define $myprivkey "'+msg.author.id.substring(10)+"\"\n"+
	   '#define $mypubkey "dead'+msg.author.id.substring(10)+"\"\n"+
	   '#define $myusername "'+msg.author.username.replace(" ","_")+"\"\n"+
	   '#define $_messageid "'+msg.id+"\"\n"+
	   '#define $_authorid "'+msg.author.id+"\"\n"+
	   content.substring(5)+"\n";
        console.log(rholang);
        //let rholang =  content.substring(5);
	      fs.writeFile("/tmp/"+author+".rhox", rholang, function(err) {
         if(err) {
          return console.log(err);
         }

         console.log("The file was saved!");
         let bash = "cat global.h '/tmp/"+author+".rhox' |cpp 2>cpperrors|"+
           "sed 's/\\%\\%\"\\([^\"]*\\)\"/\\1/g;s/\"~~\"/\\n/g'|"+
           "cat global.h end.h -|cpp 2>cpperrors2|sed -n '/^#/d;/^_end_$/,$p'|tee lasteval|"+
           "tail +2 >'/tmp/"+author+".rho';"+
           "rnode eval '/tmp/"+author+".rho' 2>&1|"+
           "sed '1,3d;/^Storage Contents:/{x;q}'";
         console.log(bash);
         dir = exec(bash,
         //dir = exec(" rnode eval /tmp/"+author+".rho|sed '2,3d;/^Storage Contents:/{x;q}'", 
           function(err, stdout, stderr) {
           if ( ! err ) {
	     if ( stdout.match(/Illegal Character \<\$\> /) ) {
	       fs.readFile("/tmp/"+author+".rho", function(err, rholang) {
                 if(err) {
                  return console.log(err);
                 } else {
		   msg.reply((""+rholang).match(/(\$[a-zA-Z_0-9]*)/)[1]+" undefined or wrong number of args.");
		 }
	       })
	     }
             else if ( stdout.match(/^Deployment cost: /) ) {
               msg.reply(stdout);
               currentMessage=null;
             }
             else if ( stdout.match(/^Error: /) ) {
	       msg.reply(stdout.match(/..........: (.*)/)[1]);
	     }
             if ( debug > 0 ) msg.reply(stdout.substring(0,stdout.length-0));
           } else {
             msg.reply(stderr);
           }
           //tail.unwatch();
        });
      });
    }
    else if (content.match(/^deploy:/i)) {
        const author = msg.author.username;
        let rholang = content.substring(8);
        dir = exec(" rnode deploy --phlo-limit 10000000000 --phlo-price 1 /tmp/"+author+".rho", 
          function(err, stdout, stderr) {
            if (err) {
                msg.reply(stderr);
                // should have err.code here?
            }
            console.log(stdout);
            msg.reply(stdout + " (See the log: https://rhobot.net/rnode-log)");
        });
    }
    else if (content.match(/^propose:/i)) {
        console.log(content);
        dir = exec("rnode propose", function(err, stdout, stderr) {
            if (err) {
                // should have err.code here?
            }
            console.log(stdout);
            msg.reply(stdout + " (See the log: https://rhobot.net/rnode-log)");
        });
    }
    else if (content.match(/^who am i\?/i)) {
        //var deployData = {term: ,
	//                  timestamp: new Date().valueOf()};
        msg.reply(`HELLO ${msg.author.username} id = ${msg.author.id}`);
    }
    else if (content.match(/^[a-z][a-zA-Z0-9_-]*:(\s|$)/) && !msg.author.bot) {
      const matches = content.match(/(^[^:]*): (.*)/);
      if (matches) {
        content = "eval: $"+matches[1]+'('+matches[2]+')';
      } else {
        content = "eval: $"+content.match(/(^[^:]*)/)[1];
      }
      repeat = true; retry = retry + 1;
    }
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
	"Welcome! How might we cooperat",
	"What’s new?",
	"It’s good to see you." ,
	"G’day!" ,
	"Howdy!" ,
	"Welcome to the server"
      ];
      var  message = messages[Math.floor(Math.random() * messages.length)];
console.log(`New member ${member} ${message}`);
    // Send the message, mentioning the member
    channel.send(`${message}. ${member}. Welcome to the cooperation laboratory.`);
});

client.login(process.env.TOKEN);

//fs.writeFile(""+author+".rho", rholang, function(err) {
//    if(err) {
 //       return console.log(err);
 //   }

//    console.log("The file was saved!");
// });
