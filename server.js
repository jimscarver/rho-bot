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

const Tail = require('tail').Tail;
 
const tail = new Tail("/home/rchain/rnode.log");
 
var currentMessage;
tail.on("line", function(data) {
  if ( data.match(/^Unforgeable|^bundle|^@|^Nil$|^\(|^"|^[0-9.][0-9.]*$|^\[|^\{|^\(|^`|^Syntax Error/)) {
     data =  data.replace(/Unforgeable\((........)[^}]*\)/g,'<$1..>');
     console.log("log: "+data);
     if ( currentMessage ) {
	currentMessage.reply(data);
       if ( data.match(/\[\"#define /) ) {
	  var matches = data.match(/(#define[^"]*)", (.*)\]/);
	  //currentMessage.reply(matches[1]+' '+matches[2]);
	  exec( "echo '"+matches[1]+' '+matches[2]+"' >>global.h");
	  //console.log("define:"+data.match(/^(#define[^"]*)", (.*)\]/));
       }
     }
  }
});
tail.unwatch()
 
tail.on("error", function(error) {
  console.log('ERROR: ', error);
});

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
  currentMessage = msg;
  console.log(msg.author.username+": "+msg.content);
  let content = msg.content;
  var repeat = true;
  var retry = 0;
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
    if (!msg.author.bot && content.match(/^rholang/i)) {
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
         let rholang = "" +
           "echo '"+content.substring(5).replace("'","'\"'\"'")+"'|"+
           "cat global.h end.h - |cpp 2>/tmp/cpp-error|tee cpp-out|"+
           "sed -n '/^#/d;/^_end_$/,$p'|tail +2|sed 's/\"~~\" */\\\n/g'";
            
/*
           "sed -n '/^#/d;/^_end_$/,$p'|tail +2|clang-format|"+
           "perl -0777 -pe 's/\\n[ \\t]*-/-/igs;s/<\\w-/<- /g;s/=\\w>/=> /g;s/\\n[ \\t]*:/:/igs;"+
           "s/ :/:/g;s/rho: ([a-zA-Z0-9]*:) /rho:\\1/g;s/ :/:/g;s/ !/!/g;"+
             "s/ \\+\\+/ \+\+ /g;s/ \\% \\% \"([^\"]*)\"/\\1/g;s/ < -/ <- /g;"+
             "s/ \\% \\% /%%/g'|tee lastecho";
*/
         console.log(rholang)
         exec(rholang,
           function(err, stdout, stderr) {
           if ( ! err ) {
	     let rho = stdout;
	     if ( ! rho.match(/\n./) ) { // add new lines to code without any
	       rho = rho.replace(/\{/g,"{\n")
	       rho = rho.replace(/\} */g,"\n}")
	       rho = rho.replace(/\| */g,"|\n")
	     }
	     let rholang2 = ""; let indent = ""; let last = "";
	     for (const c of rho) { // cpp removes whitespace so add back indenting
	       if ( c == "}" ) indent = indent.substring(2);
               if (last.match(/\n/)) rholang2 += indent;
	       if ( c == '{' ) indent += "  ";
	       //else if ( c.match(/\n/)) rholang2 += indent;
	       rholang2 += c;
               last = c;
             }
	     rholang2 += last;
             msg.reply("```scala\n"+rholang2+"\n```");
           } else {
                msg.reply("error:"+stderr);
           }
           })
    }


    else if (content.match(/^eval:/i)) {
        tail.watch(); // turn on reporting log output as msg.reply
        const author = msg.author.username;
         let rholang = '#define $myprivkey "'+msg.author.id.substring(10)+"\"\n"+
	   '#define $mypubkey "dead'+msg.author.id.substring(10)+"\"\n"+
	   '#define $myusername "'+msg.author.username.replace(" ","_")+"\"\n"+
	   content.substring(5)+"\n";
        console.log(rholang);
        //let rholang =  content.substring(5);
	fs.writeFile("/tmp/"+author+".rhox", rholang, function(err) {
         if(err) {
          return console.log(err);
         }

         console.log("The file was saved!");
         let bash = "cat global.h '/tmp/"+author+".rhox' |cpp 2>cpperrors|"+
           "sed 's/\\%\\%\"\\([^\"]*\\)\"/\\1/g;s/\"~~\"/ /g'|"+
           "cat global.h end.h -|cpp 2>cpperrors2|sed -n '/^#/d;/^_end_$/,$p'|tee lasteval|"+
           "tail +2 >'/tmp/"+author+".rho';"+
           "rnode eval '/tmp/"+author+".rho'|"+
           "sed '1,3d;s/Deployment cost: //;/^Storage Contents:/{x;q}'";
         console.log(bash);
         dir = exec(bash,
         //dir = exec(" rnode eval /tmp/"+author+".rho|sed '2,3d;/^Storage Contents:/{x;q}'", 
           function(err, stdout, stderr) {
           if ( ! err ) {
	//	msg.reply(stdout.substring(0,500)+"\n...\n"+stdout.substring(stdout.length-500));
             msg.reply(stdout.substring(0,stdout.length-0));
           } else {
                msg.reply(stderr);
           }
           tail.unwatch();
        });
      });
    }
    else if (content.match(/^deploy:/i)) {
        const author = msg.author.username;
        let rholang = content.substring(8);
        dir = exec(" rnode deploy --from \"0x1\" --phlo-limit 100000000 --phlo-price 1 --nonce 0 /tmp/"+author+".rho", 
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
    else if (content.match(/^[a-zA-Z0-9_-]*:(\s|$)/) && !msg.author.bot) {
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
