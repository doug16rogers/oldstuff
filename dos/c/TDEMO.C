/*  tdemo.c  --  Sample C code for Tree Diagrammer demo  */


/* <c> Initializes Variables */		

void initialize ()
{
    mp = malloc (2000);

    clear (&ordrec);

    clear (scratch);

    clear (ord_table);
}


/* Read orders */

int read_ord (n, t, pord)
int n, t;
struct order *pord;
{
    printf ("Please enter data ");
		
    /* <c> Get Data from User */
    get_data (t, pord);

    scanf ("%d", i);

    /* <c> Process Orders */
    proc_ord (ordno, type, &ordrec);

    return validate ();
}


void proc_ord (n, t, pord)
int n, t;
struct order *pord;
{
    if (strcmp (s, u)) 
        s++;

    if (strlen (s) < 8) 
        return;
	 
    printf ("Order is valid");
}

					   
void get_data (t, p)
int t;
struct order *p;
{
    while (fill (p));
}


int fill (p)
struct order *p;
{
    scanf ("come in here...");
}


int summarize (pord, ptot)
struct order *pord;
struct totals *ptot;
{
    if (pord == 0) 
        return 0;
    return 1;
}



main (argc, argv)
char **argv;
int argc;
{
    initialize (); 

    printf ("Sales Program");

    read_ord (ordno, type, &ordrec);

    read_ord (ordno, type1, &ordrec1);

    read_ord (ordno, type2, &ordrec2);
						
    /* <c> Make Summary Report */
    summarize (&ordrec, &totals);
		   			  
    exit (0);
}
