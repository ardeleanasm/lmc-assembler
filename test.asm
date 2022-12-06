	INP 		//read N value
	STA NVAL	//save Nvalue
LOOP	LDA PVAL	//load 0 from memory
	OUT		//print value
	ADD ONE		//add one
	STA PVAL	//store value
	LDA NVAL	//load N value
	SUB ONE		//subtract one
	STA NVAL	//store new N value
	BRP LOOP
	HLT
NVAL	DAT
PVAL	DAT 0
ONE	DAT 1
	
