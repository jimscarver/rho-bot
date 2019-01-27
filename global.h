#define  $aviation_hacker_inbox  `rho:id:ez1fr58u5em45fdw7uya91q6hga5bdxneaemhfd68wwma7i9nz3do7`
#define  $dan_locker `rho:id:pnce9bawterr16esnmopkjd1mutr4ukyhbterjn957pkwg8dpawmux`
#define  $jim_locker `rho:id:n953kxiotz6bjhe8o5xndzi7mgwo5jxumccbbroe37ue7j3eyosrd4`
#define  $jim_inbox `rho:id:6bf1cjwacbu5c8bf3yg8h6g7pj9iyih8eqqyxf3y39xt99jfa3318i`
#define  $stdnames lookup(`rho:registry:lookup`), insertArbitrary(`rho:registry:insertArbitrary`), stdout(`rho:io:stdout`), ack, return
#define  $testvotes($nonce) new  lockerCh, itemsCh, wCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in { lookup!($dan_locker, *lockerCh) | for(locker <- lockerCh) { locker!("get", "aaaa".hexToBytes(), $nonce, *stdout, *itemsCh) | for (@{"getWinner": winner, ..._} <- itemsCh) { @winner!(*wCh) | for (@(whom, tally) <- wCh) { stdout!({"winner": whom, "tally": tally}) } } } }
#define  $testgiveright($inbox,$nonce) new inboxCh, lockerCh, itemsCh, voteCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in { lookup!($dan_locker, *lockerCh) | for(locker <- lockerCh) { locker!("get", "aaaa".hexToBytes(), $nonce, *stdout, *itemsCh) | for (@{"chair": chair, ..._} <- itemsCh) { @chair!("giveRightToVote", *voteCh) | for (vote <- voteCh) { lookup!($inbox, *inboxCh) | for(inbox <- inboxCh) { inbox!(*vote) | stdout!({"sending right to vote to": $inbox}) } } } } }
#define  testvote($mylocker, $vote, $nonce) new lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in { lookup!($mylocker, *lockerCh) | for(locker <- lockerCh) { locker!("get", "aaaa".hexToBytes(), $nonce, *stdout, *itemsCh) | for (@{"inbox": inbox, ..._} <- itemsCh) { for(vote <- @inbox) { vote!("vote", $vote, *stdout, *stdout) } } } }
#define  $aviation_hacker_locker `rho:id:5nikz3pd5cygq9cjbps43foyage5rnqz5ey94uh8f4uks7g4mb63dr`
#define admin_flavors `rho:id:w1grxe9ybwegz7cchmrrjzckaq5gumo8qmy3xw87i1w9ws58ar4kcc`
#define results_flavors `rho:id:98rypkebm37155kbyoj5i9jhu5gceteexxac9z68r1zdhta1cyjh4u`
#define  $print($string) new stdout(`rho:io:stdout`) in { stdout!($string) }
#define s in your code are local to the code.
#define  $define($name,$value) stdout!(["#define " ++ $name, $value])
#define  $stdout stdout(`rho:io:stdout`)
#define  $define($name,$value) stdout!(["#define " ++ $name, $value])
#define  $trace trace(`rho:io:stderr`)
#define  $insertArbitrary insertArbitrary(`rho:registry:insertArbitrary`)
#define  $lookup lookup(`rho:registry:lookup`)
#define  $Locker `rho:id:8zzizytk8anrafnmonsyndhjt1ryn5z9ei618kx4n7ibse7r9ftdq4`
#define  $makemylocker new lookupCh, stdout(`rho:io:stdout`),       lookup(`rho:registry:lookup`), insert(`rho:registry:insertArbitrary`), insCh in {   lookup!($Locker, *lookupCh) |   for(Locker <- lookupCh) { stdout!("found Locker") |     new lockerCh, uriCh in {       Locker!($mypubkey.hexToBytes(), *lockerCh) | for(locker <- lockerCh) { stdout!("made Locker") |         insert!(*locker, *uriCh) | for (@uri <- uriCh) {           stdout!(["#define $locker_" ++ $myusername, uri])        } |         new inbox in {           insert!(bundle+{*inbox}, *insCh) |           for (@uri <- insCh) {             stdout!(["#define $inbox_" ++ $myusername, uri])           } |           locker!("update", $myprivkey.hexToBytes(), 0, {"inbox": *inbox}, *stdout, *stdout) |           stdout!(["#define $locker_nonce_" ++ $myusername, 1])         }       }     }   } }
#define $locker_jimscarver `rho:id:8wp8yebc5zzdr14h6jzxt9mo5c5p6pj919ojgj69x7th61b6iqjsyj`
#define   $mylockerStore($key,$value) new $stdout, $lookup, nonce , ret in {   lookup!($locker_##$myusername, *ret) |   for (locker <- ret) {      locker!("update", $myprivkey.hexToBytes(),$locker_nonce_##$myusername, {$key: $value}, *stdout, *ret)     for (_ <- ret) {        stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_##$myusername + 1])      }   } }
#define  $test "value"
#define  $test_value "xyzzy"
#define   $mylockerGet($ret) new $stdout, $lookup, nonce , lockerCh in {   lookup!($locker_%%$myusername, *lockerCh) |   for (locker <- lockerCh) {       locker!("get", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername,  *stdout, *$ret) |      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])    } }
#define   $mylockerUpdate($key,$value) new $stdout, $lookup, nonce , lockerCh, ret in {   lookup!($locker_%%$myusername, *lockerCh) |   for (locker <- lockerCh) {       locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername, { $key: $value},  *stdout, *ret) |      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])    } }
#define $locker_nonce_aviation_hacker 1
#define $inbox_aviation_hacker `rho:id:nmx9guxghysekqqnxszarz7ax7nadjy7jcss11xczgdjg96ax8c3rb`
#define $locker_aviation_hacker `rho:id:ak1o8wnbsnwc7gacpymhiugauyb5azgsks7c49nkfzf9xf7eo44t8x`
#define  $newBallot($name,$choiceSet) new lookupCh, bCh, Ballot, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`), lockerCh, ackCh in {   lookup!($Ballot, *lookupCh) |   for(Ballot <- lookupCh) {     Ballot!($choiceSet, *bCh) |     for (chair, getWinner <- bCh) {       lookup!($locker_%%$myusername, *lockerCh) |       for(locker <- lockerCh) {         locker!("update", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, {$name: {"chair": *chair, "getWinner": *getWinner}}, *stdout, *ackCh) |         stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}])       }     }   } }
#define $inbox_henleynatural `rho:id:n368ncyu3zchposasuiteqmwrpokn4yteybimg7oyf9d8ru885hsa6`
#define $locker_henleynatural `rho:id:e9tqyxqpxojr39ijirg3xki1hg19z4pamtbgqyduucwbgk4mrsr4qr`
#define $locker_nonce_henleynatural 2
#define  $voteresults($name) new  lockerCh, itemsCh, wCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {    lookup!($locker_%%$myusername, *lockerCh) |    for(locker <- lockerCh) {      locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |      stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |       for (@{$name: ballot, ..._} <- itemsCh) {         @{ballot.get("getWinner")}!(*wCh) | for (@(whom, tally) <- wCh) {           stdout!({"winner": whom, "tally": tally})  } } } }
#define $locker_nonce_GaryC 1
#define $inbox_GaryC `rho:id:68iayzhm7q37mf9isdikibye5py1qdo3m1fpcqbdb5setzzt93ayd9`
#define $locker_GaryC `rho:id:wu1nxbkq1bbuwpcqzobck6h6zcm6yjmnjnneihc5fnzojsnd3tm8y4`
#define $locker_Valentine `rho:id:9rj6x4ax1hatnw6z3jfrf7grwrrx5bkpxwcr34g87598yrwph4658e`
#define $inbox_Valentine `rho:id:wizy5rbccxrpsppt8jegrqd8gecjrcrggfmjkehoeyf49by7647mm8`
#define $locker_nonce_Valentine 2
#define  $inbox_dummyuser `rho:id:ya687kjhwtyj877fedddh9jiac5ydfpcjycyfa16cduhrhmwc7n738`
#define  $allowtovote($ballot,$vote) #define $ballot "flavors" new inboxCh, lockerCh, itemsCh, voteCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) |   for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |     for (@{$ballot: ballot, ..._} <- itemsCh) {       @{ballot.get("chair")}!("giveRightToVote", *voteCh) |         for (vote <- voteCh) { lookup!($inbox_%%$user, *inboxCh) |           for(inbox <- inboxCh) {           inbox!(["vote",$ballot,*vote]) |           stdout!({"sending right to vote to": $user})  } } } } }
#define  #define $inbox_dummyuser `rho:id:ya687kjhwtyj877fedddh9jiac5ydfpcjycyfa16cduhrhmwc7n738` $allowtovote("dummyuser"."flavors") new inboxCh, lockerCh, itemsCh, voteCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) |   for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |     for (@{$ballot: ballot, ..._} <- itemsCh) {       @{ballot.get("chair")}!("giveRightToVote", *voteCh) |         for (vote <- voteCh) { lookup!($inbox_%%$user, *inboxCh) |           for(inbox <- inboxCh) {           inbox!(["vote",$ballot,*vote]) |           stdout!({"sending right to vote to": $user})  } } } } }
#define  #define $inbox_dummyuser `rho:id:ya687kjhwtyj877fedddh9jiac5ydfpcjycyfa16cduhrhmwc7n738` $allowtovote($ballot,$user) new inboxCh, lockerCh, itemsCh, voteCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) |   for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |     for (@{$ballot: ballot, ..._} <- itemsCh) {       @{ballot.get("chair")}!("giveRightToVote", *voteCh) |         for (vote <- voteCh) { lookup!($inbox_%%$user, *inboxCh) |           for(inbox <- inboxCh) {           inbox!(["vote",$ballot,*vote]) |           stdout!({"sending right to vote to": $user})  } } } } }
#define  #define $inbox_dummyuser `rho:id:ya687kjhwtyj877fedddh9jiac5ydfpcjycyfa16cduhrhmwc7n738` $allowtovote($user,$ballot) new inboxCh, lockerCh, itemsCh, voteCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) |   for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |     for (@{$ballot: ballot, ..._} <- itemsCh) {       @{ballot.get("chair")}!("giveRightToVote", *voteCh) |         for (vote <- voteCh) { lookup!($inbox_%%$user, *inboxCh) |           for(inbox <- inboxCh) {           inbox!(["vote",$ballot,*vote]) |           stdout!({"sending right to vote to": $user})  } } } } }
#define  $Inbox `rho:id:nhnbxg4i3rip5d851iwrdc4wt9s4pij3p5sn8u7d1414fznwf7utrt`
#define $locker_Owans `rho:id:tq6q9cf7ucyd5ys6pyce765meohstcr6antk1t9r319fk44xnsb3ou`
#define $inbox_Owans `rho:id:m9g4jx55gjfsk8bfrbh9ydmuquixf9ec6eorupd1gbz4opsgq9jq7g`
#define $locker_nonce_Owans 2
#define   $Inbox `rho:id:nhnbxg4i3rip5d851iwrdc4wt9s4pij3p5sn8u7d1414fznwf7utrt`
#define  $add($x,$y)  ($x + $y)
#define  $newinbox new inboxCh, capabilities, ret, $stdout, $lookup, $insertArbitrary in {   lookup!($Inbox, *inboxCh) |   for (inbox <- inboxCh) {     inbox!(*capabilities) |     for (receive, send, peek <- capabilities) {       lookup!($locker_%%$myusername, *ret) |       for (locker <- ret) {         stdout!(["#define $locker_nonce_" ++ $myusername, $locker_nonce_%%$myusername + 1]) |         locker!("update", $myprivkey.hexToBytes(),$locker_nonce_%%$myusername, {"inbox": *send, "receive": *receive, "peek": *peek}, *stdout, *ret) |         for (_ <- ret) {           insertArbitrary!(*send,*ret)|           for (uri <- ret) {             stdout!(["#define $inbox_" ++ $myusername, *uri])           }         }       }     }   } }
#define $inbox_jimscarver `rho:id:no1ft9ooyskpfgsmtbk1ai57o5ddax16zwsku63gdrk5ftqxbdaubt`
#define  $allowtovote($user,$ballot) new inboxCh, lockerCh, itemsCh, voteCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) |   for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) |     for (@{$ballot: ballot, ..._} <- itemsCh) {       @{ballot.get("chair")}!("giveRightToVote", *voteCh) |         for (vote <- voteCh) { lookup!($inbox_%%$user, *inboxCh) |           for(inbox <- inboxCh) {           inbox!(["vote",$ballot,*vote],*stdout) |           stdout!({"sending right to vote to": $user})  } } } } }
#define $Ballot `rho:id:3qfh1fy7jwfcai7ceyorux4a18hzcn83n9xb6dramjf5gs7cw8fynf`
#define $locker_nonce_jimscarver 49
#define  $vote($ballot,$vote) new ret, lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       /* stdout!(["locker",items]) | */       if ( items.get("receive") == Nil ) {         stdout!("you do not have a receive channel for your inbox")       } else {         @{items.get("receive")}!(*stdout) |         stdout!("getting voter") |         @{items.get("receive")}!("vote", $ballot, *ret) |         for( voter  <- ret ) {           stdout!(["voter",*voter]) |           voter!("vote",$vote,*stdout,*stdout) |           stdout!(["vote",$ballot,$myusername,"received"])         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define $locker_nonce_jimscarver 50
#define $locker_nonce_jimscarver 51
#define  $vote($ballot,$vote) new ret, lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       /* stdout!(["locker",items]) | */       if ( items.get("receive") == Nil ) {         stdout!("you do not have a receive channel for your inbox")       } else {         @{items.get("receive")}!(*stdout) |         stdout!("getting voter") |         @{items.get("receive")}!("vote", $ballot, *ret) |         for( [voter]  <- ret ) {           stdout!(["voter",*voter]) |           voter!("vote",$vote,*stdout,*stdout) |           stdout!(["vote",$ballot,$myusername,"received"])         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define $locker_nonce_jimscarver 52
#define  $vote($ballot,$vote) new ret, lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       /* stdout!(["locker",items]) | */       if ( items.get("receive") == Nil ) {         stdout!("you do not have a receive channel for your inbox")       } else {         @{items.get("receive")}!(*stdout) |         stdout!("getting voter") |         @{items.get("receive")}!("vote", $ballot, *ret) |         for( @{[voter]}  <- ret ) {           stdout!(["voter",voter]) |           @voter!("vote",$vote,*stdout,*stdout) |           stdout!(["vote",$ballot,$myusername,"received"])         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define $locker_nonce_jimscarver 53
#define $locker_nonce_jimscarver 54
#define $locker_nonce_jimscarver 55
#define $Inbox `rho:id:ds397egbwqrtbznbs86pph8n55io6zgzf65snke1hkd339wu18k5s3`
#define $locker_nonce_jimscarver 56
#define $inbox_jimscarver `rho:id:zm5s17wmfdwgbauikxsx7gnra4ie87rtk6jkspg8mzb1mz9xbqshzi`
#define $locker_nonce_GaryC 2
#define $inbox_GaryC `rho:id:ubfm79n1ocdeaa3fk3npojynymq616y1cm7f3qtfoz53od1wybgq97`
#define $locker_nonce_Owans 3
#define $inbox_Owans `rho:id:4pmx9gacsnrxprdaxkagm7ytk38xgcd9x9d6noium7ndjk64tdqkq3`
#define $locker_nonce_jimscarver 57
#define $locker_nonce_jimscarver 58
#define $locker_nonce_jimscarver 59
#define $locker_nonce_jimscarver 60
#define $locker_nonce_Valentine 3
#define $inbox_Valentine `rho:id:c1331esaa9p3465k43ffbx1gcak7bp3q4ghkpe4gak5s3j717ycm4d`
#define $locker_nonce_jimscarver 61
#define $locker_nonce_jimscarver 62
#define  $vote($ballot,$vote) new ret, lockerCh, itemsCh, lookup(`rho:registry:lookup`), stdout(`rho:io:stdout`) in {   lookup!($locker_%%$myusername, *lockerCh) | for(locker <- lockerCh) {     locker!("get", $myprivkey.hexToBytes(), $locker_nonce_%%$myusername, *stdout, *itemsCh) |     for (@items  <- itemsCh) {       if ( items.get("peek") == Nil ) {         stdout!("you do not have a peek channel for your inbox")       } else {         @{items.get("peek")}!(*stdout) |         stdout!("getting voter") |         @{items.get("peek")}!("vote", $ballot, *ret) |         for( @{[voter]}  <- ret ) {           @voter!("vote",$vote,*stdout,*stdout) |           stdout!(["vote",$ballot,$myusername,"received"])         }       } |       stdout!(["#define $locker_nonce_" ++ $myusername, {$locker_nonce_%%$myusername + 1}]) } } }
#define $locker_nonce_jimscarver 63
#define $locker_nonce_jimscarver 64
#define $locker_nonce_jimscarver 65
#define $locker_nonce_jimscarver 66
#define $locker_nonce_jimscarver 67
#define $locker_nonce_jimscarver 68
#define $locker_nonce_jimscarver 69
