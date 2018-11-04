#Telegram-cli Notifications for i3wm

My first experience with a Linux desktop environment was gnome. It was fine, I guess, but I spent most of my time in the terminal so I never really learned where anythign was or how to do anything using gnome. I just had it.<br/>
Later on I got introduced to i3wm(i3 for short). Love at first sight.<br/>
I use vim, I use tmux, you'd think I already had enough tabs, panes, windows, sessions. You'd be most certainly wrong. After i3 I realised I still needed more.<br/>
I use telegram because friends, family, life and ... .<br/>
It's annoying to have to pay attention to my cellphone(I know I'm a horrible person, no need to point that out). Having i3 notifications is just more convinient.<br/>
So the next obvious step was to get just that. Telegram-cli has both Python and Lua APIs. I used the Lua API. I have no reason whatsoever to back my decision up other than I just felt like doing it in Lua.<br/>
TL;DR<br/>
A Telegram-cli Lua script has to implement a couple of hook functions(the API gives us hooks and callbacks):<br/>
* `on_binlog_replay_end(ok_cb, extra)` this one runs when replay of old events has finished.<br/>
* `on_get_difference_end(ok_cb, extra)` runs after the first call to `get_difference`.<br/>
* `on_our_id(our_id)` using this, you can check the id of the user you are logged in as.<br/>
* `on_msg_receive(msg)` it's called when we receive a new message.<br/>
* `on_user_update(user, what_changed)` is called when there is an update on the user's info.<br/>
* `on_chat_update(user, what_changed)` is called when there is an update on a chat info.<br>
* `on_secret_chat_update(user, what_changed)` is called when there is updated info for a secret chat.<br/>
You can read the Lua API documentation [here](https://github.com/vysheng/tg/blob/master/README-LUA). The documentation could use some help. If you can't find anything there, either look at the source code for the Lua API [here](https://github.com/vysheng/tg/blob/master/lua-tg.c) or just look at the Python API doc [here](https://github.com/vysheng/tg/blob/master/README-PY.md).<br/>
We are only going to implement `on_binlog_replay_end` and `on_msg_receive`.<br/>
Implementing those two hooks would give us a way to know when we receive a new maessage and if for whatever reason we missed that, how many unread messages we have.<br/>
The personal one version of the script I use, sends notifications to a server. Then, a client can quesry the server for updates. The client is what i3 calls for a block update.<br/>
