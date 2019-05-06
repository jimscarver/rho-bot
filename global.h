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
#define  $vote($ballot,$vote) new ret, lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("peek") == Nil ) {         stdout!("you do not have a peek channel for your inbox")       } else {         stdout!("getting voter") |         @{items.get("peek")}!("vote", $ballot, *ret) |         for( @{[voter]}  <- ret ) {           @voter!("vote",$vote,*stdout,*ret) |           stdout!(["vote",$ballot,$myusername,"received"]) |           for (success <- ret ) {            @{items.get("receive")}!("vote", $ballot, *ret) |            stdout!(*success)           }         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define  $lasteval #include "lasteval"
#define  $newDirectory($name) new ret, uriCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *uriCh) | for(locker <- uriCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items <- itemsCh) {       if ( items.get("inbox") == Nil ) {         stdout!("you do not have an inbox")       } else {         lookup!($Directory,*uriCh) |         for( Dir <- uriCh ) {           Dir!(*ret) |           for ( map <- ret ) {            @{items.get("inbox")}!(["directory", $name, *map],*stdout) |            /* stdout!(*map) | */            stdout!( $name ++ " created")          } }         } |         stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])       } } }
#define  $add2Directory($name, $key,$value,$ret) new ret, $stdout in {   $peek("directory",$name,*ret) | for (@[dir, ..._] <- ret ) {     @{dir.get("write")}!($key,$value,$ret)  } }
#define  $newBallot($name,$choiceSet) new lookupCh, bCh, $lookup, $stdout in {   lookup!($Ballot, *lookupCh) |   for(Ballot <- lookupCh) {     Ballot!($choiceSet, *bCh) |     for (admin, tally <- bCh) {       $send($myusername, "ballot", $name,  {"admin": *admin, "tally": *tally}) } } }
#define  $voteresults($ballot) new ret, $stdout in {   $peek("ballot",$ballot,*ret) |   for ( @[{"tally": results, ..._}, ..._] <- ret ) {     @results!(*stdout)   } }
#define  $allowalltovote($directory,$ballot) new lockerCh, ret, ret1, loop, $stdout, $lookup in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *ret) |     for (@items  <- ret) {       @{items.get("peek")}!("directory", $directory, *ret) |       @{items.get("peek")}!("ballot", $ballot, *ret1) |       for ( @[{"read": *read, ..._}, ..._] <- ret;             @[{"admin": *admin, ..._}, ..._] <- ret1 ) {         contract loop ( @map ) = {           match  map {             {} => Nil             { username: *inbox, ...tail } => {               admin!("giveRightToVote", *ret) |               for (@vote <- ret) {                 inbox!(["vote", $ballot, vote])               } |               loop!(tail)             }           }         } |         read!(*ret) |         for ( @members <- ret ) {           loop!(members)         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])     }   } }
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
#define $locker_henleynatural `rho:id:e9tqyxqpxojr39ijirg3xki1hg19z4pamtbgqyduucwbgk4mrsr4qr`
#define $locker_nonce_henleynatural 2
#define $locker_Valentine `rho:id:9rj6x4ax1hatnw6z3jfrf7grwrrx5bkpxwcr34g87598yrwph4658e`
#define  $add($x,$y)  ($x + $y)
#define  $newinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       lookup!($locker_%%$myusername, *ret) |       for (locker <- ret) {         stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 1]) |         locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |         for (_ <- ret) {           insertArbitrary!(*send,*ret)|           for (uri <- ret) {             stdout!(["#define $inbox_" ++ $myusername, *uri])           }         }       }     }   } }
#define $locker_nonce_Valentine 3
#define $locker_nonce_Germany 5
#define  $locker_Germany `rho:id:u5bx58kusz6gy45nu1ts3bosr6ymxeja9wue1jd1ko8udzmuux1od5`
#define $locker_Emmanuel_mebyz `rho:id:so9711cgiq6ru5s7njkypzequduqc5txgru7k85baosc6z1jqjqhd6`
#define $NonNegativeNumber `rho:id:is3wbjardqqge3obbuogfacc5sh77iwxidep3wyhkrsc9r5aikwgi5`
#define  $hi new $stdnames, say in {    say!("Rho World")   |   contract say(@val) = {     stdout!("Hello, "++ val ++"!")   } }
#define $inbox_Emmanuel_mebyz `rho:id:jd1wzadn7xpjkmrxh8ftmtrkiqb4z3uwob89xk5zw95ijfud89g66n`
#define $locker_nonce_Emmanuel_mebyz 6
#define  $vote($ballot,$vote) new ret, lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("peek") == Nil ) {         stdout!("you do not have a peek channel for your inbox")       } else {         stdout!("getting voter") |         @{items.get("peek")}!("vote", $ballot, *ret) |         for( @{[voter]}  <- ret ) {           @voter!("vote",$vote,*stdout,*ret) |           stdout!(["vote",$ballot,$myusername,"received"]) |           for (success <- ret ) {            @{items.get("receive")}!("vote", $ballot, *ret) |            stdout!(*success)           }         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define  $lasteval #include "lasteval"
#define  $newDirectory($name) new ret, uriCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *uriCh) | for(locker <- uriCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("inbox") == Nil ) {         stdout!("you do not have an inbox")       } else {         lookup!($Directory,*uriCh) |         for( Dir <- uriCh ) {           Dir!(*ret) |           for ( map  <- ret )  {            @{items.get("inbox")}!(["directory", $name, *map],*stdout) |            /* stdout!(*map) | */            stdout!( $name ++ " created")          } }         } |         stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])       } } }
#define $MakeMint `rho:id:i4pb65o68q7mgkqpp6ybb41d6bqm153g38xrtymjeg7gdasri3q47b`
#define $locker_Ed `rho:id:b6tgw11fzji4bdpowdnk6uffr3ztkaymisoebohhe8yjdoaimoogu4`
#define $inbox_Ed `rho:id:zpadycadit7147iwtk4ykfgqzzj1jz5fs57mp1kuc1inurtmyc9rjh`
#define $locker_nonce_Ed 5
#define  $add2Directory($name, $key,$value,$ret) new ret, $stdout in {   $peek("directory",$name,*ret) | for (@[dir, ..._] <- ret ) {     @{dir.get("write")}!($key,$value,$ret)  } }
#define  $newBallot($name,$choiceSet) new lookupCh, bCh, $lookup, $stdout in {   lookup!($Ballot, *lookupCh) |   for(Ballot <- lookupCh) {     Ballot!($choiceSet, *bCh) |     for (admin, tally <- bCh) {       $send($myusername, "ballot", $name,  {"admin": *admin, "tally": *tally}) } } }
#define  $voteresults($ballot) new ret, $stdout in {   $peek("ballot",$ballot,*ret) |   for ( @[{"tally": results, ..._}, ..._] <- ret ) {     @results!(*stdout)   } }
#define  $allowalltovote($directory,$ballot) new lockerCh, ret, ret1, loop, $stdout, $lookup in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *ret) |     for (@items  <- ret) {       @{items.get("peek")}!("directory", $directory, *ret) |       @{items.get("peek")}!("ballot", $ballot, *ret1) |       for ( @[{"read": *read, ..._}, ..._] <- ret;             @[{"admin": *admin, ..._}, ..._] <- ret1 ) {         contract loop ( @map ) = {           match  map {             {} => Nil             { username: *inbox, ...tail } => {               admin!("giveRightToVote", *ret) |               for (@vote <- ret) {                 inbox!(["vote", $ballot, vote])               } |               loop!(tail)             }           }         } |         read!(*ret) |         for ( @members <- ret ) {           loop!(members)         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])     }   } }
#define $locker_Owans `rho:id:muoibisbyqztaa9didj65otyxoio95yjkgqj7sgpo55xakhr4ysarq`
#define $locker_nonce_Owans 1
#define $inbox_Owans `rho:id:k3465k7e67faohyo8kt9n9ypaaeiru7hyenbys9rjq3x79a9ssx4dn`
#define  $chat($user,$message) $send($user,"chat",$myusername,$message)
#define  $updateinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       $peek(*inboxCh) |       for (oldmail <- inboxCh) {         stdout!(["saving oldmail: ",*oldmail]) |         send!(*oldmail) |         lookup!($locker_%%$myusername, *ret) |         for (locker <- ret) {           stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 2]) |           locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername + 1,             {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |           for (_ <- ret) {             insertArbitrary!(*send,*ret)|             for (uri <- ret) {               stdout!(["#define $inbox_" ++ $myusername, *uri])             }           }         }       }     }   } }
#define  $double($x) 2*$x
#define  $this that"~~"the "~~"other
#define  $makemylocker new lookupCh, stdout(`rho:io:stdout`),"~~"      lookup(`rho:registry:lookup`), insert(`rho:registry:insertArbitrary`), insCh in {"~~"  lookup!($Locker, *lookupCh) |"~~"  for(Locker <- lookupCh) {"~~"    stdout!(["found Locker",*Locker]) |"~~"    new capabilities, ret, inboxCh, lockerCh, uriCh in {"~~"      Locker!($mypubkey.hexToBytes(), *lockerCh) | for(locker <- lockerCh) {"~~"stdout!("made Locker") |"~~"        insert!(*locker, *uriCh) | for (@uri <- uriCh) {"~~"          stdout!(["#define $locker_" ++ $myusername, uri])"~~"        } |"~~"        lookup!($Inbox, *inboxCh) |"~~"        for (inbox <- inboxCh) {"~~"          inbox!(*capabilities) |"~~"          for (receive, send, peek <- capabilities) {"~~"            stdout!(["#define $locker_nonce_" ++ $myusername, 1]) |"~~"            locker!("update", $myprivkey.hexToBytes(),0, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |"~~"            for (_ <- ret) {"~~"              insert!(*send,*ret)|"~~"              for (uri <- ret) {"~~"                stdout!(["#define $inbox_" ++ $myusername, *uri])"~~"              }"~~"            }"~~"          }"~~"        }"~~"      }"~~"    }"~~"  }"~~"}
#define  $makemylocker new lookupCh, stdout(`rho:io:stdout`),"~~"      lookup(`rho:registry:lookup`), insert(`rho:registry:insertArbitrary`), insCh in {"~~"  lookup!($Locker, *lookupCh) |"~~"  for(Locker <- lookupCh) {"~~"    stdout!(["found Locker",*Locker]) |"~~"    new capabilities, ret, inboxCh, lockerCh, uriCh in {"~~"      Locker!($mypubkey.hexToBytes(), *lockerCh) | for(locker <- lockerCh) {"~~"stdout!("made Locker") |"~~"        insert!(*locker, *uriCh) | for (@uri <- uriCh) {"~~"          stdout!(["#define $locker_" ++ $myusername, uri])"~~"        } |"~~"        lookup!($Inbox, *inboxCh) |"~~"        for (inbox <- inboxCh) {"~~"          inbox!(*capabilities) |"~~"          for (receive, send, peek <- capabilities) {"~~"            stdout!(["#define $locker_nonce_" ++ $myusername, 1]) |"~~"            locker!("update", $myprivkey.hexToBytes(),0, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |"~~"            for (_ <- ret) {"~~"              insert!(*send,*ret)|"~~"              for (uri <- ret) {"~~"                stdout!(["#define $inbox_" ++ $myusername, *uri])"~~"              }"~~"            }"~~"          }"~~"        }"~~"      }"~~"    }"~~"  }"~~"}
#define  $foreach($collection,$code) new $stdout, _loop in {"~~"contract _loop ( @_map ) = {"~~"  match  _map {"~~"    {} => Nil"~~"    { _key: _value, ..._tail } => {"~~"      $code |"~~"      _loop!(_tail)"~~"    }"~~"    []  => Nil"~~"    [_value, ..._tail] /\ _key => {"~~"      $code |"~~"      _loop!(_tail)"~~"    }"~~"    Set() => Nil"~~"    Set(_value, ..._tail) /\ _key => {"~~"      $code |"~~"      _loop!(_tail)"~~"    }"~~"    /* _ /\ _name => {for ( @collection <- @_name ) { _loop!(collection) } } */"~~"  }"~~"} | _loop!( $collection )"~~"}
#define  $receive($type,$subtype_return...) new lockerCh, itemsCh, $stdout, $lookup in {"~~"  lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {"~~"    locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |"~~"    for (@items  <- itemsCh) {"~~"      if ( items.get("receive") == Nil ) {"~~"        stdout!("you do not have a receive channel for your inbox")"~~"      } else {"~~"        @{items.get("receive")}!($type, $subtype_return)"~~"      } |"~~"      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])"~~"} } }
#define  $send($user, $list...) new $stdout, $lookup, ret in {"~~"  lookup!($inbox_%%$user, *ret) |"~~"  for ( inbox <- ret ) {"~~"    inbox!([$list], *stdout)"~~"   }"~~"}
#define  $addUser($username,$directory) new ret, ret1, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {"~~"  lookup!($locker_%%$myusername, *ret) | for(locker <- ret) {"~~"    locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *ret) |"~~"    for (@items  <- ret) {"~~"      /* stdout!(items) | */"~~"      if ( items.get("peek") == Nil ) {"~~"        stdout!("you do not have an mailbox")"~~"      } else {"~~"        stdout!("finding " ++ $directory ++ " in inbox of " ++ $myusername) |"~~"        @{items.get("peek")}!("directory", $directory,*ret) |"~~"        for ( @[{"read": *read, "write": *write, "grant": grant, ..._ }, ..._] <- ret ) {"~~"          if ( *write == Nil ) {"~~"            stdout!("you do not have permission to write to " ++ $directory)"~~"          } else {"~~"            lookup!($inbox_%%$username,*ret) |"~~"            @grant!($username, *ret1) |"~~"            for( inbox <- ret; changekey <- ret1 ) {"~~"              stdout!("adding " ++ $username) |"~~"              write!($username,*inbox, *stdout) |"~~"              inbox!(["directory", $directory,"~~"                {"read": *read, "changekey": *changekey}], *stdout)"~~"            }"~~"          }"~~"      } } |"~~"      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])"~~"} } }
#define $locker_GaryC `rho:id:src8chtknsp6dswe5wcmjjc9rhbyh5fojros9ymptuodbqwnibu4s7`
#define $inbox_GaryC `rho:id:wuo5rztqroa76qeqhprgswdch9rkggrbpbb1qxq4tpq1a8rk3k4nid`
#define $locker_tgrospic `rho:id:tr7twmx38b9qq5czqzjzgapbry53yknkstsr1qfi6axwnngapnwq9o`
#define $inbox_tgrospic `rho:id:aj4d9kspsg3wib5af6hc7wuqbdgm1w89854rf535bjut1kxj9byyrc`
#define $locker_nonce_tgrospic 5
#define  $share($username, $type, $subtype)  new return in {"~~"  $peek($type, $subtype, *return) |"~~"  for ( @[*list] <- return) {"~~"    $send($username, $type, $subtype, *list)"~~"  }"~~"}
#define $locker_nonce_GaryC 8
#define $inbox_jimscarver `rho:id:wtfq8i18f46syqos11d5wwrg3whdx7q5fyndwtmdypdbqwzru79no9`
#define  $mylocker2($code)  new lockerCh, itemsCh, $stdout, $lookup in {"~~"  lookup!($locker_%%$myusername, *lockerCh) |"~~"  for(_mylocker <- lockerCh) {"~~"     _mylocker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |"~~"     stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |"~~"     for (@_locker <- itemsCh) {"~~"     match _locker {"~~"       {"peek": *_peek, "receive": *_receive, "inbox": *_inbox} => {"~~"         $code"~~"       }"~~"       _ => stdout!(["error: mailbox not found in locker.",_locker])"~~"     }"~~"} } }
#define  $mylocker($code)  new lockerCh, itemsCh, $stdout, $lookup in {"~~"  lookup!($locker_%%$myusername, *lockerCh) |"~~"  for(_mylocker <- lockerCh) {"~~"     _mylocker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |"~~"     stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |"~~"     for (@_locker <- itemsCh) {"~~"     match _locker {"~~"       {"peek": *_peek, "receive": *_receive, "inbox": *_inbox} => {"~~"         $code"~~"       }"~~"       _ => stdout!(["error: mailbox not found in locker.",_locker])"~~"     }"~~"} } }
#define  $peek($types_return...) $mylocker("~~"        _peek!($types_return)"~~")
#define $inbox_SteveHenley `rho:id:awa1ybic84pahmhk9ffcjx6ji4womjzb315oag3p18h1dpzmge1h6f`
#define $locker_SteveHenley `rho:id:uju9m7b65wfwp6mw139ffu1on3kt61wjug6oi9zn6nm58ffe5aek3j`
#define  $readchat $receive("chat",*stdout)
#define $locker_nonce_SteveHenley 18
#define  $locker_dummyuser `rho:id:c6e643opwowg8k1hdawnexsmo37twucf3cuazsndtu9xgnrc4wfymu`
#define  $inbox_dummyuser `rho:id:c7mfqadh6id74erjnd8bh1eb8qz91pnmner9zdx146i1yjg3dhguyy`
#define  $print($expression) new $stdout in {stdout!($expression)}
#define $locker_aviation_hacker `rho:id:fii899z58qzguttignbj39id94kuij8dhie6graqogmftqbcyfemkd`
#define $inbox_aviation_hacker `rho:id:a1hjgkfo67kiz13jeghyqnpkohsxzp6xwbjryzwkxdifktyk66ihou`
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
#define $locker_nonce_dummyuser 41
#define $locker_yao `rho:id:sdm4y951fgmbtxihami14qo3zhsfxqi1wb4qw4xpgn7ut17ep6wsfy`
#define $locker_nonce_yao 1
#define $inbox_yao `rho:id:7xod9kw7kf7g97d3byetiyydf7arfc3afuro55ngqwzphcpebuwmm5`
#define $locker_nonce_aviation_hacker 5
#define $Poll `rho:id:kbghsngehgn3t1a8mym7itmjjtcefudptb8wdmr76shxyuhwkg91ws`
#define $locker_momchilov `rho:id:iqsnfap1hh1wjuhjmw96diz3yymfcocs5zttx5nuzwwwyz5j77n51n`
#define $inbox_momchilov `rho:id:oyw5hqousp1au8y96xun96xegu748wk9rw6pk74xn9om7qcfqjrtno`
#define  $allowtovote($user,$ballot) new ret in {     $peek("ballot",$ballot,*ret) |"~~"  for ( @[{"admin": admin, ..._}, ..._] <- ret ) {"~~"        @admin!("giveRightToVote", $user, *ret) |"~~"    for (@vote <- ret) {"~~"            $send($user,"vote", $ballot, vote)    "~~"    }}"~~"}
#define $locker_nonce_momchilov 9
#define $Ballot `rho:id:3pxdr7yzgfomdnzedyiet64qyqjz87tfkt31ico8hb8watp6bp5pes`
#define $getTicket `rho:id:k7hcncc35jrygdrwzactfp3sxtfqhigr18hy6btk7bs1dsbkqsbx5j`
#define $Community `rho:id:c6oakwbqkfgyya6aobtxmqgx33t5iftysnqmoaugnhzrg9mekor4ym`
#define $locker_jimscarver `rho:id:e5rgmynksxbp4khwn8yi6uzpemfymjytf69np5tacw566oga95yywz`
#define $locker_nonce_jimscarver 177
#define $Inbox `rho:id:gkrjusfzhjo81auedk3i54qhwa1b5a6crc95mxu45a75q8iyy83jcj`
#define $Inbox `rho:id:gkrjusfzhjo81auedk3i54qhwa1b5a6crc95mxu45a75q8iyy83jcj`
#define $Locker `rho:id:ww1g4a351ahgtztq6zfiey6bt1dswrznw6f3dct59fesqupnu5su8g`
#define $Locker `rho:id:ww1g4a351ahgtztq6zfiey6bt1dswrznw6f3dct59fesqupnu5su8g`
#define $Directory `rho:id:f9kmasb4g1t8a4o5dtbxmdnqdzjpoisyy6kdtjf8raxwes3iy3rbz9`
#define $Directory `rho:id:mzghhmzqr8x5zjj8n3h6xa11ndt7cm1rawmier4atqo1argbb5i9xx`
#define $Directory `rho:id:qsabcreprma4iq4hbfwgh7cnuw3kpddkrxdmbx7ejbtnoyz5qj1tf3`
#define $Directory `rho:id:5q3dztqz91soijtjjzxnna9e5ajywhnbgt6eamimojycb17gpwf5sk`
#define $Community `rho:id:imnmf6wdiog49xzgiukbchiwuo1dm7gf7r1168ux6g1q141r8u99mr`
