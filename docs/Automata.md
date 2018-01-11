# Automata

For error checking of queries an FSM is used. The table of this fsm is stored in `resource/automata-table`. The protocol fo the file is
~~~
TokenTypesCount
StateCount
TokenType1  TokenType2  ... TokenTypeN
Action11    Action12    ... Action1N
.           .           ... 
.           .           ...
.           .           ...
ActionM1    ActionM2    ... ActionMN
~~~