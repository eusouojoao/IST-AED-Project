#ifndef __writeOutputFile__
#define __writeOutputFile__

void write2outputFile (char *output, int to_print);

#ifdef FINAL
void writeInvalid (char *output, bool first);
void writeZero(char *output, bool first);
#endif

#endif
