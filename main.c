/*** includes ***/
//check
#include<ctype.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>

/*** defines ***/
#define CTRL_KEY(k) ( (k) & 0x1f)

/*** data ***/
struct termios orig_termios;

/*** terminal ***/
void die(const char *s){
  perror(s);
  exit(1);
}


void disableRawMode(){
  if ( tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig_termios) == -1) die("tsgetattr"); //tcgetattr() reads the current attributes into a struct
}


void enableRawMode(){
  if (tcgetattr(STDIN_FILENO,&orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);     //quits as soon as you press q
  raw.c_cc[VMIN]= 0;
  raw.c_cc[VTIME]= 1;
  if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw) == -1) die("tcsetattr");
}

/*** init ***/

int main()
{
  enableRawMode();
while (1) {
    char c ='\0';
    if(read(STDIN_FILENO,&c,1) == -1 && errno != EAGAIN) die("read");
//  char c; 
//  while (read(STDIN_FILENO,&c,1)==1 && c!='q') {
  if (iscntrl(c)) {                               //if input is control character return 1 else 0
      printf("%d\n",c);
     }else {
     printf("%d ('%c')\r\n",c,c);                  //Ctrl-S stops sending you output. Press Ctrl-Q to tell it to resume sending you output
      /*printf("%c\n",c); */
          }
// if(c=='q') break;
    if(c == CTRL_KEY('q')) break;
//  }
  }
  return 0; 
}
