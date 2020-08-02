#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void holy_grail() {
  char b[1024];
  char a[256];
  strcpy(a, "echo \"I am Arthur, King of the Britons\n\"");
  fgets(b, 1024, stdin);
  strcat(a, b);
  for (int i = 0; i < strlen(b); i++) {
    if(b[i] == '\n') continue;
    if (b[i] < '0' || b[i] > '9') {
      fputs("You can't expect to wield supreme "
           "executive power just because some "
           "watery tart threw a sword at you.\n",stdout);
      fflush(stdout);
      return;
    }
  }
  fputs("Who are you, who are so wise in the "
       "ways of science?\n", stdout);
  fflush(stdout);
  system(a);
  fflush(stdout);
  exit(0);
}

int main(int argc, const char* argv[]) {
  setuid(0);
  fputs("What is the airspeed velocity of an unladen swallow?\n", stdout);
  fflush(stdout);
  holy_grail();
  return 0;
}
