#MacRhoLang @RHO-bot ([google doc](https://docs.google.com/document/d/1mTUQwWV9zW5INaJekf-hrZoukWh8Gt8ggFwIxREp1vk/edit#heading=h.rcgqrt7cbij8))

Chatbots have become a leading means of managing distributed systems. @RHO-bot in the CoLab discord chat allows us to interface to a rchain node (rhobot.net,soon to be on testnet) in a language we cooperatively define. We define macros of parameterized rholang snippets allowing them to be used like user defined commands, functions or variables generating code to be run on rchain. It employs discord identities to create toy private and public keys enabling demonstration of contracts running on rchain with simple command interface. E.g. testing voting contracts and getting community sentiment on discord without people needing to learn rholang:

  

newBallot: "colors",  Set("red", "green", "blue")

vote: “colors”, “green”

voteresults: “colors”

  

@RHO-bot is a work in progress being cooperatively constructed in [the colab](https://blog.rchain.coop/blog/2018/09/24/introduction-to-rchain-colab/) currently employing the [cpp macro preprocessor](http://gcc.gnu.org/onlinedocs/cpp/) and [clang-format](https://www.kernel.org/doc/html/v4.17/process/clang-format.html) formatter for [rholang](https://developer.rchain.coop/) on rchain with a discord.js chatbot.  @RHO-bot is running on rhobot.net. The [source code](https://github.com/jimscarver/rho-bot) is in github.

### Built in commands

eval: &lt;macrholang code&gt; 

Expands macros, if any, and then executes the resulting rholang code and posts result from the log in the channel. Macros are defined if the output includes [“#define $name”,aValue]

  

echo: &lt;macrholang code&gt;

Expands global macros (excluding personal macros such as private key) and posts the result to the channel.

  

define: $&lt;name&gt; &lt;macrholang code&gt;

define: $&lt;name&gt;($arg, …) &lt;macrholang code including $args substitution&gt;

e.g.

define: $Ballot `rho:id:3qfh1fy7jwfcai7ceyorux4a18hzcn83n9xb6dramjf5gs7cw8fynf`

define: $print($expression) new stdout(`rho:io:stdout`) in { stdout!($expression) }

  
  

find: &lt;optional search regular expression&gt;

  

deploy: 

No arguments, deploys the tuple space of the last thing you evaluated on rchain.

  

propose: 

No arguments. Proposes a block consisting of deploys from any user.

### Private data data locker and user mailbox macro commands

makemylocker:

mylockerGet: *stdout

mylockerGet: “key”, *stdout

mylockerStore: “key”, “value”

mylockerUpdate: “key”, “value”

  

### Mailbox macro commands

We communicate with other members through their write only public mailbox (inbox.rho). We can send messages that may include capabilities granting access rights such as the right to vote on a ballot.

  

send: “username”, “type”, “subtype”, data….

receive: “type”, subtype”, *stdout

peek: *stdout

peek: “type”, *stdout

peek: “type”, “subtype”, *stdout

  

### Directories

$newDirectory($name)

$addUser($username,$directory)

  

### Voting

newBallot: "colors",  Set("red", "green", "blue")

allowtovote: "jimscarver","colors"

allowgrouptovote: “colab”, “colors”

vote: "colors", "red"

voteresults: "colors"

  
  

### Example using macros in Rholang code to share data and capabilities

Discord markup renders rholang wrong such that characters are missing and copied programs are broken unless in a code block started and ended with ```

We get the advantage of somewhat useful syntax highlighting if we start with ```scala

  

```scala

define: $share($username, $type, $subtype)

new return in (

  $peek($type, $subtype, *return) |

  for ( list &lt;- return) {

    $send($username, [$type, $subtype] ++ *list)

  }

}

```

Then I can invoke the macro as a command to share my colab directory admin capabilities:

  

share: "Emmanuel_mebyz", "directory", "colab"

  
  

### Your rchain application

Build a Discord Bot
=========================

This is a quick and easy template for building your own Discord bot, this project relies on **[Discord.js](https://github.com/hydrabolt/discord.js/)**, if you want to extend this bot, I suggest you look at **[the official Discord API documentation](https://discordapp.com/developers/docs/intro)** and **the Discord.js README**.

Before building your own bot, I highly advise checking if the bot you want to build already exists, the common places to find Discord bots are:
* [https://discordbots.org/](https://discordbots.org/)
* [https://bots.discord.pw/](https://bots.discord.pw/)
  
# Getting Started

**Step One**  
You need to  [create a new Discord Application](https://discordapp.com/developers/applications/me) in the developer console.

![Step One Screenshot](https://i.gyazo.com/216dec70040ae14a6ee121dce67e3f82.png)

**Step Two**  
You need to create a Bot User for your application.
![Step Two Screenshot](https://i.gyazo.com/47f8e671eccfb09ac22c6630417ccdee.png)

**Step Three**  
You need to add your Bot User's token as a parameter to `client.login()`, these will be used by the code to act on behalf of your registered application's bot.

If you want to avoid unneeded complexity, untick **Require OAuth2 Code Grant**.

![Step Three Screenshot](https://i.gyazo.com/d5f194037b8823f42c7442e3959c8f32.png)

**Step Four**  
**Generate your bot's OAuth2 URL** and it can then be invited to any channel with just a click of a link.

![Step Four Screenshot](https://i.gyazo.com/5cabc8aaa969f4339ddab48f8f2234f4.png)

**Step Five**  
Remix this project, look through the code in `server.js` and add the custom commands you wish your bot to have and **VIOLA**, you're now the proud owner of your own Discord bot 🎉🎉🎉🎉

**Go build something cool.**  
  
  
# More about Discord
  
**Discord** is all-in-one voice and text chat for gamers that's free, secure, and works on both your desktop and phone. Stop paying for TeamSpeak servers and hassling with Skype. Simplify your life.

# More about Glitch

**Glitch** is the friendly community where you'll build the app of your dreams. Glitch lets you instantly create, remix, edit, and host an app, bot or site, and you can invite collaborators or helpers to simultaneously edit code with you.

Find out more [about Glitch](https://glitch.com/about).


# This project was built by [Greg Worrall](https://twitter.com/gregjwww) 🎉
-------------------

\\( ゜o゜)ノ Follow me on [Twitter](https://twitter.com/gregjwww) or [GitHub](https://github.com/gregjw).
 
