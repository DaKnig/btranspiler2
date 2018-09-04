# boptimize
this program makes an Abstract Syntax Tree out of a brainfuck program
## the syntax:
```	program = statementList
	statementList = statement*
	statement = + | - | > | < | . | , | loop | e
	loop = '[' statementList ']'	```

see typedef statement, loop

implemented according to the brainducks specifications in other brainducks

notes to self - comments with "NTS"

assumes a preprocessor did some work before it, but work just fine without it
