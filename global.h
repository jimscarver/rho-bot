#define  $stdnames lookup(`rho:registry:lookup`), insertArbitrary(`rho:registry:insertArbitrary`), stdout(`rho:io:stdout`), ack, return
#define  $print($string) new stdout(`rho:io:stdout`) in { stdout!($string) }
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
#define  $send($user, $list...) new $stdout, $lookup, ret in {   lookup!($inbox_%%$user, *ret) |   for ( inbox <- ret ) {     inbox!([$list], *stdout)    } }
#define  $peek($types_return...) new lockerCh, itemsCh, $stdout, $lookup in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("peek") == Nil ) {         stdout!("you do not have a peek channel for your inbox")       } else {         @{items.get("peek")}!($types_return)       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define $NonNegativeNumber `rho:id:is3wbjardqqge3obbuogfacc5sh77iwxidep3wyhkrsc9r5aikwgi5`
#define  $hi new $stdnames, say in {    say!("Rho World")   |   contract say(@val) = {     stdout!("Hello, "++ val ++"!")   } }
#define  $vote($ballot,$vote) new ret, lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("peek") == Nil ) {         stdout!("you do not have a peek channel for your inbox")       } else {         stdout!("getting voter") |         @{items.get("peek")}!("vote", $ballot, *ret) |         for( @{[voter]}  <- ret ) {           @voter!("vote",$vote,*stdout,*ret) |           stdout!(["vote",$ballot,$myusername,"received"]) |           for (success <- ret ) {            @{items.get("receive")}!("vote", $ballot, *ret) |            stdout!(*success)           }         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define  $share($username, $type, $subtype) new return in {   $peek($type, $subtype, *return) |   for ( list <- return) {     $send($username, [$type, $subtype] ++ *list)   } }
#define  $lasteval #include "lasteval"
#define  $newDirectory($name) new ret, uriCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *uriCh) | for(locker <- uriCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items <- itemsCh) {       if ( items.get("inbox") == Nil ) {         stdout!("you do not have an inbox")       } else {         lookup!($Directory,*uriCh) |         for( Dir <- uriCh ) {           Dir!(*ret) |           for ( map <- ret ) {            @{items.get("inbox")}!(["directory", $name, *map],*stdout) |            /* stdout!(*map) | */            stdout!( $name ++ " created")          } }         } |         stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])       } } }
#define $MakeMint `rho:id:i4pb65o68q7mgkqpp6ybb41d6bqm153g38xrtymjeg7gdasri3q47b`
#define  $readDirectory($name, $arg_ret...) new ret, $stdout in {   $peek("directory",$name,*ret) | for (@[dir, ..._] <- ret ) {     @{dir.get("read")}!($arg_ret)  } }
#define  $add2Directory($name, $key,$value,$ret) new ret, $stdout in {   $peek("directory",$name,*ret) | for (@[dir, ..._] <- ret ) {     @{dir.get("write")}!($key,$value,$ret)  } }
#define  $newBallot($name,$choiceSet) new lookupCh, bCh, $lookup, $stdout in {   lookup!($Ballot, *lookupCh) |   for(Ballot <- lookupCh) {     Ballot!($choiceSet, *bCh) |     for (chair, getWinner <- bCh) {       $send($myusername, "ballot", $name,  {"chair": *chair, "getWinner": *getWinner}) } } }
#define  $allowtovote($user,$ballot) new ret in {   $peek("ballot",$ballot,*ret) |   for ( @[{"chair": chair, ..._}, ..._] <- ret ) {     @chair!("giveRightToVote", *ret) |     for (@vote <- ret) {       $send($user,"vote", $ballot, vote)     }   } }
#define  $voteresults($ballot) new ret, $stdout in {   $peek("ballot",$ballot,*ret) |   for ( @[{"getWinner": results, ..._}, ..._] <- ret ) {     @results!(*stdout)   } }
#define  $receive($type,$subtype_return...) new lockerCh, itemsCh, $stdout, $lookup in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("receive") == Nil ) {         stdout!("you do not have a receive channel for your inbox")       } else {         @{items.get("receive")}!($type, $subtype_return)       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define  $allowalltovote($directory,$ballot) new lockerCh, ret, ret1, loop, $stdout, $lookup in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *ret) |     for (@items  <- ret) {       @{items.get("peek")}!("directory", $directory, *ret) |       @{items.get("peek")}!("ballot", $ballot, *ret1) |       for ( @[{"read": *read, ..._}, ..._] <- ret;             @[{"chair": *chair, ..._}, ..._] <- ret1 ) {         contract loop ( @map ) = {           match  map {             {} => Nil             { username: *inbox, ...tail } => {               chair!("giveRightToVote", *ret) |               for (@vote <- ret) {                 inbox!(["vote", $ballot, vote])               } |               loop!(tail)             }           }         } |         read!(*ret) |         for ( @members <- ret ) {           loop!(members)         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])     }   } }
#define  $addUser( $username,$directory) new ret, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *ret) | for(locker <- ret) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *ret) |     for (@items  <- ret) {       /* stdout!(items) | */       if ( items.get("peek") == Nil ) {         stdout!("you do not have an mailbox")       } else {         stdout!("finding " ++ $directory ++ " in inbox of " ++ $myusername) |         @{items.get("peek")}!("directory", $directory,*ret) |         for ( @[{"read": *read, "write": *write, ..._ }, ..._] <- ret ) {           if ( *write == Nil ) {             stdout!("you do not have permission to write to " ++ $directory)           } else {             lookup!($inbox_%%$username,*ret) |             for( inbox <- ret ) {               stdout!("adding " ++ $username) |               write!($username,*inbox, *stdout) |               inbox!(["directory", $directory, {"read": *read}], *stdout)             }           }       } } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define  $foreach($collection,$code) new $stdout, _loop in { contract _loop ( @_map ) = {   match  _map {     {} => Nil     { _key: _value, ..._tail } => {       $code |       _loop!(_tail)     }     []  => Nil     [_value, ..._tail] /\ _key => {       $code |       _loop!(_tail)     }     Set() => Nil     Set(_value, ..._tail) /\ _key => {       $code |       _loop!(_tail)     }   } } | _loop!( $collection ) }
#define  $chat($user,$message) $send($user,"chat",$myusername,$message)
#define  $Ballot
#define  $updateinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       $peek(*inboxCh) |       for (oldmail <- inboxCh) {         stdout!(["saving oldmail: ",*oldmail]) |         send!(*oldmail) |         lookup!($locker_%%$myusername, *ret) |         for (locker <- ret) {           stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 2]) |           locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername + 1,             {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |           for (_ <- ret) {             insertArbitrary!(*send,*ret)|             for (uri <- ret) {               stdout!(["#define $inbox_" ++ $myusername, *uri])             }           }         }       }     }   } }
#define  $double($x) 2*$x
#define  $this that"~~"the "~~"other
#define $Locker `rho:id:djbh4ogpqc7fn9tp5ahz6kpuonmtqbype11uycf75fgnrdgz6dj496`
#define $Inbox `rho:id:ayqysruudhpa9r1iurargh6wgbyjyr3nk16ey1dzyztofn44kj4w6w`
#define $Directory `rho:id:sq8mauht78ufw7r1etupb8mzgp4j8ii5etpo81ec3t5k57b6958ju5`
#define  $stdnames lookup(`rho:registry:lookup`), insertArbitrary(`rho:registry:insertArbitrary`), stdout(`rho:io:stdout`), ack, return
#define  $print($string) new stdout(`rho:io:stdout`) in { stdout!($string) }
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
#define  $inbox_dummyuser `rho:id:ya687kjhwtyj877fedddh9jiac5ydfpcjycyfa16cduhrhmwc7n738`
#define  $add($x,$y)  ($x + $y)
#define  $newinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       lookup!($locker_%%$myusername, *ret) |       for (locker <- ret) {         stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 1]) |         locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |         for (_ <- ret) {           insertArbitrary!(*send,*ret)|           for (uri <- ret) {             stdout!(["#define $inbox_" ++ $myusername, *uri])           }         }       }     }   } }
#define $locker_nonce_Valentine 3
#define $locker_nonce_Germany 5
#define  $send($user, $list...) new $stdout, $lookup, ret in {   lookup!($inbox_%%$user, *ret) |   for ( inbox <- ret ) {     inbox!([$list], *stdout)    } }
#define  $locker_Germany `rho:id:u5bx58kusz6gy45nu1ts3bosr6ymxeja9wue1jd1ko8udzmuux1od5`
#define  $peek($types_return...) new lockerCh, itemsCh, $stdout, $lookup in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("peek") == Nil ) {         stdout!("you do not have a peek channel for your inbox")       } else {         @{items.get("peek")}!($types_return)       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define $locker_Emmanuel_mebyz `rho:id:so9711cgiq6ru5s7njkypzequduqc5txgru7k85baosc6z1jqjqhd6`
#define $NonNegativeNumber `rho:id:is3wbjardqqge3obbuogfacc5sh77iwxidep3wyhkrsc9r5aikwgi5`
#define  $hi new $stdnames, say in {    say!("Rho World")   |   contract say(@val) = {     stdout!("Hello, "++ val ++"!")   } }
#define $inbox_Emmanuel_mebyz `rho:id:jd1wzadn7xpjkmrxh8ftmtrkiqb4z3uwob89xk5zw95ijfud89g66n`
#define $locker_nonce_Emmanuel_mebyz 6
#define  $vote($ballot,$vote) new ret, lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("peek") == Nil ) {         stdout!("you do not have a peek channel for your inbox")       } else {         stdout!("getting voter") |         @{items.get("peek")}!("vote", $ballot, *ret) |         for( @{[voter]}  <- ret ) {           @voter!("vote",$vote,*stdout,*ret) |           stdout!(["vote",$ballot,$myusername,"received"]) |           for (success <- ret ) {            @{items.get("receive")}!("vote", $ballot, *ret) |            stdout!(*success)           }         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define  $share($username, $type, $subtype) new return in {   $peek($type, $subtype, *return) |   for ( list <- return) {     $send($username, [$type, $subtype] ++ *list)   } }
#define  $lasteval #include "lasteval"
#define  $newDirectory($name) new ret, uriCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *uriCh) | for(locker <- uriCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("inbox") == Nil ) {         stdout!("you do not have an inbox")       } else {         lookup!($Directory,*uriCh) |         for( Dir <- uriCh ) {           Dir!(*ret) |           for ( map  <- ret )  {            @{items.get("inbox")}!(["directory", $name, *map],*stdout) |            /* stdout!(*map) | */            stdout!( $name ++ " created")          } }         } |         stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])       } } }
#define $MakeMint `rho:id:i4pb65o68q7mgkqpp6ybb41d6bqm153g38xrtymjeg7gdasri3q47b`
#define $locker_GaryC `rho:id:jkugjormrrttw4i6for5h987646ir4j8d9sj8gfwnkwcu4zqmy6cgt`
#define $inbox_GaryC `rho:id:agof71dygw7riegjujrz43y3dtkkqgg75yu8kezurbzjou687o99mp`
#define $locker_nonce_GaryC 4
#define $locker_SteveHenley `rho:id:rh41467aenjk3dqu8i8tm3omkp1d3u8a4ofcdm8d9etueuyh9nw1c5`
#define $inbox_SteveHenley `rho:id:mgkjhrdhigbpnrswmk8fjrcgyo6xwmmaoqquirke8ggemdjwkj4aiw`
#define $locker_Ed `rho:id:b6tgw11fzji4bdpowdnk6uffr3ztkaymisoebohhe8yjdoaimoogu4`
#define $inbox_Ed `rho:id:zpadycadit7147iwtk4ykfgqzzj1jz5fs57mp1kuc1inurtmyc9rjh`
#define $locker_nonce_Ed 5
#define  $readDirectory($name, $arg_ret...) new ret, $stdout in {   $peek("directory",$name,*ret) | for (@[dir, ..._] <- ret ) {     @{dir.get("read")}!($arg_ret)  } }
#define  $add2Directory($name, $key,$value,$ret) new ret, $stdout in {   $peek("directory",$name,*ret) | for (@[dir, ..._] <- ret ) {     @{dir.get("write")}!($key,$value,$ret)  } }
#define  $newBallot($name,$choiceSet) new lookupCh, bCh, $lookup, $stdout in {   lookup!($Ballot, *lookupCh) |   for(Ballot <- lookupCh) {     Ballot!($choiceSet, *bCh) |     for (chair, getWinner <- bCh) {       $send($myusername, "ballot", $name,  {"chair": *chair, "getWinner": *getWinner}) } } }
#define  $allowtovote($user,$ballot) new ret in {   $peek("ballot",$ballot,*ret) |   for ( @[{"chair": chair, ..._}, ..._] <- ret ) {     @chair!("giveRightToVote", *ret) |     for (@vote <- ret) {       $send($user,"vote", $ballot, vote)     }   } }
#define  $voteresults($ballot) new ret, $stdout in {   $peek("ballot",$ballot,*ret) |   for ( @[{"getWinner": results, ..._}, ..._] <- ret ) {     @results!(*stdout)   } }
#define  $receive($type,$subtype_return...) new lockerCh, itemsCh, $stdout, $lookup in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("receive") == Nil ) {         stdout!("you do not have a receive channel for your inbox")       } else {         @{items.get("receive")}!($type, $subtype_return)       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define  $allowalltovote($directory,$ballot) new lockerCh, ret, ret1, loop, $stdout, $lookup in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *ret) |     for (@items  <- ret) {       @{items.get("peek")}!("directory", $directory, *ret) |       @{items.get("peek")}!("ballot", $ballot, *ret1) |       for ( @[{"read": *read, ..._}, ..._] <- ret;             @[{"chair": *chair, ..._}, ..._] <- ret1 ) {         contract loop ( @map ) = {           match  map {             {} => Nil             { username: *inbox, ...tail } => {               chair!("giveRightToVote", *ret) |               for (@vote <- ret) {                 inbox!(["vote", $ballot, vote])               } |               loop!(tail)             }           }         } |         read!(*ret) |         for ( @members <- ret ) {           loop!(members)         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])     }   } }
#define $locker_Owans `rho:id:muoibisbyqztaa9didj65otyxoio95yjkgqj7sgpo55xakhr4ysarq`
#define $locker_nonce_Owans 1
#define $inbox_Owans `rho:id:k3465k7e67faohyo8kt9n9ypaaeiru7hyenbys9rjq3x79a9ssx4dn`
#define  $addUser( $username,$directory) new ret, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *ret) | for(locker <- ret) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *ret) |     for (@items  <- ret) {       /* stdout!(items) | */       if ( items.get("peek") == Nil ) {         stdout!("you do not have an mailbox")       } else {         stdout!("finding " ++ $directory ++ " in inbox of " ++ $myusername) |         @{items.get("peek")}!("directory", $directory,*ret) |         for ( @[{"read": *read, "write": *write, ..._ }, ..._] <- ret ) {           if ( *write == Nil ) {             stdout!("you do not have permission to write to " ++ $directory)           } else {             lookup!($inbox_%%$username,*ret) |             for( inbox <- ret ) {               stdout!("adding " ++ $username) |               write!($username,*inbox, *stdout) |               inbox!(["directory", $directory, {"read": *read}], *stdout)             }           }       } } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define $locker_nonce_SteveHenley 5
#define $locker_tgrospic `rho:id:mb3e8gadzbcuydgng9ig4he7dx1nfgups8xz8jfi6km5bwouupbigd`
#define $locker_nonce_tgrospic 1
#define $inbox_tgrospic `rho:id:k6hn3yz3kforb95bxypa5wb4oakpra64exgj6cmm3mbhh3fhj6xhej`
#define  $foreach($collection,$code) new $stdout, _loop in { contract _loop ( @_map ) = {   match  _map {     {} => Nil     { _key: _value, ..._tail } => {       $code |       _loop!(_tail)     }     []  => Nil     [_value, ..._tail] /\ _key => {       $code |       _loop!(_tail)     }     Set() => Nil     Set(_value, ..._tail) /\ _key => {       $code |       _loop!(_tail)     }   } } | _loop!( $collection ) }
#define $locker_aviation_hacker `rho:id:p88oj4ds1cpoxm8jkqyrfzn8ums9xwzay3a78q6imgcnba8xrzzmxd`
#define $inbox_aviation_hacker `rho:id:dkdmeknxxypdpexfaets3q3b9iizdm3gjh63dcnozntsh93957utdf`
#define  $chat($user,$message) $send($user,"chat",$myusername,$message)
#define $locker_nonce_aviation_hacker 7
#define  $Ballot
#define  $updateinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       $peek(*inboxCh) |       for (oldmail <- inboxCh) {         stdout!(["saving oldmail: ",*oldmail]) |         send!(*oldmail) |         lookup!($locker_%%$myusername, *ret) |         for (locker <- ret) {           stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 2]) |           locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername + 1,             {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |           for (_ <- ret) {             insertArbitrary!(*send,*ret)|             for (uri <- ret) {               stdout!(["#define $inbox_" ++ $myusername, *uri])             }           }         }       }     }   } }
#define  $double($x) 2*$x
#define  $this that"~~"the "~~"other
#define $Locker `rho:id:djbh4ogpqc7fn9tp5ahz6kpuonmtqbype11uycf75fgnrdgz6dj496`
#define $Inbox `rho:id:ayqysruudhpa9r1iurargh6wgbyjyr3nk16ey1dzyztofn44kj4w6w`
#define $Directory `rho:id:sq8mauht78ufw7r1etupb8mzgp4j8ii5etpo81ec3t5k57b6958ju5`
#define $locker_jimscarver `rho:id:utjdj4urgdf99t9qnrywt3rtdaijapw783mktoqndpfzxf3x3tw488`
#define $inbox_jimscarver `rho:id:o7c4wzx33u9c3ctc8kwsjjrgp859j7wfjdy5wyurjnmrzjxdi4pcdo`
#define $locker_nonce_jimscarver 2
#define  $makemylocker new lookupCh, stdout(`rho:io:stdout`),"~~"      lookup(`rho:registry:lookup`), insert(`rho:registry:insertArbitrary`), insCh in {"~~"  lookup!($Locker, *lookupCh) |"~~"  for(Locker <- lookupCh) {"~~"    stdout!(["found Locker",*Locker]) |"~~"    new capabilities, ret, inboxCh, lockerCh, uriCh in {"~~"      Locker!($mypubkey.hexToBytes(), *lockerCh) | for(locker <- lockerCh) {"~~"stdout!("made Locker") |"~~"        insert!(*locker, *uriCh) | for (@uri <- uriCh) {"~~"          stdout!(["#define $locker_" ++ $myusername, uri])"~~"        } |"~~"        lookup!($Inbox, *inboxCh) |"~~"        for (inbox <- inboxCh) {"~~"          inbox!(*capabilities) |"~~"          for (receive, send, peek <- capabilities) {"~~"            stdout!(["#define $locker_nonce_" ++ $myusername, 1]) |"~~"            locker!("update", $myprivkey.hexToBytes(),0, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |"~~"            for (_ <- ret) {"~~"              insert!(*send,*ret)|"~~"              for (uri <- ret) {"~~"                stdout!(["#define $inbox_" ++ $myusername, *uri])"~~"              }"~~"            }"~~"          }"~~"        }"~~"      }"~~"    }"~~"  }"~~"}
#define $locker_jimscarver `rho:id:utjdj4urgdf99t9qnrywt3rtdaijapw783mktoqndpfzxf3x3tw488`
#define $inbox_jimscarver `rho:id:o7c4wzx33u9c3ctc8kwsjjrgp859j7wfjdy5wyurjnmrzjxdi4pcdo`
#define $locker_nonce_jimscarver 2
#define  $makemylocker new lookupCh, stdout(`rho:io:stdout`),"~~"      lookup(`rho:registry:lookup`), insert(`rho:registry:insertArbitrary`), insCh in {"~~"  lookup!($Locker, *lookupCh) |"~~"  for(Locker <- lookupCh) {"~~"    stdout!(["found Locker",*Locker]) |"~~"    new capabilities, ret, inboxCh, lockerCh, uriCh in {"~~"      Locker!($mypubkey.hexToBytes(), *lockerCh) | for(locker <- lockerCh) {"~~"stdout!("made Locker") |"~~"        insert!(*locker, *uriCh) | for (@uri <- uriCh) {"~~"          stdout!(["#define $locker_" ++ $myusername, uri])"~~"        } |"~~"        lookup!($Inbox, *inboxCh) |"~~"        for (inbox <- inboxCh) {"~~"          inbox!(*capabilities) |"~~"          for (receive, send, peek <- capabilities) {"~~"            stdout!(["#define $locker_nonce_" ++ $myusername, 1]) |"~~"            locker!("update", $myprivkey.hexToBytes(),0, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |"~~"            for (_ <- ret) {"~~"              insert!(*send,*ret)|"~~"              for (uri <- ret) {"~~"                stdout!(["#define $inbox_" ++ $myusername, *uri])"~~"              }"~~"            }"~~"          }"~~"        }"~~"      }"~~"    }"~~"  }"~~"}
#define  $foreach($collection,$code) new $stdout, _loop in {"~~"contract _loop ( @_map ) = {"~~"  match  _map {"~~"    {} => Nil"~~"    { _key: _value, ..._tail } => {"~~"      $code |"~~"      _loop!(_tail)"~~"    }"~~"    []  => Nil"~~"    [_value, ..._tail] /\ _key => {"~~"      $code |"~~"      _loop!(_tail)"~~"    }"~~"    Set() => Nil"~~"    Set(_value, ..._tail) /\ _key => {"~~"      $code |"~~"      _loop!(_tail)"~~"    }"~~"    /* _ /\ _name => {for ( @collection <- @_name ) { _loop!(collection) } } */"~~"  }"~~"} | _loop!( $collection )"~~"}
