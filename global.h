#define  $stdnames lookup(`rho:registry:lookup`), insertArbitrary(`rho:registry:insertArbitrary`), stdout(`rho:io:stdout`), ack, return
#define  $stdout stdout(`rho:io:stdout`)
#define  $define($name,$value) stdout!(["#define " ++ $name, $value])
#define  $trace trace(`rho:io:stderr`)
#define  $insertArbitrary insertArbitrary(`rho:registry:insertArbitrary`)
#define  $lookup lookup(`rho:registry:lookup`)
#define   $mylockerStore($key,$value) new $stdout, $lookup, nonce , ret in {   lookup!($locker_##$myusername, *ret) |   for (locker <- ret) {      locker!("update", $myprivkey.hexToBytes(),$locker_nonce_##$myusername, {$key: $value}, *stdout, *ret)     for (_ <- ret) {        stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_##$myusername + 1])      }   } }
#define  $test_value "xyzzy"
#define   $mylockerGet($ret) new $stdout, $lookup, nonce , lockerCh in {   lookup!($locker_%%$myusername, *lockerCh) |   for (locker <- lockerCh) {       locker!("get", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername,  *stdout, *$ret) |      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])    } }
#define   $mylockerUpdate($key,$value) new $stdout, $lookup, nonce , lockerCh, ret in {   lookup!($locker_%%$myusername, *lockerCh) |   for (locker <- lockerCh) {       locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername, { $key: $value},  *stdout, *ret) |      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])    } }
#define  $add($x,$y)  ($x + $y)
#define  $newinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       lookup!($locker_%%$myusername, *ret) |       for (locker <- ret) {         stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 1]) |         locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |         for (_ <- ret) {           insertArbitrary!(*send,*ret)|           for (uri <- ret) {             stdout!(["#define $inbox_" ++ $myusername, *uri])           }         }       }     }   } }
#define  $hi new $stdnames, say in {    say!("Rho World")   |   contract say(@val) = {     stdout!("Hello, "++ val ++"!")   } }
#define  $lasteval #include "lasteval"
#define  $newDirectory($name) new ret, uriCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *uriCh) | for(locker <- uriCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items <- itemsCh) {       if ( items.get("inbox") == Nil ) {         stdout!("you do not have an inbox")       } else {         lookup!($Directory,*uriCh) |         for( Dir <- uriCh ) {           Dir!(*ret) |           for ( map <- ret ) {            @{items.get("inbox")}!(["directory", $name, *map],*stdout) |            /* stdout!(*map) | */            stdout!( $name ++ " created")          } }         } |         stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])       } } }
#define  $add2Directory($name, $key,$value,$ret) new ret, $stdout in {   $peek("directory",$name,*ret) | for (@[dir, ..._] <- ret ) {     @{dir.get("write")}!($key,$value,$ret)  } }
#define  $newBallot($name,$choiceSet) new lookupCh, bCh, $lookup, $stdout in {   lookup!($Ballot, *lookupCh) |   for(Ballot <- lookupCh) {     Ballot!($choiceSet, *bCh) |     for (admin, tally <- bCh) {       $send($myusername, "ballot", $name,  {"admin": *admin, "tally": *tally}) } } }
#define  $voteresults($ballot) new ret, $stdout in {   $peek("ballot",$ballot,*ret) |   for ( @[{"tally": results, ..._}, ..._] <- ret ) {     @results!(*stdout)   } }
#define  $chat($user,$message) $send($user,"chat",$myusername,$message)
#define  $updateinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       $peek(*inboxCh) |       for (oldmail <- inboxCh) {         stdout!(["saving oldmail: ",*oldmail]) |         send!(*oldmail) |         lookup!($locker_%%$myusername, *ret) |         for (locker <- ret) {           stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 2]) |           locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername + 1,             {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |           for (_ <- ret) {             insertArbitrary!(*send,*ret)|             for (uri <- ret) {               stdout!(["#define $inbox_" ++ $myusername, *uri])             }           }         }       }     }   } }
#define  $double($x) 2*$x
#define  $this that"~~"the "~~"other
#define  $stdnames lookup(`rho:registry:lookup`), insertArbitrary(`rho:registry:insertArbitrary`), stdout(`rho:io:stdout`), ack, return
#define  $stdout stdout(`rho:io:stdout`)
#define  $define($name,$value) stdout!(["#define " ++ $name, $value])
#define  $trace trace(`rho:io:stderr`)
#define  $insertArbitrary insertArbitrary(`rho:registry:insertArbitrary`)
#define  $lookup lookup(`rho:registry:lookup`)
#define   $mylockerStore($key,$value) new $stdout, $lookup, nonce , ret in {   lookup!($locker_##$myusername, *ret) |   for (locker <- ret) {      locker!("update", $myprivkey.hexToBytes(),$locker_nonce_##$myusername, {$key: $value}, *stdout, *ret)     for (_ <- ret) {        stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_##$myusername + 1])      }   } }
#define  $test_value "xyzzy"
#define   $mylockerGet($ret) new $stdout, $lookup, nonce , lockerCh in {   lookup!($locker_%%$myusername, *lockerCh) |   for (locker <- lockerCh) {       locker!("get", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername,  *stdout, *$ret) |      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])    } }
#define   $mylockerUpdate($key,$value) new $stdout, $lookup, nonce , lockerCh, ret in {   lookup!($locker_%%$myusername, *lockerCh) |   for (locker <- lockerCh) {       locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername, { $key: $value},  *stdout, *ret) |      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])    } }
#define  $add($x,$y)  ($x + $y)
#define  $newinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       lookup!($locker_%%$myusername, *ret) |       for (locker <- ret) {         stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 1]) |         locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |         for (_ <- ret) {           insertArbitrary!(*send,*ret)|           for (uri <- ret) {             stdout!(["#define $inbox_" ++ $myusername, *uri])           }         }       }     }   } }
#define $locker_nonce_Valentine 3
#define $locker_nonce_Germany 5
#define  $hi new $stdnames, say in {    say!("Rho World")   |   contract say(@val) = {     stdout!("Hello, "++ val ++"!")   } }
#define  $lasteval #include "lasteval"
#define  $newDirectory($name) new ret, uriCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *uriCh) | for(locker <- uriCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("inbox") == Nil ) {         stdout!("you do not have an inbox")       } else {         lookup!($Directory,*uriCh) |         for( Dir <- uriCh ) {           Dir!(*ret) |           for ( map  <- ret )  {            @{items.get("inbox")}!(["directory", $name, *map],*stdout) |            /* stdout!(*map) | */            stdout!( $name ++ " created")          } }         } |         stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])       } } }
#define  $add2Directory($name, $key,$value,$ret) new ret, $stdout in {   $peek("directory",$name,*ret) | for (@[dir, ..._] <- ret ) {     @{dir.get("write")}!($key,$value,$ret)  } }
#define  $newBallot($name,$choiceSet) new lookupCh, bCh, $lookup, $stdout in {   lookup!($Ballot, *lookupCh) |   for(Ballot <- lookupCh) {     Ballot!($choiceSet, *bCh) |     for (admin, tally <- bCh) {       $send($myusername, "ballot", $name,  {"admin": *admin, "tally": *tally}) } } }
#define  $voteresults($ballot) new ret, $stdout in {   $peek("ballot",$ballot,*ret) |   for ( @[{"tally": results, ..._}, ..._] <- ret ) {     @results!(*stdout)   } }
#define  $chat($user,$message) $send($user,"chat",$myusername,$message)
#define  $updateinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       $peek(*inboxCh) |       for (oldmail <- inboxCh) {         stdout!(["saving oldmail: ",*oldmail]) |         send!(*oldmail) |         lookup!($locker_%%$myusername, *ret) |         for (locker <- ret) {           stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 2]) |           locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername + 1,             {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |           for (_ <- ret) {             insertArbitrary!(*send,*ret)|             for (uri <- ret) {               stdout!(["#define $inbox_" ++ $myusername, *uri])             }           }         }       }     }   } }
#define  $double($x) 2*$x
#define  $this that"~~"the "~~"other
#define  $makemylocker new lookupCh, stdout(`rho:io:stdout`),"~~"      lookup(`rho:registry:lookup`), insert(`rho:registry:insertArbitrary`), insCh in {"~~"  lookup!($Locker, *lookupCh) |"~~"  for(Locker <- lookupCh) {"~~"    stdout!(["found Locker",*Locker]) |"~~"    new capabilities, ret, inboxCh, lockerCh, uriCh in {"~~"      Locker!($mypubkey.hexToBytes(), *lockerCh) | for(locker <- lockerCh) {"~~"stdout!("made Locker") |"~~"        insert!(*locker, *uriCh) | for (@uri <- uriCh) {"~~"          stdout!(["#define $locker_" ++ $myusername, uri])"~~"        } |"~~"        lookup!($Inbox, *inboxCh) |"~~"        for (inbox <- inboxCh) {"~~"          inbox!(*capabilities) |"~~"          for (receive, send, peek <- capabilities) {"~~"            stdout!(["#define $locker_nonce_" ++ $myusername, 1]) |"~~"            locker!("update", $myprivkey.hexToBytes(),0, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |"~~"            for (_ <- ret) {"~~"              insert!(*send,*ret)|"~~"              for (uri <- ret) {"~~"                stdout!(["#define $inbox_" ++ $myusername, *uri])"~~"              }"~~"            }"~~"          }"~~"        }"~~"      }"~~"    }"~~"  }"~~"}
#define  $foreach($collection,$code) new $stdout, _loop in {"~~"contract _loop ( @_map ) = {"~~"  match  _map {"~~"    {} => Nil"~~"    { _key: _value, ..._tail } => {"~~"      $code |"~~"      _loop!(_tail)"~~"    }"~~"    []  => Nil"~~"    [_value, ..._tail] /\ _key => {"~~"      $code |"~~"      _loop!(_tail)"~~"    }"~~"    Set() => Nil"~~"    Set(_value, ..._tail) /\ _key => {"~~"      $code |"~~"      _loop!(_tail)"~~"    }"~~"    /* _ /\ _name => {for ( @collection <- @_name ) { _loop!(collection) } } */"~~"  }"~~"} | _loop!( $collection )"~~"}
#define  $receive($type,$subtype_return...) new lockerCh, itemsCh, $stdout, $lookup in {"~~"  lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {"~~"    locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |"~~"    for (@items  <- itemsCh) {"~~"      if ( items.get("receive") == Nil ) {"~~"        stdout!("you do not have a receive channel for your inbox")"~~"      } else {"~~"        @{items.get("receive")}!($type, $subtype_return)"~~"      } |"~~"      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])"~~"} } }
#define  $send($user, $list...) new $stdout, $lookup, ret in {"~~"  lookup!($inbox_%%$user, *ret) |"~~"  for ( inbox <- ret ) {"~~"    inbox!([$list], *stdout)"~~"   }"~~"}
#define  $addUser($username,$directory) new ret, ret1, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {"~~"  lookup!($locker_%%$myusername, *ret) | for(locker <- ret) {"~~"    locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *ret) |"~~"    for (@items  <- ret) {"~~"      /* stdout!(items) | */"~~"      if ( items.get("peek") == Nil ) {"~~"        stdout!("you do not have an mailbox")"~~"      } else {"~~"        stdout!("finding " ++ $directory ++ " in inbox of " ++ $myusername) |"~~"        @{items.get("peek")}!("directory", $directory,*ret) |"~~"        for ( @[{"read": *read, "write": *write, "grant": grant, ..._ }, ..._] <- ret ) {"~~"          if ( *write == Nil ) {"~~"            stdout!("you do not have permission to write to " ++ $directory)"~~"          } else {"~~"            lookup!($inbox_%%$username,*ret) |"~~"            @grant!($username, *ret1) |"~~"            for( inbox <- ret; changekey <- ret1 ) {"~~"              stdout!("adding " ++ $username) |"~~"              write!($username,*inbox, *stdout) |"~~"              inbox!(["directory", $directory,"~~"                {"read": *read, "changekey": *changekey}], *stdout)"~~"            }"~~"          }"~~"      } } |"~~"      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])"~~"} } }
#define $locker_nonce_tgrospic 5
#define  $share($username, $type, $subtype)  new return in {"~~"  $peek($type, $subtype, *return) |"~~"  for ( @[*list] <- return) {"~~"    $send($username, $type, $subtype, *list)"~~"  }"~~"}
#define  $mylocker2($code)  new lockerCh, itemsCh, $stdout, $lookup in {"~~"  lookup!($locker_%%$myusername, *lockerCh) |"~~"  for(_mylocker <- lockerCh) {"~~"     _mylocker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |"~~"     stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |"~~"     for (@_locker <- itemsCh) {"~~"     match _locker {"~~"       {"peek": *_peek, "receive": *_receive, "inbox": *_inbox} => {"~~"         $code"~~"       }"~~"       _ => stdout!(["error: mailbox not found in locker.",_locker])"~~"     }"~~"} } }
#define  $mylocker($code)  new lockerCh, itemsCh, $stdout, $lookup in {"~~"  lookup!($locker_%%$myusername, *lockerCh) |"~~"  for(_mylocker <- lockerCh) {"~~"     _mylocker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |"~~"     stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |"~~"     for (@_locker <- itemsCh) {"~~"     match _locker {"~~"       {"peek": *_peek, "receive": *_receive, "inbox": *_inbox} => {"~~"         $code"~~"       }"~~"       _ => stdout!(["error: mailbox not found in locker.",_locker])"~~"     }"~~"} } }
#define  $peek($types_return...) $mylocker("~~"        _peek!($types_return)"~~")
#define  $readchat $receive("chat",*stdout)
#define  $print($expression) new $stdout in {stdout!($expression)}
#define   $at($timespec,$code) new $stdout in {   $send("dummyuser","event:", $_messageid,  $code) |"~~"  stdout!(["event:", "at:", $timespec,  $_messageid])"~~"}
#define   $readDir($name, $arg_ret...)  new _ret, $stdout in {   $peek("directory",$name,*_ret) |"~~"  for (@[dir] <- _ret ) {"~~"    @{dir.get("read")}!($arg_ret) "~~"  }}
#define   $readDirectory($name, $arg_ret...)  new _ret, $stdout in {   $peek("directory",$name,*_ret) |"~~"  for (@[dir, ..._] <- _ret ) {"~~"    @{dir.get("read")}!($arg_ret) "~~"  }}
#define  $for($name,$channel,$code) new $name in {"~~"  for ( $name <- $channel  ) {"~~"    $code"~~"} }
#define  $new($list...) new $list in
#define  $code($args...) $args
#define    $arg($args...) $%%"code"($args)
#define  $doForever("timespec", $code)  at: $timespec, $arg($code|$at($timespec,$code|$at($timespec,$code)))
#define  $doForever("timespec", $code)  at: $timespec, $arg($code|$at($timespec,i$arg($code|$at($timespec,$code))))
#define  $doForever("timespec", $code)  at: $timespec, $arg($code|$at($timespec,$code|$at($timespec,$code)))
#define  $doForever("timespec", $code)  at: $timespec, $arg($code|$at($timespec,$code|))
#define  $doForever("timespec", $code)  $at( $timespec, $arg($code|$at($timespec,$code|)))
#define $locker_nonce_yao 1
#define  $allowtovote($user,$ballot) new ret in {     $peek("ballot",$ballot,*ret) |"~~"  for ( @[{"admin": admin, ..._}, ..._] <- ret ) {"~~"        @admin!("giveRightToVote", $user, *ret) |"~~"    for (@vote <- ret) {"~~"            $send($user,"vote", $ballot, vote)    "~~"    }}"~~"}
#define  $newCommunity($name) new $stdout, $lookup, ret, ret2 in {"~~" lookup!($Community,*ret)|"~~" lookup!($inbox_%%$myusername,*ret2)|"~~" for ( C <- ret; box <- ret2) {"~~"  C!("new",$name, *box, *ret)|"~~"   for (caps <- ret) {"~~"    if ( *caps != Nil ) {"~~"     box!(["Community",$name,*caps],*stdout)"~~"   } else { stdout!("newCommunity " ++ $name ++ " failed")"~~"   }"~~"}}}
#define   $addmember($community,$user)  new $stdout, $lookup, ret, boxCh, ack in {   $peek("Community",$community,*ret )|"~~"  lookup!($inbox_%%$user,*boxCh)|"~~"  for ( @[{"admin": *admin, "read": *read, "write": *write, "grant": *grant}] <- ret; box <- boxCh ) {"~~"    stdout!("adding user")|"~~"    admin!("add user", $user, *box, *ret) |"~~"    for (selfmod <- ret ) {"~~"       stdout!("user added") |"~~"      box!(["member",$community,{"read": *read, "selfmod": *selfmod}],*stdout)"~~"    }}}
#define  $membersOf($community,$return) new $stdout, ret in { "~~" $peek("member",$community, *ret)|for ( @[caps] <- ret) {"~~"  @{caps.get("read")}! ($return)"~~"}}
#define  $allowalltovote($community,$ballot) new lockerCh, ret, ret1, ret2, ret3, loop, $stdout, $lookup in {     lookup!($locker_%%$myusername, *lockerCh) |"~~"  for(locker <- lockerCh) {"~~"        locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *ret) |"~~"    for (@items  <- ret) {"~~"            @{items.get("peek")}!("member", $community, *ret) |"~~"      @{items.get("peek")}!("ballot", $ballot, *ret1) |"~~"      for ( @[{"read": *read, ..._}, ..._] <- ret;  @[{"admin": *admin, ..._}, ..._] <- ret1 ) {"~~"        stdout!("adding users") |"~~"        contract loop ( @map ) = {"~~"          match  map {"~~"              {} => Nil"~~"             {  username: {"inbox": *inbox, "delegate": delegate, ..._}, ...tail } => {"~~"                  stdout!(["user",username]) |"~~"                  admin!("giveRightToVote", username, *ret2) |"~~"                 for (@vote <- ret2) {"~~"stdout!([username, "added"]) |"~~"                      inbox!(["vote", $ballot, vote], *stdout) |"~~"                      @vote!("delegate",delegate, *stdout)"~~"                 }|"~~"                 loop!(tail)"~~"             }"~~"             somethingelse => stdout!(["somethingelse", somethingelse])"~~"          }"~~"        }|"~~"        read!(*ret3) |"~~"        for ( @members <- ret3 ) {"~~"                 stdout!(["keys",members.keys()]) |"~~"                 loop!(members)"~~"      }} |"~~"      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])"~~"    }}"~~"}
#define  $vote($ballot,$vote) new ret, lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {     lookup!($locker_%%$myusername, *lockerCh) |"~~"  for(locker <- lockerCh) {"~~"        locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |"~~"    for (@items  <- itemsCh) {"~~"            if ( items.get("peek") == Nil ) {"~~"                stdout!("you do not have a peek channel for your inbox")      "~~"      }else {"~~"                stdout!("getting voter") |"~~"        @{items.get("peek")}!("vote", $ballot, *ret) |"~~"        for( @{[voter]}  <- ret ) {"~~"                    @voter!("vote",$vote,*stdout,*ret) |"~~"          stdout!(["vote",$ballot,$myusername,"received"]) |"~~"          for (success <- ret ) {"~~"                       @{items.get("peek")}!("vote", $ballot, *ret) |"~~"            stdout!(*success)          "~~"          }}      "~~"      }|"~~"      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])"~~"    }}"~~"}
#define $inbox_dummyuser `rho:id:rynzwt6gdma3c6h71teq6mg7gyu88bok38yharnmeoq1ogg6pgff4p`
#define   $mydelegate($ballot,$username)   new $stdout, $lookup, ret, ret2 in {    $peek("vote",$ballot, *ret)|"~~"  lookup!($inbox_%%$myusername, *ret2) |"~~"  stdout!("setting delegate") |"~~"  for ( @[vote] <- ret; inbox <- ret2 ) {"~~"    stdout!("got inbox and vote") |"~~"    @vote!("delegate",  $username, *stdout)"~~"  }}
#define   $mydefaultdelegate($community,$username)   new $stdout, $lookup, ret, ret2 in {    $peek("member",$community, *ret)|   lookup!($inbox_%%$myusername, *ret2) |"~~"  stdout!("setting delegate") |"~~"  for ( @[caps] <- ret; inbox <- ret2 ) {"~~"    stdout!("got inbox and capabilities") |"~~"    @{caps.get("selfmod").get("selfupdate")}!({"inbox":  *inbox, "delegate": $username},*stdout)"~~"  }}
#define $Locker `rho:id:s8cnegq8j35k9st1h7bkod4mfdir87irumjw3m6a1h66uh9jnw655x`
#define $Locker `rho:id:s8cnegq8j35k9st1h7bkod4mfdir87irumjw3m6a1h66uh9jnw655x`
#define $locker_jimscarver `rho:id:997g87acwt1duub955ci1ssb1tu6t8hm3jkdxudrryaw93rt5k3um9`
#define  $balance($revaddr) new return(`rho:io:stdout`), lookup(`rho:registry:lookup`),"~~"    RevVaultCh, vaultCh, balanceCh in {"~~"      lookup!(`rho:rchain:revVault`, *RevVaultCh) |"~~"      return!("looking up REV balance for " ++ $revaddr) |"~~"      for (@(_, RevVault) <- RevVaultCh) {"~~"        return!("found revVault") |"~~"        @RevVault!("findOrCreate", $revaddr, *vaultCh) |"~~"        for (@(true, vault) <- vaultCh) {"~~"          @vault!("balance", *balanceCh) |"~~"          for (@balance <- balanceCh) {"~~"            return!(["#define", "$myBalance", balance])"~~"          }"~~"        }"~~"      }"~~"    }
#define  $urireg($value) new return(`rho:io:stdout`), uriCh, valueCh,"~~"  insertArbitrary(`rho:registry:insertArbitrary`)"~~"in {"~~"    insertArbitrary!( $value, *uriCh) |"~~"    for (@uri <- uriCh) {"~~"      return!(("URI", uri))"~~"    }"~~"  }
#define  $lookupuri($uri) new return(`rho:io:stdout`),"~~"  lookup(`rho:registry:lookup`), valueCh in {"~~"   lookup!( `$uri` , *valueCh) |"~~"    for (@value <- valueCh) {"~~"      return!(("Value from registry", value))"~~"    }"~~"  }
#define  $tally($issue)  match ["inbox", $issue] {"~~"  [lockerTag, issue] => {"~~"    new"~~"    return(`rho:rchain:deployId`),"~~"    deployerId(`rho:rchain:deployerId`),"~~"    stdout(`rho:io:stdout`),"~~"    ch"~~"    in {"~~"      for (@{ "peek": *peek, ..._ } <<- @[*deployerId, lockerTag]) {"~~"        peek!("issue", issue, *ch) |"~~"        for (@[{"tally": *tally, ...restOfStuff }] <- ch) {"~~"          tally!(*return, *stdout)"~~"        }"~~"      }"~~"    } "~~"  }"~~"}
#define   $lookupuri($uri) new return(`rho:io:stdout`), lookup(`rho:registry:lookup`), valueCh in {"~~"  lookup!( $uri , *valueCh) |"~~"  for (@value <- valueCh) {"~~"    return!(("Value from registry", value))"~~"  }"~~"}
#define   $lookupuri($uri) explore: new return(`rho:io:stdout`), lookup(`rho:registry:lookup`), valueCh in {"~~"  lookup!( $uri , *valueCh) |"~~"  for (@value <- valueCh) {"~~"    return!(("Value from registry", value))"~~"  }"~~"}
#define   $lookupuri($uri) new return(`rho:io:stdout`), lookup(`rho:registry:lookup`), valueCh in {"~~"  lookup!( $uri , *valueCh) |"~~"  for (@value <- valueCh) {"~~"    return!(("Value from registry", value))"~~"  }"~~"}
#define $myBalance  0
#define $myBalance  10000000000
#define $inbox  `rho:id:78fp3b8mq84e8kwdct93cyezoqexa7mkukkn8rds8mbnrt17az18sz`
#define $inbox  `rho:id:78fp3b8mq84e8kwdct93cyezoqexa7mkukkn8rds8mbnrt17az18sz`
#define $inbox  `rho:id:egfozts5jamxrwwtuwmrah6nixsitjtft73a9sgkrhpwdcuuu6kryo`
#define $inbox  `rho:id:egfozts5jamxrwwtuwmrah6nixsitjtft73a9sgkrhpwdcuuu6kryo`
#define $Issue `rho:id:a1pc6fs6ppbi3uwmcxh3k44f9cyeko617uzf3r6q7op4kmx8496ip4`
#define $Issue `rho:id:a1pc6fs6ppbi3uwmcxh3k44f9cyeko617uzf3r6q7op4kmx8496ip4`
#define $inbox  `rho:id:7cu8qtd7kh4sr7yys9hs1scwjgqf3op1bwexeaeye6zifsnmans1e1`
#define $inbox  `rho:id:7cu8qtd7kh4sr7yys9hs1scwjgqf3op1bwexeaeye6zifsnmans1e1`
#define $inbox  `rho:id:brepys39ymegx9f4b963diz633fwsibuzgpijphrdtm9z1r4jxnyg8`
#define $inbox  `rho:id:brepys39ymegx9f4b963diz633fwsibuzgpijphrdtm9z1r4jxnyg8`
#define $inbox  `rho:id:ezhbzn63rxoisi33py11jpf1f9g6jeqgcpxebtcko5bu4bjpakbpq3`
#define $inbox  `rho:id:ezhbzn63rxoisi33py11jpf1f9g6jeqgcpxebtcko5bu4bjpakbpq3`
#define $inbox  `rho:id:xqsxesxxumd8ssa8suhbqezsngt6qwsdywywrskjnsp9r5fgf33f87`
#define $inbox  `rho:id:xqsxesxxumd8ssa8suhbqezsngt6qwsdywywrskjnsp9r5fgf33f87`
#define $Issue `rho:id:ubc194ikekabyis3mub3bmqcrkrico4j8adjpa5zbx96iwux7ywf89`
#define $Issue `rho:id:ubc194ikekabyis3mub3bmqcrkrico4j8adjpa5zbx96iwux7ywf89`
#define $inbox  `rho:id:wiqdk9bb48wgp84dbgcej3qqjm7a83sjsamz81ba8jp38bb7wrxk4e`
#define $inbox  `rho:id:wiqdk9bb48wgp84dbgcej3qqjm7a83sjsamz81ba8jp38bb7wrxk4e`
#define $inbox  `rho:id:zr69asw6751obskez7r4s94189pebjapam9q1sjpaw5rsw93uqd9bu`
#define $inbox  `rho:id:zr69asw6751obskez7r4s94189pebjapam9q1sjpaw5rsw93uqd9bu`
#define $inbox  `rho:id:fmdxd9uu45e4xgraom715qpnyj33g676pis8o4177qdssokp1we3o8`
#define $inbox  `rho:id:fmdxd9uu45e4xgraom715qpnyj33g676pis8o4177qdssokp1we3o8`
#define $inbox  `rho:id:5ahfds4mbsmu5mnxr8it15kxiapsxag73tsnaodpc9uh87jw375jio`
#define $inbox  `rho:id:5ahfds4mbsmu5mnxr8it15kxiapsxag73tsnaodpc9uh87jw375jio`
#define $inbox  `rho:id:c5ii7untb468y64h355pzh5yx5iswzezihrsf4z15y1z6j5pjtxttq`
#define $inbox  `rho:id:c5ii7untb468y64h355pzh5yx5iswzezihrsf4z15y1z6j5pjtxttq`
#define $inbox  `rho:id:9pcgmw3xznfwen5qk6qprtxkbuw83wjzoeoqp3egj9za8k3yimoaso`
#define $inbox  `rho:id:9pcgmw3xznfwen5qk6qprtxkbuw83wjzoeoqp3egj9za8k3yimoaso`
#define $inbox  `rho:id:5xa9c737iakgs7wuqwm1ifrsb1jgojhshhrhdotzm4bxirzfzu95bu`
#define $inbox  `rho:id:5xa9c737iakgs7wuqwm1ifrsb1jgojhshhrhdotzm4bxirzfzu95bu`
#define $inbox  `rho:id:b1a5pzykqzgom1986noxtoyfqm7jo9o14zh7uded4cfnskq6d8k88t`
#define $inbox  `rho:id:b1a5pzykqzgom1986noxtoyfqm7jo9o14zh7uded4cfnskq6d8k88t`
#define $Group `rho:id:ahhgoqb8wmn8u88qcx5tndft4ksah8nnpa8rkxa1tekrmm7wan9meh`
#define $Group `rho:id:ahhgoqb8wmn8u88qcx5tndft4ksah8nnpa8rkxa1tekrmm7wan9meh`
#define $Group `rho:id:e6k8bpsyiog5phmgntn1ygk5suerrtdmndojrhn6yhm8g5qx1omu6t`
#define $Group `rho:id:e6k8bpsyiog5phmgntn1ygk5suerrtdmndojrhn6yhm8g5qx1omu6t`
#define $inbox  `rho:id:88mou8pg5tfu1nbtq6y539hnembt33y19h7jkhyp3qpjokwysa71st`
#define $inbox  `rho:id:88mou8pg5tfu1nbtq6y539hnembt33y19h7jkhyp3qpjokwysa71st`
#define $Group `rho:id:oyrt3q7x8d6d76uwmw3wko7z74yjoz85bwxqy97f77dgdxcfwor6mx`
#define $Group `rho:id:oyrt3q7x8d6d76uwmw3wko7z74yjoz85bwxqy97f77dgdxcfwor6mx`
#define $myBalance  999978783181529
#define $myBalance  999978783181529
