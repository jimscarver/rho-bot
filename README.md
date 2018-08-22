
Sample dialog from this bot. Modify to make your own bot.

jimscarverToday at 2:46 PM
rholang!
RHO-botBOTToday at 2:46 PM
@jimscarver, RHOlang is the language of RChain. Run a rholang program by typing:
rholang: <your program>
jimscarverToday at 2:47 PM
rholang: 
new HelloWorld in {
  contract HelloWorld(return) = {
    return!("Hello, World!")
  } |
  new myChannel in {
    HelloWorld!(*myChannel)
  }
}
RHO-botBOTToday at 2:47 PM
@jimscarver,  @{"result"}!(new x0 in { for( x1 <= x0 ) { x1!("Hello, World!") }
 (See the log: https://rhobot.net/rnode-log/
jimscarverToday at 2:47 PM
deploy:
RHO-botBOTToday at 2:47 PM
@jimscarver, 18:47:54.604 [main] INFO  c.r.n.configuration.Configuration$ - Starting with profile default
Response: Success!
 (See the log: https://rhobot.net/rnode-log/
jimscarverToday at 2:48 PM
propose:
RHO-botBOTToday at 2:48 PM
@jimscarver, 18:48:25.814 [main] INFO  c.r.n.configuration.Configuration$ - Starting with profile default
Response: Success! Block 6819ab4e2b... created and added.
 (See the log: https://rhobot.net/rnode-log/
jimscarverToday at 2:52 PM
rholang developer wannabees are welcome here.
RHO-botBOTToday at 2:52 PM
@jimscarver, RHOlang is the language of the mobile asyncronous communicating process calculus.
jimscarverToday at 2:56 PM
rholang can be discussed here.
RHO-botBOTToday at 2:56 PM
@jimscarver, See the rholang tutorial at https://developer.rchain.coop/tutorial


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
 
