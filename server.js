const Discord = require('discord.js');
const client = new Discord.Client();
var sys = require('sys')
var exec = require('child_process').exec;
var fs = require('fs');

require('dotenv').config();

// 'client.on('message')' commands are triggered when the
// specified message is read in a text channel that the bot is in.

client.on('ready', () => {
    console.log(`Logged in as ${client.user.tag}!`);
});

client.on('message', msg => {
    if (msg.content === 'ping') {
        msg.reply('Pong!');
    }
});
client.on('message', msg => {
    //console.log(msg.author);
    if (!msg.author.bot && msg.content.match(/^rholang/i)) {
        let dir;
        console.log(msg.content);
        if (msg.content.match(/^rholang:/i)) {
            console.log(msg.content);
            let rholang = msg.content.substring(8).replace(/[\r\n\t]/g, " ");
            var rholang2 = rholang.replace(/'/g, "'\"'\"'");
            console.log(rholang2);
//	    var sed='sed -n "/Error: /p;s/.*@{\"result/ @{\"result/;s/ |.*//;/^ @/p"';
            dir = exec('set -x 2>&1;echo '+"'" + rholang2 + "'"+'  2>&1 >last.rho;echo '+"'"+'@"result" ! ('+ rholang2 + ")'| tee repl.in|rnode repl|tee repl.out",
            //dir = exec('echo '+"'" + rholang2 + "'"+'  2>&1 >last.rho;echo '+"'"+'@"result" ! ('+ rholang2 + ")'| tee repl.in"+'|echo sed -n "/Error: /p;s/.*@{\"result/ @{\"result/;s/ |.*//;/^ @/p"',
             //dir = exec('echo '+"'" + rholang2 + "'"+'  2>&1 >last.rho;echo '+"'"+'@"result" ! (' + rholang2 + ')|rnode repl 2>&1|tee repl.out|sed -n "/Error: /p;s/.*@{\"result/ @{\"result/;s/ |.*//;/^ @/p"',
                function(err, stdout, stderr) {
                    if (err) {
                        // should have err.code here?
                    }
                    //console.log(stdout);
		    var result = stdout.replace(/[\r\n\t]/g, " ");
		    result = result.replace(/.*result/," @{\"result");
		    result = result.replace(/ \|.*/,"");
                    console.log(result);
                    msg.reply(result + "\n (See the log: https://rhobot.net/rnode-log/");
                }
            );
        } else {
	    var n = Math.floor(Math.random() * 3);
            var answers = ["RHOlang is the language of RChain. Run a rholang program by typing:\nrholang: <your program>",
		"RHOlang is the language of the mobile asyncronous communicating process calculus.",
		"See the rholang tutorial at https://developer.rchain.coop/tutorial"];
            msg.reply(answers[n]);
	}
    }
    if (msg.content.match(/^deploy:/i)) {
        console.log(msg.content);
        dir = exec(" rnode deploy --from \"0x1\" --phlo-limit 0 --phlo-price 0 --nonce 0 last.rho", function(err, stdout, stderr) {
            if (err) {
                // should have err.code here?
            }
            console.log(stdout);
            msg.reply(stdout + " (See the log: https://rhobot.net/rnode-log/");
        });
    }
    if (msg.content.match(/^propose:/i)) {
        console.log(msg.content);
        dir = exec("rnode propose", function(err, stdout, stderr) {
            if (err) {
                // should have err.code here?
            }
            console.log(stdout);
            msg.reply(stdout + " (See the log: https://rhobot.net/rnode-log/");
        });
    }
});
client.on('guildMemberAdd', member => {
    // Send the message to a designated channel on a server:
    const channel = member.guild.channels.find(ch => ch.name === 'general');
    // Do nothing if the channel wasn't found on this server
    if (!channel) return;
    // Send the message, mentioning the member
    channel.send(`Welcome to the server, ${member}`);
});
client.login(process.env.TOKEN);

//fs.writeFile("last.rho", rholang, function(err) {
//    if(err) {
 //       return console.log(err);
 //   }

//    console.log("The file was saved!");
// });
