/*-----------------------------------------------------19910729 RWB-------*/
/* msglib.h -- message library prototypes                                 */
/*------------------------------------------------------------------------*/

byte open_message_pipe(int station);
byte close_message_pipe(int station);
byte broadcast_message( byte station, char message[] );
byte send_message( byte station, char message[] );
byte recv_message( char message[] );
