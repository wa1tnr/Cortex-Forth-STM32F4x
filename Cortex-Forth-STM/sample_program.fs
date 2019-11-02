: ledon   1 13 pinwrite ;
: ledoff  0 13 pinwrite ;
: waitfor 222111 0 do 1 drop loop ;
: togl    ledon waitfor ledoff waitfor ;
: setups  1 13 pinmode ;
: showme  0 do 43 emit togl 32 emit loop ;
: go      cr setups 12 showme ;
